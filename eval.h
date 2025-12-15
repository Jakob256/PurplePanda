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

int ID(int board[8][8],unsigned int move){// side + piecetype + toSquare
	int c1=move&0b111;
	int r1=(move>>3)&0b111;
	int c2=(move>>6)&0b111;
	int r2=(move>>9)&0b111;
	
	int piece=board[c1][r1];
	int to=r2+8*c2;
	return to*13+piece+6;
}

int max_int(int a,int b){return (a+b+abs(a-b))/2;}

int eval(int board[8][8],unsigned long long int key, long long int hash, int atDepth, int depth2Go, int alpha, int beta, int maxDepth, int captures, int nrDubiousWhiteMoves, int nrDubiousBlackMoves){
	
	nodes++;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	
	/*****************************************************************
	***** First we examine, wether we need to return immediately *****
	*****************************************************************/
	
	int standingPat=stationaryEval(board,key);
	if (depth2Go==0){return standingPat;}
	if (atDepth==maxDepth){return standingPat;}
	if (atDepth>0 && nrPieces<=3){return standingPat;}
	
	if (atDepth<=3 && atDepth>0){ // check for repetition at shallow depth, but not at the root
		int howOftenOccured=0;
		for (int i=0;i<100;i++){
			if (hash==previous100PositionHashes[i]){howOftenOccured++;}
		}
		if (howOftenOccured>=2){return 0;} // occured already 2 times -> now 3 times
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
	
	
	bool weAreInCheck=(isKingInCheck(board,turn,0,0)!=0);
	int margins[4]={0,0,135,270};
	if (depth2Go>=1 && depth2Go<=3 && atDepth>=6 && !weAreInCheck){
		if ((turn==+1 && standingPat>= beta+margins[depth2Go])||(turn==-1 && standingPat<= alpha-margins[depth2Go])){
			return standingPat+turn;	
		}
	}
	
	/***************************
	***** Nullmove pruning *****
	***************************/
	
	int score;
	unsigned long long int newKeyy;
	long long int newHashh;
	
	if ((standingPat>beta+40 && turn==1)||(standingPat<alpha-40 && turn==-1)){
		if (depth2Go>3 && atDepth!=0 && !weAreInCheck){
		
			newKeyy=newKey(board,key,0);
			newHashh=newHash(board,key,hash,0);
			//using R=2
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1-2,alpha,beta,maxDepth-2,captures,0,0);
			
			if ((score>beta && turn==1)||(score<alpha && turn==-1)){
				return score;
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
	
	
	if (clock()>endTime){exitSearch=true;return 0;}	
	if (NODELIMIT!=-1 && nodes>=NODELIMIT){exitSearch=true;return 0;}
	unsigned int moveList[250];
	assignMoveListAndSort(board,key,moveList,true,thisMoveFirst);
	
	if (moveList[0]==0){ //there are no legal moves -> checkmate or stalemate
		if (weAreInCheck){ // we are in check
			return -turn*100000;
		} else {
			return 0;
		}
	}
	
	
	
	/*************************************
	***** Can the search be skipped? *****
	*************************************/

	// skipping agressive recaptures, when quiet moves suffice
	int quietPSTgain=int(moveList[moveList[0]+2])-10000;
	if (depth2Go==1 && atDepth>3 && abs(quietPSTgain)<200 && moveList[0]>1){ // we have a valid quietPSTgain
		if (turn== 1 && standingPat+quietPSTgain-30>beta ){return standingPat+quietPSTgain-30;}
		if (turn==-1 && standingPat-quietPSTgain+30<alpha){return standingPat-quietPSTgain+30;}
	}



	/************************
	***** The move loop *****
	************************/
	
	int bestScore=-INF*turn;
	int depth2GoNew,capturesNew;	
	bool aQuietMoveTried=false;
	unsigned int move,bestMove;
	int oldAlpha=alpha;
	int oldBeta=beta;
	

	int nrDubiousWhiteMovesNew,nrDubiousBlackMovesNew;
	bool dubiousMove,normalEvaluation;
	int moveID;
	
	for (int i=1; i<=moveList[0];i++){
		if (exitSearch){return 0;} // check if search has to be aborted.
		
		move=moveList[i];
		moveID=ID(board,move);
		
		// when depth2Go=1, we only want to try a single quiet move, that was highest ranked after moveordering
		// this prunes the tree and also returns a better estimate of the true evaluation
		// for instance, we won't move the queen to a very active square, where it can be captured by a pawn
		if (move<=4095){ // 7*(1+8+64+512)
			if (aQuietMoveTried && depth2Go==1){continue;}
			aQuietMoveTried=true;
		}
		
		// These lines reduce the number of nodes dramatically, but is mostly for aesthetic purposes
		// It skips moves that will most probably be killed immediately by frontier pruning
		if (depth2Go==2 && ((standingPat<alpha+30 && turn==1) || (standingPat>beta-30 && turn==-1)) 
			&& i>5 && move<=4095){continue;}
		
		if (depth2Go==2 && ((standingPat<alpha-30 && turn==1) || (standingPat>beta+30 && turn==-1)) 
			&& i>1 && move<=4095){continue;}
		
		
		if (move<4096 && i>15){ // late quiet move
			historyA[moveID]++;
			historyC[moveID]++;
			if (historyA[moveID]>=10 && historyB[moveID]==0 && historyC[moveID]>2){
				historyC[moveID]=0;
				continue; //reduce the move
			}
		}
		newKeyy=newKey(board,key,move);
		newHashh=newHash(board,key,hash,move);
		
		assignMakeMove(board,move);
		
		
		depth2GoNew=depth2Go-1+((depth2Go==1 && move>4095)|| weAreInCheck); // reduce by one, unless this was the final move and it's a capture, or we are in check
		capturesNew=captures+(move>4095);
		
		dubiousMove= ((i>6) && (move<4095));
		if (dubiousMove && depth2Go>3){// checks are never dubious
			if (isKingInCheck(board,-turn,0,0)!=0){dubiousMove=false;}
		}
		
		
		nrDubiousWhiteMovesNew=nrDubiousWhiteMoves+(dubiousMove && turn== 1);
		nrDubiousBlackMovesNew=nrDubiousBlackMoves+(dubiousMove && turn==-1);
		
		
		
		normalEvaluation=true;
		if (dubiousMove && depth2Go>3 && atDepth!=0 && (nrDubiousWhiteMovesNew==2 || nrDubiousBlackMovesNew==2)){
			// second dubious move -> late move reduction by two ply
			nrDubiousWhiteMovesNew=0;
			nrDubiousBlackMovesNew=0;
			
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2GoNew-2,alpha,beta,maxDepth-2,capturesNew,nrDubiousWhiteMovesNew,nrDubiousBlackMovesNew);
			normalEvaluation=((turn==1 && score>alpha)||(turn==-1 && score<beta)); //re-search needed
		}
		
		if (normalEvaluation){
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2GoNew,alpha,beta,maxDepth,capturesNew,nrDubiousWhiteMovesNew,nrDubiousBlackMovesNew);
		}
		
		assignUndoMove(board,move);
		
		
		/*************************************
		***** Alpha/Beta Updates/Cutoffs *****
		*************************************/
		
		
		if (turn== 1 && score >alpha){alpha=score;if (move<4096 && i>15){historyB[moveID]++;}}
		if (turn==-1 && score <beta ){beta =score;if (move<4096 && i>15){historyB[moveID]++;}}
		
		
		if ((turn==1 && score >beta) || (turn==-1 && score <alpha)){ // "beta-cutoff"
			if (move<4096 && depth2Go>=2 && i!=1){
				int bonus=1+(depth2Go>3)*4;
				//history gravity formula:
				moveHistoryScore[move+4096*(turn==1)]+=bonus-(moveHistoryScore[move+4096*(turn==1)]*bonus)/100;
				for (int j=1;j<i; j++){
					if (moveList[j]<4096){
						moveHistoryScore[moveList[j]+4096*(turn==1)]=max_int(moveHistoryScore[moveList[j]+4096*(turn==1)]-1,-100);
					}
				}
			}
			
			
			
			if (i>5){killerMoves[atDepth]=move;}
			if (doHashMoves && !exitSearch){
				ht_hash[hash4moveOrder]=hash;
				ht_moveShort[hash4moveOrder]=move& 0xffff; 
				hashstored++;
			}
			return score;
		}
		
		
	
		if (score*turn>bestScore*turn){
			bestScore=score;
			bestMove=move;
			if (atDepth==0 && !exitSearch){
				rootBestMove=move;
			}
		}
		
		// move killer move to position 6:
		if (i==5 && moveList[0]>7 && depth2Go>1){
			for (int j=7;j<=moveList[0];j++){
				if (moveList[j]==killerMoves[atDepth]){					
					for (int k=j; k>=7; k--){
						moveList[k]=moveList[k-1];
					}
					moveList[6]=killerMoves[atDepth];
					break;
				}
			}
		}
	}
	
	

	/**************************************************************
	***** Return our findings if there were no "beta-cutoffs" *****
	**************************************************************/

	bool failLow=(turn==1 && bestScore<=oldAlpha)||(turn==-1 && bestScore>=oldBeta);

	// store best move if no cutoff occured
	if (doHashMoves && !exitSearch && !failLow){
		ht_hash[hash4moveOrder]=hash;
		ht_moveShort[hash4moveOrder]=bestMove& 0xffff;
		hashstored++;
	}

	return bestScore;
}	

#endif