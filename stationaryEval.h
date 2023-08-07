/**************************
*** function depends on ***
**************************/

#include "stationaryEvalTB.h"
#include "stationaryEvalOpponentAtEdge.h"

/************
*** Guard ***
************/

#ifndef stationaryEvalFILE
#define stationaryEvalFILE

/***************
*** Let's go ***
***************/

float stationaryEval (int board[8][8],unsigned long long int key){
	countingStationaryEvalCalled++;
	float eval;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	eval=int((key>>15)&255)-128;
	int bonus=(int((key>>23)&1023)-512);  // since PP10

	if (abs(eval)==3 && nrPieces==3){return 0.;}  // lone bishop or knight cant checkmate

	if (abs(eval)==1 && nrPieces==3){return stationaryEvalTB(board,turn);} // pawn endgame
	
	if (abs(eval)==9 && nrPieces==3){eval=100*eval;}// to keep in line with the next line

	if (moveOpponentToEdge!=0){eval+=stationaryEvalOpponentAtEdge(board,turn,moveOpponentToEdge);}
	
	eval+=bonus/100.;  
		
	return eval;
}

#endif