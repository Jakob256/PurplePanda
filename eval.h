/**************************
*** function depends on ***
**************************/

#include "stationaryEval.h"
#include "assignMoveListAndSort.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "hashFunction.h"
#include "newKey.h"
#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef evalFILE
#define evalFILE

/***************
*** Let's go ***
***************/

float eval(int board[8][8],unsigned long long int key, long long int hash, int atDepth, int depth2Go, float alpha, float beta, int endTime ){
	
	countingEvalCalled+=1;
	int turn;
	turn=-1+2*(key%2);
	
	/****************************************************************
	***** First we examine, wether we need to return imediately *****
	****************************************************************/
	
	if (depth2Go==0){
		return stationaryEval(board,key) + (rand() % 100)/1000000.*randomness;
	}
	if (atDepth==globalMaxDepth){
		return stationaryEval(board,key) + (rand() % 100)/1000000.*randomness;
	}
	
	
	if (atDepth<=3 && atDepth>0){ // second part to make sure it will return some value... like, you have to do something and cant just stop
		int howOftenOccured=0;
		for (int i=0;i<100;i++){
			if (hash==previous100PositionHashes[i]){howOftenOccured++;}
		}
		if (howOftenOccured>=2){return 0.;} // occured already 2 times -> now 3 times
	}
	
		
	/**********************************************
	***** Now we prepare the moves to examine *****
	**********************************************/

	
	float bestScore=-INF*turn;
	int moveList[250*5];
	
	// generate moves
	
	if (atDepth<=5){
		assignMoveListAndSort(board,key,moveList,true); // sorting moves
	} else {
		assignMoveListAndSort(board,key,moveList,false);
	}
	
	
	if (moveList[0]==0){ //there is a final evaluation
		if (isKingInCheck(board,turn,0,0)!=0){
			return -turn*1000;
		} else {
			return 0;
		}
	}

	
	/*************************************************
	***** Do move ordering for alpha-beta search *****
	*************************************************/

	if (atDepth==0){
		for (int i=1; i<=moveList[0];i++){
			if (moveList[5*i-4]==globalBestMove[0] && 
			moveList[5*i-3]==globalBestMove[1] && 
			moveList[5*i-2]==globalBestMove[2] && 
			moveList[5*i-1]==globalBestMove[3] && 
			moveList[5*i]==globalBestMove[4]){
				moveList[5*i-4]=moveList[1];  // move the global best move to first position
				moveList[5*i-3]=moveList[2];  // move the global best move to first position
				moveList[5*i-2]=moveList[3];  // move the global best move to first position
				moveList[5*i-1]=moveList[4];  // move the global best move to first position
				moveList[5*i  ]=moveList[5];  // move the global best move to first position
				moveList[1]=globalBestMove[0];
				moveList[2]=globalBestMove[1];
				moveList[3]=globalBestMove[2];
				moveList[4]=globalBestMove[3];
				moveList[5]=globalBestMove[4];
			}
		}
	}
	
	bool eligibleForHashMoveOrdering=false;
	
	if (atDepth<= 5 && atDepth>0){
		eligibleForHashMoveOrdering=true;
	}
	int hash4moveOrder,hashOfMove,posOfMove,hv;
	bool switched=false;
	int switchedWith=-1;

	
	
	if (eligibleForHashMoveOrdering){
		countingHashesStored++;
		//cout << "we are at depth "<< atDepth; // for verification
		hash4moveOrder=int(abs(hashFunction(board,key)%hashMoveOrderingTableSize));
		hashOfMove=hashMoveOrderingTable[hash4moveOrder][0];
		posOfMove=hashMoveOrderingTable[hash4moveOrder][1];
		if (posOfMove<=moveList[0] && posOfMove>1){ //position of move exists & is not already the first move
			if (moveList[5*posOfMove-4]+moveList[5*posOfMove-3]*20+moveList[5*posOfMove-2]*400+moveList[5*posOfMove-1]*8000+moveList[5*posOfMove]*160000==hashOfMove){ //there is the expected move
				//cout << " and we have a hit"; // for verification
				switched=true;
				switchedWith=posOfMove;
				
				for (int i=1;i<=5;i++){
					hv=moveList[i];
					moveList[i]=moveList[5*posOfMove-5+i];
					moveList[5*posOfMove-5+i]=hv;
				}
			}
		}
		/*if (posOfMove==1){ // for verification
			if (moveList[5*posOfMove-4]+moveList[5*posOfMove-3]*20+moveList[5*posOfMove-2]*400+moveList[5*posOfMove-1]*8000+moveList[5*posOfMove]*160000==hashOfMove){ //there is the expected move
				cout << " and we have a hit at position 1";
			}
		}
		cout <<"\n"; // for verification */
		
	}
	
	
	/*******************************
	***** Do the actual search *****
	*******************************/


	
	float score;
	unsigned long long int newKeyy;
	long long int newHashh;
	
	//if (atDepth==0){printMoveList(moveList);}// for admiring the root moves updated order

	for (int i=1; i<=moveList[0];i++){
		if (clock()>endTime){return 0;} // check if search has to be abborted.
		

		newKeyy=newKey(board,key,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		newHashh=newHash(board,key,hash,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		if (moveList[5*i]==0){ // no capture or anything
			score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime);
			
		} else {
			if (depth2Go>1){
				score=eval(board,newKeyy,newHashh,atDepth+1,depth2Go-1,alpha,beta,endTime);
			} else {
				score=eval(board,newKeyy,newHashh,atDepth+1,1,alpha,beta,endTime);
			}	
		}
		
		assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
		
		
		/**********************************************************************************
		***** Alpha/Beta Updates (unfortunatly long code because of HashTableUpdates) *****
		**********************************************************************************/
		
		
		if (turn==1 && score >alpha){alpha=score;}
		if (turn==1 && score >beta){
			if (eligibleForHashMoveOrdering){
				hashMoveOrderingTable[hash4moveOrder][0]=moveList[5*i-4]+moveList[5*i-3]*20+moveList[5*i-2]*400+moveList[5*i-1]*8000+moveList[5*i]*160000;
				if (switched==false){hashMoveOrderingTable[hash4moveOrder][1]=i;} 
				else {
					if (i==1){hashMoveOrderingTable[hash4moveOrder][1]=switchedWith;}
					else if (i==switchedWith){hashMoveOrderingTable[hash4moveOrder][1]=1;}
					else {hashMoveOrderingTable[hash4moveOrder][1]=i;}
				}
			}
			return score;
		}
			
		if (turn==-1 && score <beta){beta=score;}
		if (turn==-1 && score <alpha){
			if (eligibleForHashMoveOrdering){
				hashMoveOrderingTable[hash4moveOrder][0]=moveList[5*i-4]+moveList[5*i-3]*20+moveList[5*i-2]*400+moveList[5*i-1]*8000+moveList[5*i]*160000;
				if (switched==false){hashMoveOrderingTable[hash4moveOrder][1]=i;}
				else {
					if (i==1){hashMoveOrderingTable[hash4moveOrder][1]=switchedWith;}
					else if (i==switchedWith){hashMoveOrderingTable[hash4moveOrder][1]=1;}
					else {hashMoveOrderingTable[hash4moveOrder][1]=i;}
				}
			}
			return score;
		}
		
	
		if (score*turn>bestScore*turn){
			bestScore=score;
			if (atDepth==0 && clock()<endTime){
				globalBestMove[0]=moveList[5*i-4];
				globalBestMove[1]=moveList[5*i-3];
				globalBestMove[2]=moveList[5*i-2];
				globalBestMove[3]=moveList[5*i-1];
				globalBestMove[4]=moveList[5*i];
				
				//cout << "best Move updated to ";
				//printMove(globalBestMove[0],globalBestMove[1],globalBestMove[2],globalBestMove[3],globalBestMove[4]);
				//cout <<"\n\n";
			}
			if (eligibleForHashMoveOrdering){
				hashMoveOrderingTable[hash4moveOrder][0]=moveList[5*i-4]+moveList[5*i-3]*20+moveList[5*i-2]*400+moveList[5*i-1]*8000+moveList[5*i]*160000;
				if (switched==false){hashMoveOrderingTable[hash4moveOrder][1]=i;}
				else {
					if (i==1){hashMoveOrderingTable[hash4moveOrder][1]=switchedWith;}
					else if (i==switchedWith){hashMoveOrderingTable[hash4moveOrder][1]=1;}
					else {hashMoveOrderingTable[hash4moveOrder][1]=i;}
				}
			}
		}
	}
	
	

	/******************************
	***** Return our findings *****
	******************************/
	
	return bestScore;
}	

#endif