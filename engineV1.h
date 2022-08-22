/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"
#include "evalV1.h"
#include "printMoves.h"
#include "preSearchCalculationsV1.h"
#include "oracleV1.h"
#include "oracleSetPST_V1.h"


/************
*** Guard ***
************/

#ifndef engineV1FILE
#define engineV1FILE

/***************
*** Let's go ***
***************/


void engineV1 (int board[8][8],unsigned long long int key, int time){
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


	for (int depth2go=2; depth2go<=20; depth2go++){	
		globalMaxDepth=8;
		if (depth2go>=8){globalMaxDepth=depth2go+2;}
		if (abs(eval)==1000){break;} // break immediately if there is a mate evaluation
		if (clock()<endTime){
			eval=evalV1(board,key,0,depth2go,-INF,+INF,endTime);
			if (clock()<endTime){
				// evaluation was completed (aka not aborted)
				
				globalBestMoveCompletedMove[0]=globalBestMove[0];
				globalBestMoveCompletedMove[1]=globalBestMove[1];
				globalBestMoveCompletedMove[2]=globalBestMove[2];
				globalBestMoveCompletedMove[3]=globalBestMove[3];
				globalBestMoveCompletedMove[4]=globalBestMove[4];
				
				
				cout << "info depth "<< 10*depth2go<<" score cp " << (int)(eval*100*turn) << " nodes "<< countingStationaryEvalCalled<< " pv ";
				printMove(globalBestMove[0],globalBestMove[1],globalBestMove[2],globalBestMove[3],globalBestMove[4]);
				cout << "\n";
				if (abs(eval)==1000){break;} // a direct mate found
			}
		}
	}
	
	

	/***************************
	***** output best move *****
	***************************/

	cout << "countingStationaryEvalCalled: "<<countingStationaryEvalCalled<< "\n"; 
	cout << "countingEvalCalled: "<<countingEvalCalled<< "\n";
	cout << "countingMoveGenerationCalled: "<<countingMoveGenerationCalled<< "\n";

	
	cout << "bestmove ";
	printMove(globalBestMoveCompletedMove[0],globalBestMoveCompletedMove[1],globalBestMoveCompletedMove[2],globalBestMoveCompletedMove[3],globalBestMoveCompletedMove[4]);
	cout << "\n";
}	

#endif