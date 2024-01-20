/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"
#include "eval.h"
#include "printMoves.h"
#include "oracle.h"
#include "setBonusOfKey.h"
#include "assignMoveListAndSort.h"


/************
*** Guard ***
************/

#ifndef engineFILE
#define engineFILE

/***************
*** Let's go ***
***************/


void engine (int board[8][8],unsigned long long int key, int time, int inputDepth){
	/********************************************
	**** tracking time and setting variables ****
	********************************************/
	
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	int startTime=clock();
	time=time*CLOCKS_PER_SEC/1000;
	int endTime=startTime+time;

	/************************************
	***** reseting counting numbers *****
	************************************/

	countingStationaryEvalCalled=0;
	countingEvalCalled=0;
	countingMoveGenerationCalled=0;
	countingHashesStored=0;
	
	/*****************************
	***** consult the oracle *****
	*****************************/

	oracle(board,key);
	key=setBonusOfKey(board,key);
	
	/****************************************
	***** prepare/tablebase stuff/abort *****
	****************************************/
	
	// prepare default move:
	int moveList[250*5];
	assignMoveListAndSort(board,key,moveList,true);
	for (int i=0;i<5;i++){
		rootBestMove[i]=moveList[i+1];
	}
	
	// in case of only one legal move:
	if (moveList[0]==1){
		cout << "bestmove ";
		printMove(rootBestMove[0],rootBestMove[1],rootBestMove[2],rootBestMove[3],rootBestMove[4]);
		cout << "\n";
		return;
	} 
	
	// in case we are in the 3men tablebase:
	if (nrPieces<=3){
		float bestScore=-INF*turn;
		float score;
		unsigned long long int newKeyy;
		
		for (int i=1; i<=moveList[0];i++){
			newKeyy=newKey(board,key,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
			
			assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
			score=stationaryEval(board,newKeyy);
			assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
			
			if (score*turn>bestScore*turn){
				bestScore=score;
				rootBestMove[0]=moveList[5*i-4];
				rootBestMove[1]=moveList[5*i-3];
				rootBestMove[2]=moveList[5*i-2];
				rootBestMove[3]=moveList[5*i-1];
				rootBestMove[4]=moveList[5*i];
			}
		}
		
		cout << "bestmove ";
		printMove(rootBestMove[0],rootBestMove[1],rootBestMove[2],rootBestMove[3],rootBestMove[4]);
		cout << "\n";
		return;
	}
	
	
	/*********************************
	***** start extensive search *****
	*********************************/
	
	int maxDepth;
	float evaluation;
	
	for (int depth2go=2; depth2go<=20; depth2go++){	
		if (depth2go>inputDepth){break;}
		maxDepth=8;
		if (depth2go>=7){maxDepth=depth2go+2;}
		if (clock()<endTime){
			evaluation=eval(board,key,hashFunction(board,key),0,depth2go,-INF,+INF,endTime,maxDepth);
			if (clock()<endTime){
				
				cout << "info depth "<< depth2go<<" score cp " << (int)(evaluation*100*turn) << " nodes "<< countingStationaryEvalCalled<< " pv ";
				printMove(rootBestMove[0],rootBestMove[1],rootBestMove[2],rootBestMove[3],rootBestMove[4]);
				cout << "\n";
			}
		}
		
		if (abs(evaluation)==1000){break;} // break immediately if there is a mate evaluation
	}
	
	

	/***************************
	***** output best move *****
	***************************/

	cout << "\ncountingStationaryEvalCalled: "<<countingStationaryEvalCalled<< "\n"; 
	cout << "countingEvalCalled: "<<countingEvalCalled<< "\n";
	cout << "countingMoveGenerationCalled: "<<countingMoveGenerationCalled<< "\n";
	cout << "countingHashesStored: "<<countingHashesStored<< "\n";
	
	
	cout << "bestmove ";
	printMove(rootBestMove[0],rootBestMove[1],rootBestMove[2],rootBestMove[3],rootBestMove[4]);
	cout << "\n";
}	

#endif