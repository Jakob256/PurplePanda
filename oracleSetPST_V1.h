/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef oracleSetPST_V1FILE
#define oracleSetPST_V1FILE

/***************
*** Let's go ***
***************/

void oracleSetPST_V1(int board[8][8],unsigned long long int key){
	
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
				
				/*
				if (globalNrWhiteQueens+globalNrBlackQueens==0 && globalPieceScoreWhite-globalNrWhitePawns <13 && globalPieceScoreBlack-globalNrBlackPawns <13){
					if (piece==-6){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKingEndgame[j][i]/5;}
					if (piece== 6){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKingEndgame[7-j][i]/5;}
				} else {	
					if (piece==-6){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKingMiddlegame[j][i]/5;}
					if (piece== 6){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKingMiddlegame[7-j][i]/5;}
				}
				*/
				
				/// try for PP11:
				if (globalNrWhiteQueens+globalNrBlackQueens==0 && globalPieceScoreWhite-globalNrWhitePawns <13 && globalPieceScoreBlack-globalNrBlackPawns <13){
					if (piece==-6){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKingEndgame[j][i];}
					if (piece== 6){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKingEndgame[7-j][i];}
				} else {	
					if (piece==-6){ pieceSquareTable[6+piece][i][j] =-pieceSquareTableKingMiddlegame[j][i];}
					if (piece== 6){ pieceSquareTable[6+piece][i][j] = pieceSquareTableKingMiddlegame[7-j][i];}
				}
				
				
			}
		}
	}
	
	/****************************
	*** Move Opponent to edge ***
	****************************/
	
	// should deactivate pieceSquareTable
	if (moveOpponentToEdge!=0){
		for (int i=0; i<8;i++){
			for (int j=0; j<8;j++){
				for (int piece=-6; piece < 7; piece++){
					pieceSquareTable[6+piece][i][j] =0;
				}
			}
		}
	}
	
	return;
}
	
#endif