/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"
#include "eval.h"
#include "printingStuff.h"
#include "oracle.h"
#include "setBonusOfKey.h"
#include "assignMoveListAndSort.h"
#include "randomOpening.h"


/************
*** Guard ***
************/

#ifndef engineFILE
#define engineFILE

/***************
*** Let's go ***
***************/


void engine(int board[8][8],unsigned long long int key, int time, int inputDepth, int nodeLimit){
	/********************************************
	**** tracking time and setting variables ****
	********************************************/
	
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	int startTime=clock();
	time=time*CLOCKS_PER_SEC/1000;
	endTime=startTime+time;
	NODELIMIT=nodeLimit;
	
	/*************************************
	***** resetting counting numbers *****
	*************************************/

	staticeval=0;
	nodes=0;
	movegen=0;
	hashstored=0;
	hashhits=0;
	for (int i=0;i<100;i++){killerMoves[i]=0;}
	for (int i=0;i<8192;i++){moveHistoryScore[i]=0;}
	for (int i=0;i<4096;i++){
		historyA[i]=0;
		historyB[i]=0;
		historyC[i]=0;
	}
	
	/*****************************
	***** consult the oracle *****
	*****************************/

	oracle(board,key);
	key=setBonusOfKey(board,key);
	
	/****************************************
	***** prepare/tablebase stuff/abort *****
	****************************************/
	
	
	if (UCI_OPTION_OPENINGBOOK){
		if (randomOpening(board,key)){return;}
	}
	
	
	
	// prepare default move:
	unsigned int moveList[250];
	assignMoveListAndSort(board,key,moveList,true);
	rootBestMove=moveList[1];
	
	
	// in case of only one legal move:
	if (moveList[0]==1){
		cout << "bestmove " << move2string(rootBestMove) << "\n";
		return;
	} 
	
	// in case we are in the 3men tablebase:
	if (nrPieces<=3){
		int score,bestScore=-INF*turn;
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
		
		cout << "bestmove " << move2string(rootBestMove) << "\n";
		return;
	}
	
	
	// if we have queen vs queen, rook vs rook or minor vs minor, we can reduce the search depth
	int phase24=int((key>>23)&63);
	int naiveEval=int((key>>15)&255)-128;
	if (nrPieces==4 && naiveEval==0 && phase24>0){inputDepth=8;}
	
	
	/*********************************
	***** start extensive search *****
	*********************************/
	
	int maxDepth,evaluation,hash4moveOrder,tempBoard[8][8],moveShort;
	exitSearch=false;
	long long int hash,hashOfTable;
	unsigned long long int tempKey;
	unsigned int move;
	
	for (int depth2go=2; depth2go<=30; depth2go++){	
		if (exitSearch){break;}
		if (depth2go>inputDepth){break;}
		
		maxDepth=depth2go+4;
		evaluation=eval(board,key,hashFunction(board,key),0,depth2go,-INF,+INF,maxDepth,0,0,0);
		
		if (exitSearch){break;}
		
		cout <<
		"info depth "<< depth2go<<
		" score cp " << evaluation*turn << 
		" nodes "<< nodes<< 
		" time "<<(clock()- startTime)*1000/CLOCKS_PER_SEC <<
		" hashhits "<<hashhits<<
		" hashstored "<<hashstored<<
		" pv "<<move2string(rootBestMove);
		
		/*************************************************************/
		/** is there a longer principal variation in the hashtable? **/
		
		// 1. make backup
		for (int i=0; i<64; i++){tempBoard[i/8][i%8]=board[i/8][i%8];}
		tempKey=key;
		
		
		// 2. iterate over PV
		key=newKey(board,key,rootBestMove);
		assignMakeMove(board,rootBestMove);
		
		for (int i=1; i<depth2go; i++){
			hash=hashFunction(board,key);
			hash4moveOrder=int(abs(hash%ht_size));
			hashOfTable=ht_hash[hash4moveOrder];
			
			if (hashOfTable==hash){// we have the exact same hash stored!
				moveShort=ht_moveShort[hash4moveOrder];	
			} else {
				moveShort=0;
			}
			
			assignMoveListAndSort(board,key,moveList,true,moveShort);
			if (moveList[1]%65536==moveShort){ // this is indeed a legal move
				move=moveList[1];
				cout << " "<<move2string(move);
				key=newKey(board,key,move);
				assignMakeMove(board,move);
			} else {
				break;
			}
		}
		
		// 3. restore the backup
		for (int i=0; i<64; i++){board[i/8][i%8]=tempBoard[i/8][i%8];}
		key=tempKey;
		
		/** end of PV stuff **/
		/*********************/
		
		cout << std::endl;
		
		if (abs(evaluation)==100000){break;} // break immediately if there is a mate evaluation
	}
	
	

	/***************************
	***** output best move *****
	***************************/

	cout << "\nnodes: "<<nodes<< "\n"; 
	cout << "staticeval: "<<staticeval<< "\n";
	cout << "movegen: "<<movegen<< "\n";
	cout << "bestmove "<< move2string(rootBestMove) << "\n";
}	

#endif