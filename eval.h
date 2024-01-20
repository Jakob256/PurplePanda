/**************************
*** function depends on ***
**************************/

#include "stationaryEval.h"
#include "assignMoveListAndSort.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "newKey.h"
#include "globalVariables.h"
#include "moveID.h"

/************
*** Guard ***
************/

#ifndef evalFILE
#define evalFILE

/***************
*** Let's go ***
***************/

float eval(int board[8][8],unsigned long long int key, long long int hash, int atDepth, int depth2Go, float alpha, float beta, int endTime, int maxDepth){
	
	countingEvalCalled+=1;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	
	/****************************************************************
	***** First we examine, wether we need to return imediately *****
	****************************************************************/
	
	if (depth2Go==0 || atDepth==maxDepth){
		return stationaryEval(board,key) + (rand() % 100)/1000000.*randomness;
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
	
	float standingPat;
	if ((depth2Go==1) && (atDepth>=6) && (moveOpponentToEdge==0)){
		standingPat=stationaryEval(board,key);
		if ((turn==+1 && standingPat>= beta)||(turn==-1 && standingPat<= alpha)){
			if (isKingInCheck(board,turn,0,0)==0){
				return standingPat;
			}
		}
	}
	// it seems cutting off only is beneficial, if I don't risk getting to the same position with iterative deepening again but then don't have a lookup ready
	
	
	
	/**********************************************
	***** Now we prepare the moves to examine *****
	**********************************************/
	
	
	int moveList[250*5];
	assignMoveListAndSort(board,key,moveList,true); // sorting moves
	
	
	//if (moveList[moveList[0]*5+1]==0 &&  isKingInCheck(board,turn,0,0)){cout << "ono in eval\n";}
	//if (moveList[moveList[0]*5+1]!=0 && !isKingInCheck(board,turn,0,0)){cout << "ono in eval\n"}
	
	if (moveList[0]==0){ //there is a final evaluation
		if (moveList[moveList[0]*5+1]==1){ // we are in check
			return -turn*1000;
		} else {
			return 0;
		}
	}
	
	

	
	
	/*************************************************
	***** Do move ordering for alpha-beta search *****
	*************************************************/

	if (atDepth==0){  // move the root best move to first position
		for (int i=1; i<=moveList[0];i++){
			if (moveList[5*i-4]==rootBestMove[0] && moveList[5*i-3]==rootBestMove[1] && moveList[5*i-2]==rootBestMove[2] && moveList[5*i-1]==rootBestMove[3] && moveList[5*i]==rootBestMove[4]){
				moveList[5*i-4]=moveList[1];moveList[5*i-3]=moveList[2];moveList[5*i-2]=moveList[3];moveList[5*i-1]=moveList[4];moveList[5*i]=moveList[5];
				moveList[1]=rootBestMove[0];moveList[2]=rootBestMove[1];moveList[3]=rootBestMove[2];moveList[4]=rootBestMove[3];moveList[5]=rootBestMove[4];
			}
		}
	}
	
	
	bool eligibleForHashMoveOrdering=false;
	
	if (atDepth<=5 && atDepth>0){
		eligibleForHashMoveOrdering=true;
	}
	
	int hash4moveOrder,hv;
	long long int hashOfTable;
	short int moveIDD;
	
	if (eligibleForHashMoveOrdering){
		countingHashesStored++;
		//cout << "we are at depth "<< atDepth; // for verification
		hash4moveOrder=int(abs(hash%hashMoveOrderingTableSize));
		hashOfTable=hashMoveOrderingTable_hash[hash4moveOrder];
		moveIDD=hashMoveOrderingTable_moveID[hash4moveOrder];
		if (hashOfTable==hash){// we have the exact same hash stored!
			
			// here we could check if we sould simply play a quiet move
			for (int i=1;i<=moveList[0];i++){
				if (moveID(moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i])==moveIDD){
					for (int j=1;j<=5;j++){
						hv=moveList[j];
						moveList[j]=moveList[5*i-5+j];
						moveList[5*i-5+j]=hv;
					}
					break;
				}
				//if (i==moveList[0]){cout << "strange... the move was not found... \n";}
			}
		}
		
	}
	
	
	/*******************************
	***** Do the actual search *****
	*******************************/


	float bestScore=-INF*turn;
	float score;
	unsigned long long int newKeyy;
	long long int newHashh;
	int bestMoveIndex;
	//if (atDepth==0){printMoveList(moveList);}// for admiring the root moves updated order

	
	

	for (int i=1; i<=moveList[0];i++){
		if (clock()>endTime){return 0;} // check if search has to be aborted.
		

		newKeyy=newKey(board,key,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		newHashh=newHash(board,key,hash,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		
		if (moveList[5*i]==0){ // no capture or anything
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime,maxDepth);
			
		} else {
			if (depth2Go>1){
				score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime,maxDepth);
			} else {
				score=eval(board,newKeyy,newHashh,atDepth+1,1,alpha,beta,endTime,maxDepth);
			}	
		}
		
		assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		
		/**********************************************************************************
		***** Alpha/Beta Updates (unfortunatly long code because of HashTableUpdates) *****
		**********************************************************************************/
		
		
		if (turn==1 && score >alpha){alpha=score;}
		if (turn==-1 && score <beta){beta=score;}
		
		
		if ((turn==1 && score >beta) || (turn==-1 && score <alpha)){ // "beta-cutoff"
		
			if (eligibleForHashMoveOrdering){
				hashMoveOrderingTable_hash[hash4moveOrder]=hash;
				hashMoveOrderingTable_moveID[hash4moveOrder]=moveID(moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			}
			
			return score;
		}
		
		
	
		if (score*turn>bestScore*turn){
			bestScore=score;
			if (atDepth==0 && clock()<endTime){
				rootBestMove[0]=moveList[5*i-4];
				rootBestMove[1]=moveList[5*i-3];
				rootBestMove[2]=moveList[5*i-2];
				rootBestMove[3]=moveList[5*i-1];
				rootBestMove[4]=moveList[5*i];
			}
			bestMoveIndex=i;
		}
		
	}
	
	

	/**************************************************************
	***** Return our findings if there were no "beta-cutoffs" *****
	**************************************************************/
	
	// store best move if no cutoff occured
	if (eligibleForHashMoveOrdering){
		hashMoveOrderingTable_hash[hash4moveOrder]=hash;
		hashMoveOrderingTable_moveID[hash4moveOrder]=moveID(moveList[5*bestMoveIndex-4],moveList[5*bestMoveIndex-3],moveList[5*bestMoveIndex-2],moveList[5*bestMoveIndex-1],moveList[5*bestMoveIndex]);
	}


	
	return bestScore;
}	

#endif