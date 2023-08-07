/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef preSearchCalculationsFILE
#define preSearchCalculationsFILE

/***************
*** Let's go ***
***************/

void preSearchCalculations(int board[8][8],unsigned long long int key){
	
	int piece;
	
	globalNrWhitePawns=0;
	globalNrBlackPawns=0;
	globalNrWhiteRooks=0;
	globalNrBlackRooks=0;
	globalNrWhiteKnights=0;
	globalNrBlackKnights=0;
	globalNrWhiteBishops=0;
	globalNrBlackBishops=0;
	globalNrWhiteQueens=0;
	globalNrBlackQueens=0;
	
	globalNrWhitePieces=0;
	globalNrBlackPieces=0;
	
	globalPieceScoreWhite=0;
	globalPieceScoreBlack=0;
	globalPiecesOnHomeSquareWhite=0;
	globalPiecesOnHomeSquareBlack=0;
	
	for (int col=0;col<8;col++){
		for (int  row=0;row<8;row++){
			piece=board[col][row];
			
			if (piece==0){continue;}
			
			if (piece>0){globalNrWhitePieces++;}
			if (piece<0){globalNrBlackPieces++;}
			
			if (piece==-5){globalPieceScoreBlack+=9;globalNrBlackQueens++;}
			if (piece==-4){globalPieceScoreBlack+=3;globalNrBlackBishops++;}
			if (piece==-3){globalPieceScoreBlack+=3;globalNrBlackKnights++;}
			if (piece==-2){globalPieceScoreBlack+=5;globalNrBlackRooks++;}
			if (piece==-1){globalPieceScoreBlack+=1;globalNrBlackPawns++;}
			if (piece==1 ){globalPieceScoreWhite+=1;globalNrWhitePawns++;}
			if (piece==2 ){globalPieceScoreWhite+=5;globalNrWhiteRooks++;}
			if (piece==3 ){globalPieceScoreWhite+=3;globalNrWhiteKnights++;}
			if (piece==4 ){globalPieceScoreWhite+=3;globalNrWhiteBishops++;}
			if (piece==5 ){globalPieceScoreWhite+=9;globalNrWhiteQueens++;}
		}
	}
	

	if (board[0][0]==2){globalPiecesOnHomeSquareWhite++;}
	if (board[1][0]==3){globalPiecesOnHomeSquareWhite++;}
	if (board[2][0]==4){globalPiecesOnHomeSquareWhite++;}
	if (board[3][0]==5){globalPiecesOnHomeSquareWhite++;}
	if (board[4][0]==6){globalPiecesOnHomeSquareWhite++;}
	if (board[5][0]==4){globalPiecesOnHomeSquareWhite++;}
	if (board[6][0]==3){globalPiecesOnHomeSquareWhite++;}
	if (board[7][0]==2){globalPiecesOnHomeSquareWhite++;}	
	
	if (board[0][7]==-2){globalPiecesOnHomeSquareBlack++;}
	if (board[1][7]==-3){globalPiecesOnHomeSquareBlack++;}
	if (board[2][7]==-4){globalPiecesOnHomeSquareBlack++;}
	if (board[3][7]==-5){globalPiecesOnHomeSquareBlack++;}
	if (board[4][7]==-6){globalPiecesOnHomeSquareBlack++;}
	if (board[5][7]==-4){globalPiecesOnHomeSquareBlack++;}
	if (board[6][7]==-3){globalPiecesOnHomeSquareBlack++;}
	if (board[7][7]==-2){globalPiecesOnHomeSquareBlack++;}	
	
	for (int col=0;col<8;col++){
		if (board[col][1]==1){globalPiecesOnHomeSquareWhite++;}	
		if (board[col][6]==-1){globalPiecesOnHomeSquareBlack++;}	
	}
	
	globalSide2Play=-1+(key%2)*2;
	
	return;
}
	
#endif