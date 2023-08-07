/**************************
*** function depends on ***
**************************/

#include "preSearchCalculations.h"
#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef oracleFILE
#define oracleFILE

/***************
*** Let's go ***
***************/

void oracle(int board[8][8],unsigned long long int key){
	
	
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