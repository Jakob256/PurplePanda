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
	int startTime=clock();
	int endTime=startTime+time;
	float evaluation=0;

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
	
	/*********************************
	***** start extensive search *****
	*********************************/
	
	int moveList[250*5];
	assignMoveListAndSort(board,key,moveList,true);
	for (int i=0;i<5;i++){
		globalBestMove[i]=moveList[i+1];
	}
	
	
	for (int depth2go=2; depth2go<=20; depth2go++){	
		if (depth2go>inputDepth){break;}
		globalMaxDepth=8;
		if (depth2go>=7){globalMaxDepth=depth2go+2;}
		if (clock()<endTime){
			evaluation=eval(board,key,hashFunction(board,key),0,depth2go,-INF,+INF,endTime);
			if (clock()<endTime){
				
				cout << "info depth "<< depth2go<<" score cp " << (int)(evaluation*100*turn) << " nodes "<< countingStationaryEvalCalled<< " pv ";
				printMove(globalBestMove[0],globalBestMove[1],globalBestMove[2],globalBestMove[3],globalBestMove[4]);
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
	printMove(globalBestMove[0],globalBestMove[1],globalBestMove[2],globalBestMove[3],globalBestMove[4]);
	cout << "\n";
}	

#endif