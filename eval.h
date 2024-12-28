/**************************
*** function depends on ***
**************************/

#include "stationaryEval.h"
#include "assignMoveListAndSort.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "newKey.h"
#include "newHash.h"
#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef evalFILE
#define evalFILE

/***************
*** Let's go ***
***************/

int eval(int board[8][8],unsigned long long int key, long long int hash, int atDepth, int depth2Go, int alpha, int beta, int endTime, int maxDepth, int captures){
	
	nodes++;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	
	/*****************************************************************
	***** First we examine, wether we need to return immediately *****
	*****************************************************************/
	
	if (depth2Go==0 || atDepth==maxDepth){
		return stationaryEval(board,key);
	}
	
	
	if (atDepth<=3 && atDepth>0){ // check for repetition at shallow depth, but not at the root
		int howOftenOccured=0;
		for (int i=0;i<100;i++){
			if (hash==previous100PositionHashes[i]){howOftenOccured++;}
		}
		if (howOftenOccured>=2){return 0;} // occured already 2 times -> now 3 times
	}
	
	if (atDepth>0 && nrPieces<=3){ // 3men Tablebase
		return stationaryEval(board,key);
	}

	/*******************************
	***** Repetition detection *****
	*******************************/
	
	// if this position was reached earlier in the tree, consider it drawn
	
	branch_hashes[atDepth]=hash;
	if (atDepth>=4){
		for (int d=atDepth-4;d>=0;d-=2){
			if (branch_hashes[d]==hash){
				return 0;
			}
		}
	}
	
	/****************************
	***** Search extensions *****
	****************************/
	
	// 4 captures extend the search by 2 ply. This is allowed once
	
	if (captures==4 && depth2Go>=2){
		captures=-123;
		depth2Go+=2;
		maxDepth+=2;
	}
	
	
	/********************************************************
	***** Is the current position so good we can abort? *****
	********************************************************/

	// when we are at "depth2Go<=3" and we are in a very good position, we will prune this branch,
	// assuming we are not in check. However, it seems cutting off only is beneficial, if we don't
	// risk getting to the same position with iterative deepening again but then don't have a hashmove ready
	
	
	int standingPat=stationaryEval(board,key);
	
	int margins[4]={0,0,250,500};
	if (depth2Go>=1 && depth2Go<=3 && atDepth>=6){
		if ((turn==+1 && standingPat>= beta+margins[depth2Go])||(turn==-1 && standingPat<= alpha-margins[depth2Go])){
			if (isKingInCheck(board,turn,0,0)==0){
				return standingPat+turn;
			}
		}
	}
	
	/***************************
	***** Nullmove pruning *****
	***************************/
	
	int score;
	unsigned long long int newKeyy;
	long long int newHashh;
	
	if ((standingPat>beta+100 && turn==1)||(standingPat<alpha-100 && turn==-1)){
		if (depth2Go>3 && atDepth!=0){
			if (isKingInCheck(board,turn,0,0)==0){
				newKeyy=newKey(board,key,0);
				newHashh=newHash(board,key,hash,0);
				//using R=2
				score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1-2,alpha,beta,endTime,maxDepth-2,captures);
				
				if ((score>beta && turn==1)||(score<alpha && turn==-1)){
					return score;
				}
			}
		}
	}
	
	
	/*************************************************
	***** Generate and order move simultaneously *****
	*************************************************/
	
	unsigned short int thisMoveFirst=0;
	bool doHashMoves=(atDepth<=5 && atDepth>0);
	
	int hash4moveOrder;
	long long int hashOfTable;
	
	
	if (atDepth==0){  // move the root best move to first position
		thisMoveFirst=rootBestMove&0xffff;
	} else if (doHashMoves){
		hash4moveOrder=int(abs(hash%ht_size));
		hashOfTable=ht_hash[hash4moveOrder];
		
		if (hashOfTable==hash){// we have the exact same hash stored!
			thisMoveFirst =ht_moveShort[hash4moveOrder];
			hashhits++;
		}
	}
	
	//so, if we have no hashmove but still a way to go, we will do a shallow search to find a good first move
	if (thisMoveFirst==0 && depth2Go>=4){
		eval(board,key,hash,atDepth,depth2Go-2,-INF, INF, endTime, atDepth+4,-123);
		thisMoveFirst=evalBestMove;
	}
	
	if (clock()>endTime){exitSearch=true;return 0;}	
	unsigned int moveList[250];
	assignMoveListAndSort(board,key,moveList,true,thisMoveFirst);
	
	if (moveList[0]==0){ //there are no legal moves -> checkmate or stalemate
		if (moveList[moveList[0]+1]==1){ // we are in check
			return -turn*100000;
		} else {
			return 0;
		}
	}
	
	
	
	/*******************************
	***** Do the actual search *****
	*******************************/


	int bestScore=-INF*turn;
	int bestMoveIndex;	
	bool aQuietMoveTried=false;
	unsigned int move;
	
	for (int i=1; i<=moveList[0];i++){
		move=moveList[i];
		if (exitSearch){return 0;} // check if search has to be aborted.
		
		
		// when depth2Go=1, we only want to try a single quiet move, that was highest ranked after moveordering
		// this prunes the tree and also returns a better estimate of the true evaluation
		// for instance, we won't move the queen to a very active square, where it can be captured by a pawn
		if (move<=4095){ // 7*(1+8+64+512)
			if (aQuietMoveTried && depth2Go==1){continue;}
			aQuietMoveTried=true;
		}
		
		// These lines reduce the number of nodes dramatically, but is mostly for aesthetic purposes
		// The actual time save is almost negligible
		// Some tests suggest it results in a 7 Elo improvement... so lets keep it...
		// It skips moves that will most probably be killed immediately by frontier pruning
		if (depth2Go==2 && ((standingPat<alpha+60 && turn==1) || (standingPat>beta-60 && turn==-1)) 
			&& i>5 && move<=4095){continue;}
		
		if (depth2Go==2 && ((standingPat<alpha-70 && turn==1) || (standingPat>beta+70 && turn==-1)) 
			&& i>1 && move<=4095){continue;}
		
		
		newKeyy=newKey(board,key,move);
		newHashh=newHash(board,key,hash,move);
		
		assignMakeMove(board,move);
		
		
		if (move<=4095){ // 7*(1+8+64+512): no capture or anything
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime,maxDepth,captures);
			
		} else {
			if (depth2Go>1){
				score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime,maxDepth,captures+1);
			} else {
				score=eval(board,newKeyy,newHashh,atDepth+1,1,alpha,beta,endTime,maxDepth,captures+1);
			}	
		}
		
		assignUndoMove(board,move);
		
		
		/*************************************
		***** Alpha/Beta Updates/Cutoffs *****
		*************************************/
		
		
		if (turn==1 && score >alpha){alpha=score;}
		if (turn==-1 && score <beta){beta=score;}
		
		
		if ((turn==1 && score >beta) || (turn==-1 && score <alpha)){ // "beta-cutoff"
			
			if (doHashMoves && !exitSearch){
				ht_hash[hash4moveOrder]=hash;
				ht_moveShort[hash4moveOrder]=move& 0xffff; 
				hashstored++;
			}
			evalBestMove=move;
			return score;
		}
		
		
	
		if (score*turn>bestScore*turn){
			bestScore=score;
			if (atDepth==0 && !exitSearch){
				rootBestMove=move;
			}
			bestMoveIndex=i;
		}
		
	}
	
	

	/**************************************************************
	***** Return our findings if there were no "beta-cutoffs" *****
	**************************************************************/
	
	// store best move if no cutoff occured
	if (doHashMoves && !exitSearch){
		ht_hash[hash4moveOrder]=hash;
		ht_moveShort[hash4moveOrder]=moveList[bestMoveIndex]& 0xffff;
		hashstored++;
	}

	
	evalBestMove=moveList[bestMoveIndex];
	return bestScore;
}	

#endif