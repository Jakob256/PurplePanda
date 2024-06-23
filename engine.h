/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"
#include "eval.h"
#include "printingStuff.h"
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

	/*************************************
	***** resetting counting numbers *****
	*************************************/

	staticeval=0;
	nodes=0;
	movegen=0;
	hashstored=0;
	hashhits=0;
	
	/*****************************
	***** consult the oracle *****
	*****************************/

	oracle(board,key);
	key=setBonusOfKey(board,key);
	
	/****************************************
	***** prepare/tablebase stuff/abort *****
	****************************************/
	
	// prepare default move:
	unsigned int moveList[250];
	assignMoveListAndSort(board,key,moveList,true);
	rootBestMove=moveList[1];
	
	
	// in case of only one legal move:
	if (moveList[0]==1){
		cout << "bestmove ";
		printMove(rootBestMove);
		cout << "\n";
		return;
	} 
	
	// in case we are in the 3men tablebase:
	if (nrPieces<=3){
		float bestScore=-INF*turn;
		float score;
		unsigned long long int newKeyy;
		unsigned int move;
		for (int i=1; i<=moveList[0];i++){
			move=moveList[i];
			newKeyy=newKey(board,key,move);
			
			assignMakeMove(board,move);
			score=stationaryEval(board,newKeyy);
			assignUndoMove(board,move);
			
			if (score*turn>bestScore*turn){
				bestScore=score;
				rootBestMove=move;
			}
		}
		
		cout << "bestmove ";
		printMove(rootBestMove);
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
		maxDepth=depth2go+4;
		if (clock()<endTime){
			evaluation=eval(board,key,hashFunction(board,key),0,depth2go,-INF,+INF,endTime,maxDepth);
			if (clock()<endTime){
				cout <<
				"info depth "<< depth2go<<
				" score cp " << (int)(evaluation*100*turn) << 
				" nodes "<< nodes<< 
				" time "<<(clock()- startTime)*1000/CLOCKS_PER_SEC <<
				" hashhits "<<hashhits<<
				" hashstored "<<hashstored<<
				" pv "; 
				printMove(rootBestMove);
				cout << std::endl;
			}
		}
		
		if (abs(evaluation)==1000){break;} // break immediately if there is a mate evaluation
	}
	
	

	/***************************
	***** output best move *****
	***************************/

	cout << "\nnodes: "<<nodes<< "\n"; 
	cout << "staticeval: "<<staticeval<< "\n";
	cout << "movegen: "<<movegen<< "\n";
	
	cout << "bestmove ";
	printMove(rootBestMove);
	cout << "\n";
}	

#endif