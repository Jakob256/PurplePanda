/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"
#include "evalV1.h"
#include "printMoves.h"
#include "preSearchCalculationsV1.h"
#include "oracleV1.h"
#include "oracleSetPST_V1.h"
#include "assignMoveList.h"


/************
*** Guard ***
************/

#ifndef engineV1FILE
#define engineV1FILE

/***************
*** Let's go ***
***************/


void engineV1 (int board[8][8],unsigned long long int key, int time, int inputDepth){
	/********************************************
	**** tracking time and setting variables ****
	********************************************/
	int turn;
	turn=-1+2*(key%2);
	int startTime=clock();
	int endTime=startTime+time;
	float eval=0;
	bool searchCompleted=false;

	/************************************
	***** reseting counting numbers *****
	************************************/

	countingStationaryEvalCalled=0;
	countingEvalCalled=0;
	countingMoveGenerationCalled=0;
	countingHashesStored=0;
	
	/***************************************
	***** do preProcessing of position *****
	***************************************/

	
	preSearchCalculationsV1(board,key);
	oracleV1(board,key);
	oracleSetPST_V1(board,key);
	
	//cout << "key when entering engine after oracle shit: "<<key <<" = "<< bitset<64>(key)<<"\n";

	/*********************************
	***** start extensive search *****
	*********************************/
	
	int moveList[250*5];
	assignMoveList(board,key,moveList);
	for (int i=0;i<5;i++){
		globalBestMove[i]=moveList[i+1];
	}
	
	
	for (int depth2go=2; depth2go<=20; depth2go++){	
		if (depth2go>inputDepth){break;}
		globalMaxDepth=8;
		if (depth2go>=8){globalMaxDepth=depth2go+2;}
		if (abs(eval)==1000){break;} // break immediately if there is a mate evaluation
		if (clock()<endTime){
			eval=evalV1(board,key,hashFunction(board,key),0,depth2go,-INF,+INF,endTime);
			if (clock()<endTime){
				
				cout << "info depth "<< depth2go<<" score cp " << (int)(eval*100*turn) << " nodes "<< countingStationaryEvalCalled<< " pv ";
				printMove(globalBestMove[0],globalBestMove[1],globalBestMove[2],globalBestMove[3],globalBestMove[4]);
				cout << "\n";
				if (abs(eval)==1000){break;} // a direct mate found
			}
		}
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