/**************************
*** function depends on ***
**************************/

#include "preSearchCalculationsV1.h"
#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef oracleV1FILE
#define oracleV1FILE

/***************
*** Let's go ***
***************/

void oracleV1(int board[8][8],unsigned long long int key){
	
	
	/**********************
	** Force Legal Moves **
	**********************/
	
	oracleStrongLegalMoves=false;
	if (globalNrWhitePieces<=2 || globalNrBlackPieces<=2){
		oracleStrongLegalMoves=true;
	}
	
	/******************************
	** Force someone to the edge **
	******************************/
	
	moveOpponentToEdge=0;
	if (globalNrWhitePawns+globalNrBlackPawns==0){ // this could be further extended...
		if (globalPieceScoreWhite>globalPieceScoreBlack){moveOpponentToEdge=1;}
		if (globalPieceScoreWhite<globalPieceScoreBlack){moveOpponentToEdge=-1;}		
	}

	
	return;
}
	
#endif