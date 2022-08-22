/**************************
*** function depends on ***
**************************/

#include "stationaryEvalV1.h"
#include "assignMoveList.h"
#include "assignSortMovesV1.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "hashFunction.h"
#include "newKey.h"
#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef evalV1FILE
#define evalV1FILE

/***************
*** Let's go ***
***************/

float evalV1 (int board[8][8],unsigned long long int key, int atDepth, int depth2Go, float alpha, float beta, int endTime ){
	/*
	if (key > 1125899906842624 && atDepth==2){
		//plotBoard(board);
		cout << "Key at evddal: "<<key <<" = "<< bitset<64>(key)<<"\n";
	} else {
		cout << "good\n";
	}
	*/
	
	countingEvalCalled+=1;
	int turn;
	turn=-1+2*(key%2);
	
	/****************************************************************
	***** First we examine, wether we need to return imediately *****
	****************************************************************/
	
	if (depth2Go==0){
		return stationaryEvalV1(board,key) + (rand() % 100)/1000000.;
	}
	if (atDepth==globalMaxDepth){
		return stationaryEvalV1(board,key) + (rand() % 100)/1000000.;
	}
	
	
	if (atDepth<=3 && atDepth>0){ // second part to make sure it will return some value... like, you have to do something and cant just stop
		int HASH=hashFunction8(board,key);
		int howOftenOccured=0;
		for (int i=0;i<100;i++){
			if (HASH==previous100PositionHashes[i]){howOftenOccured++;}
		}
		if (howOftenOccured>=2){return 0.;} // occured already 2 times -> now 3 times
	}
	
	
	/****************************************************
	***** Obvious Alpha/Beta Cutoffs at depth2Go==1 *****
	****************************************************/
	
	/*
	if (depth2Go==1 && moveOpponentToEdge==0){ // if opponent move to edge, the gain in eval can be larger than 0.5; so we cannot allow this method (?)
		int currentEval;
		currentEval=int((key>>15)&255)-128;
		if (turn==1 && currentEval>beta+0.5){return currentEval;}
		if (turn==-1 && currentEval<alpha-0.5){return currentEval;}
	}
	*/
	
	/******************************************************
	***** Now we prepare the "legal moves" to examine *****
	******************************************************/

	
	float bestScore=-INF*turn;
	int bestMoveIndex=0;	
	int moveList[250*5];
	
	// generate Legal moves
	if (atDepth<3 || oracleStrongLegalMoves){
		assignMoveList(board,key,moveList,true);
		if (moveList[0]==0){ //there is a final evaluation
			//cout << "There are no legal moves\n";
			if (isKingInCheck(board,turn)){
				//cout << "There is a mate!!\n";
				return -turn*1000;
			} else {
				return 0;
			}
		}
	} else {
		assignMoveList(board,key,moveList,false);
		// check if I can capture opponents king:
		for (int i=1; i<=moveList[0];i++){
			if (abs(board[moveList[5*i-2]][moveList[5*i-1]])==6){
				return 1000*turn;
			}
		}
	}
	
	/*************************************************
	***** Do move ordering for alpha-beta search *****
	*************************************************/

	if (atDepth<=4){assignSortMovesV1(moveList,board);}
	
	/**************************************************************
	***** Examine, wether there is a policy in the hash table *****
	**************************************************************/
	
	//int h=hashFunction(board,turn);
	//short int p=hashPolicyTable[h];
	//bool hashPolicyFound=true;
	//if (p==0){hashPolicyFound=false;}
	//if (p>moveList[0]){hashPolicyFound=false;}
	
		
	
	
	/*******************************
	***** Do the actual search *****
	*******************************/


	
	float score;
	int i;
	unsigned long long int newKeyy;
	
	for (int ii=1; ii<=moveList[0];ii++){
		/* info: the element ii=0 is only used if a policyHash was found. Then i is mapped to that move*/
		// if this gets reactivated, ii must start from 0
		i=ii;
		//if (ii==0){
		//	if (hashPolicyFound){
		//		i=p;
		//	} else {
		//		continue;
		//	}
		//}

		
		if (clock()>endTime){return 0;} // check if search has to be abborted.
		

		newKeyy=newKey(board,key,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		if (moveList[5*i]==0){ // no capture or anything
			score=evalV1(board,newKeyy,atDepth+1,depth2Go-1,alpha,beta,endTime);
			
		} else {
			if (depth2Go>1){
				score=evalV1(board,newKeyy,atDepth+1,depth2Go-1,alpha,beta,endTime);
			} else {
				score=evalV1(board,newKeyy,atDepth+1,1,alpha,beta,endTime);
			}	
		}
		
		assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		
		
		if (turn==1 && score >alpha){alpha=score;}
		if (turn==1 && score >beta){return score;}
			
		if (turn==-1 && score <beta){beta=score;}
		if (turn==-1 && score <alpha){return score;}
		
		/*
		if (turn==1 && score >alpha){alpha=score;}
		if (turn==1 && score >beta){hashPolicyTable[h]=i; return score;}
			
		if (turn==-1 && score <beta){beta=score;}
		if (turn==-1 && score <alpha){hashPolicyTable[h]=i; return score;}		
		*/
	
		if (score*turn>bestScore*turn){
			bestMoveIndex=i;
			bestScore=score;
		}
	}
	
	/******************************************************************
	***** Store the findings of the completed search in hashTable *****
	******************************************************************/
	
	//hashPolicyTable[h]=bestMoveIndex;
	
	
	/*****************************************************************************
	***** If we are atDepth=0, we need to store globally the best move found *****
	*****************************************************************************/
	
	
	if (atDepth==0){
		globalBestMove[0]=moveList[5*bestMoveIndex-4];
		globalBestMove[1]=moveList[5*bestMoveIndex-3];
		globalBestMove[2]=moveList[5*bestMoveIndex-2];
		globalBestMove[3]=moveList[5*bestMoveIndex-1];
		globalBestMove[4]=moveList[5*bestMoveIndex-0];
	}
	
	/******************************
	***** Return our findings *****
	******************************/
	
	return bestScore;
}	

#endif