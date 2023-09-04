/**************************
*** function depends on ***
**************************/

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
	
	/************************
	** pre search counting **
	************************/
	
	int piece;
	int nrWhitePawns=0;
	int nrBlackPawns=0;
	int nrWhiteQueens=0;
	int nrBlackQueens=0;
	int pieceScoreWhite=0;
	int pieceScoreBlack=0;

	
	
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			piece=board[col][row];
			
			if (piece==0){continue;}
			
			if (piece==-5){pieceScoreBlack+=9;}
			if (piece==-4){pieceScoreBlack+=3;}
			if (piece==-3){pieceScoreBlack+=3;}
			if (piece==-2){pieceScoreBlack+=5;}
			if (piece==-1){pieceScoreBlack+=1;}
			if (piece== 1){pieceScoreWhite+=1;}
			if (piece== 2){pieceScoreWhite+=5;}
			if (piece== 3){pieceScoreWhite+=3;}
			if (piece== 4){pieceScoreWhite+=3;}
			if (piece== 5){pieceScoreWhite+=9;}
			
			if (piece==-1){nrBlackPawns+=1;}
			if (piece== 1){nrWhitePawns+=1;}
			
		}
	}
	
	
	
	/******************************
	** Force someone to the edge **
	******************************/
	
	moveOpponentToEdge=0;
	if (nrWhitePawns+nrBlackPawns==0){ // this could be further extended...
		if (pieceScoreWhite>pieceScoreBlack){moveOpponentToEdge=1;}
		if (pieceScoreWhite<pieceScoreBlack){moveOpponentToEdge=-1;}
	}

	/***********************
	** Piece Square Table **
	***********************/
	
	int pieceSquareTablePawn[8][8]={
	{0,  0,  0,  0,  0,  0,  0,  0},
	{50, 50, 50, 50, 50, 50, 50, 50},
	{10, 10, 20, 30, 30, 20, 10, 10},
	{5,  5, 10, 25, 25, 10,  5,  5},
	{0,  0,  0, 20, 20,  0,  0,  0},
	{5, -5,-10,  0,  0,-10, -5,  5},
	{5, 10, 10,-20,-20, 10, 10,  5},
	{0,  0,  0,  0,  0,  0,  0,  0}};

	int pieceSquareTableRook[8][8]={
	{0,  0,  0,  0,  0,  0,  0,  0},
	{5, 10, 10, 10, 10, 10, 10,  5},
	{-5,  0,  0,  0,  0,  0,  0, -5},
	{-5,  0,  0,  0,  0,  0,  0, -5},
	{-5,  0,  0,  0,  0,  0,  0, -5},
	{-5,  0,  0,  0,  0,  0,  0, -5},
	{-5,  0,  0,  0,  0,  0,  0, -5},
	{0,  0,  0,  5,  5,  0,  0,  0}};
	
	int pieceSquareTableKnight[8][8]={
	{-50,-40,-30,-30,-30,-30,-40,-50},
	{-40,-20,  0,  0,  0,  0,-20,-40},
	{-30,  0, 10, 15, 15, 10,  0,-30},
	{-30,  5, 15, 20, 20, 15,  5,-30},
	{-30,  0, 15, 20, 20, 15,  0,-30},
	{-30,  5, 10, 15, 15, 10,  5,-30},
	{-40,-20,  0,  5,  5,  0,-20,-40},
	{-50,-40,-30,-30,-30,-30,-40,-50}};
	
	int pieceSquareTableBishop[8][8]={
	{-20,-10,-10,-10,-10,-10,-10,-20},
	{-10,  0,  0,  0,  0,  0,  0,-10},
	{-10,  0,  5, 10, 10,  5,  0,-10},
	{-10,  5,  5, 10, 10,  5,  5,-10},
	{-10,  0, 10, 10, 10, 10,  0,-10},
	{-10, 10, 10, 10, 10, 10, 10,-10},
	{-10,  5,  0,  0,  0,  0,  5,-10},
	{-20,-10,-10,-10,-10,-10,-10,-20}};
	
	int pieceSquareTableQueen[8][8]={
	{-20,-10,-10, -5, -5,-10,-10,-20},
	{-10,  0,  0,  0,  0,  0,  0,-10},
	{-10,  0,  5,  5,  5,  5,  0,-10},
	{ -5,  0,  5,  5,  5,  5,  0, -5},
	{  0,  0,  5,  5,  5,  5,  0, -5},
	{-10,  5,  5,  5,  5,  5,  0,-10},
	{-10,  0,  5,  0,  0,  0,  0,-10},
	{-20,-10,-10, -5, -5,-10,-10,-20}};
	
	int pieceSquareTableKingMiddlegame[8][8]={
	{-30,-40,-40,-50,-50,-40,-40,-30},
	{-30,-40,-40,-50,-50,-40,-40,-30},
	{-30,-40,-40,-50,-50,-40,-40,-30},
	{-30,-40,-40,-50,-50,-40,-40,-30},
	{-20,-30,-30,-40,-40,-30,-30,-20},
	{-10,-20,-20,-20,-20,-20,-20,-10},
	{ 20, 20,  0,  0,  0,  0, 20, 20},
	{ 20, 30, 10,  0,  0, 10, 30, 20}};
	
	int pieceSquareTableKingEndgame[8][8]={
	{-50,-40,-30,-20,-20,-30,-40,-50},
	{-30,-20,-10,  0,  0,-10,-20,-30},
	{-30,-10, 20, 30, 30, 20,-10,-30},
	{-30,-10, 30, 40, 40, 30,-10,-30},
	{-30,-10, 30, 40, 40, 30,-10,-30},
	{-30,-10, 20, 30, 30, 20,-10,-30},
	{-30,-30,  0,  0,  0,  0,-30,-30},
	{-50,-30,-30,-30,-30,-30,-30,-50}};

	
	for (int i=0; i<8;i++){
		for (int j=0; j<8;j++){
			for (int piece=-6; piece < 7; piece++){
				
				if (piece==-5){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableQueen[j][i]/5;}
				if (piece==-4){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableBishop[j][i]/5;}
				if (piece==-3){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKnight[j][i]/5;}
				if (piece==-2){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableRook[j][i]/5;}
				if (piece==-1){ pieceSquareTable[6+piece][i][j] =-pieceSquareTablePawn[j][i]/5;}
				if (piece== 0){ pieceSquareTable[6+piece][i][j] = 0;}
				if (piece== 1){ pieceSquareTable[6+piece][i][j] = pieceSquareTablePawn[7-j][i]/5;}
				if (piece== 2){ pieceSquareTable[6+piece][i][j] = pieceSquareTableRook[7-j][i]/5;}
				if (piece== 3){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKnight[7-j][i]/5;}
				if (piece== 4){ pieceSquareTable[6+piece][i][j] = pieceSquareTableBishop[7-j][i]/5;}
				if (piece== 5){ pieceSquareTable[6+piece][i][j] = pieceSquareTableQueen[7-j][i]/5;}
				
				
				 // could be expanded:
				if (nrWhiteQueens+nrBlackQueens==0 && pieceScoreWhite-nrWhitePawns <13 && pieceScoreBlack-nrBlackPawns <13){
					if (piece==-6){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKingEndgame[j][i]/5;}
					if (piece== 6){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKingEndgame[7-j][i]/5;}
				} else {	
					if (piece==-6){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKingMiddlegame[j][i]/5;}
					if (piece== 6){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKingMiddlegame[7-j][i]/5;}
				}
				
				
			}
		}
	}
	
	
	/****************************
	*** Move Opponent to edge ***
	****************************/
	
	// if wants to force king to the edge, loosing King wants to stay in the middle
	if (moveOpponentToEdge!=0){
		for (int col=0; col<8;col++){
			for (int row=0; row<8;row++){
				for (int piece=-6; piece < 7; piece++){
					
					if (piece==-6*moveOpponentToEdge){ // opponents king; measure distance to center
						pieceSquareTable[6+piece][col][row]= moveOpponentToEdge*(abs(col-3.5)+abs(row-3.5))*20;// measured in 1/100
					} else {
						pieceSquareTable[6+piece][col][row]=0;
					}
					
				}
			}
		}
	}
	
	return;
}
	
#endif