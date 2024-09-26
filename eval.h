/**************************
*** function depends on ***
**************************/

#include "stationaryEval.h"
#include "assignMoveListAndSort.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "newKey.h"
#include "globalVariables.h"
//#include "moveID.h"

/************
*** Guard ***
************/

#ifndef evalFILE
#define evalFILE

/***************
*** Let's go ***
***************/

int eval(int board[8][8],unsigned long long int key, long long int hash, int atDepth, int depth2Go, int alpha, int beta, int endTime, int maxDepth){
	
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
		if (howOftenOccured>=2){return 0.;} // occured already 2 times -> now 3 times
	}
	
	if (atDepth>0 && nrPieces<=3){ // 3men Tablebase
		return stationaryEval(board,key);
	}
	
	
	/********************************************************
	***** Is the current position so good we can abort? *****
	********************************************************/

	// currently, when we are at "depth2Go==1", we will only make one move and then evaluate, unless this move is a capture.
	// so: if we are in a very good position, any non-capture move will cause a cutoff. Therefore, we don't need the moveGeneration at all.
	// but maybe, we should at least check that we are not in check, otherwise there might be stupid consequences... so:
	
	int standingPat=stationaryEval(board,key);
	if ((depth2Go==1) && (atDepth>=6)){
		if ((turn==+1 && standingPat>= beta)||(turn==-1 && standingPat<= alpha)){
			if (isKingInCheck(board,turn,0,0)==0){
				return standingPat+turn;
			}
		}
	}
	// it seems cutting off only is beneficial, if I don't risk getting to the same position with iterative deepening again but then don't have a lookup ready

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
				score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1-2,alpha,beta,endTime,maxDepth-2);
				
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
	bool doHashMoves=false;
	
	int hash4moveOrder;
	long long int hashOfTable;
	unsigned short int moveIDD;
	
	if (atDepth<=5 && atDepth>0){
		doHashMoves=true;
	}
	
	if (atDepth==0){  // move the root best move to first position
		thisMoveFirst=rootBestMove&0xffff;
	} else if (doHashMoves){
		hash4moveOrder=int(abs(hash%hashMoveOrderingTableSize));
		hashOfTable=ht_hash[hash4moveOrder];
		
		if (hashOfTable==hash){// we have the exact same hash stored!
			thisMoveFirst =ht_moveID[hash4moveOrder];
			hashhits++;
		}
	}
	
	
	if (clock()>endTime){exitSearch=true;return 0;}	
	unsigned int moveList[250];
	assignMoveListAndSort(board,key,moveList,1,thisMoveFirst);
	
	//if (moveList[moveList[0]+1]==0 &&  isKingInCheck(board,turn,0,0)){cout << "this should not happen!\n";}
	//if (moveList[moveList[0]+1]!=0 && !isKingInCheck(board,turn,0,0)){cout << "this should not happen!\n";}
	
	if (moveList[0]==0){ //there is a final evaluation
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
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime,maxDepth);
			
		} else {
			if (depth2Go>1){
				score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime,maxDepth);
			} else {
				score=eval(board,newKeyy,newHashh,atDepth+1,1,alpha,beta,endTime,maxDepth);
			}	
		}
		
		assignUndoMove(board,move);
		
		
		/*************************************
		***** Alpha/Beta Updates/Cutoffs *****
		*************************************/
		
		
		if (turn==1 && score >alpha){alpha=score;}
		if (turn==-1 && score <beta){beta=score;}
		
		
		if ((turn==1 && score >beta) || (turn==-1 && score <alpha)){ // "beta-cutoff"
			
			if (doHashMoves){
				ht_hash[hash4moveOrder]=hash;
				ht_moveID[hash4moveOrder]=move& 0xffff; 
				hashstored++;
			}
			
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
		ht_moveID[hash4moveOrder]=moveList[bestMoveIndex]& 0xffff;
		hashstored++;
	}


	
	return bestScore;
}	

#endif