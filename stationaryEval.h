/**************************
*** function depends on ***
**************************/

#include "stationaryEvalTB.h"

/************
*** Guard ***
************/

#ifndef stationaryEvalFILE
#define stationaryEvalFILE

/***************
*** Let's go ***
***************/

float stationaryEval (int board[8][8],unsigned long long int key){
	
	/******************
	** initial stuff **
	******************/
	
	countingStationaryEvalCalled++;
	float eval=int((key>>15)&255)-128;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	int bonus=(int((key>>23)&1023)-512);


	/******************
	** endgame stuff **
	******************/
	
	if (nrPieces==3){
		if (abs(eval)==3){return 0.;}  // lone bishop or knight cant checkmate
		if (abs(eval)==1){return stationaryEvalTB(board,turn);} // pawn endgame
		if (abs(eval)==9){eval=100*eval;} // to keep in line with the following
	}
	
	eval+=bonus/100.;
	
	
	/************************
	** wanting kings close **
	************************/
	
	int whiteKingCol,whiteKingRow,blackKingCol,blackKingRow,piece;
	
	if (moveOpponentToEdge!=0){
		for (int col=0;col<8;col++){
			for (int row=0;row<8;row++){
				piece=board[col][row];
				if (piece==6){
					whiteKingCol=col;
					whiteKingRow=row;
				}
				if (piece==-6){
					blackKingCol=col;
					blackKingRow=row;
				}
			}
		}
		eval+= -moveOpponentToEdge*max(abs(whiteKingCol-blackKingCol),abs(whiteKingRow-blackKingRow))*0.01;  // king distance metric
		eval+=10.*moveOpponentToEdge;
		return eval;
	}
	
	return eval;
	
	
}

#endif