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
	
	
	
	/********************
	** PST from Tuning **
	********************/
	
	int PST_Pawn_mg[8][8]={
	{  0,  0,  0,  0,  0,  0,  0,  0},
	{180, 99,180,177,162, -6,153, -6},
	{ 39, 48, 72, 84, 90,111, 90, 66},
	{ 15, -3, -6,  9, 42, 33, 36, 12},
	{-18, -6,  3,  6,  9, -3, 12, -6},
	{-18,-18,-18,-15,-12,-15, 18,-12},
	{-18,-18,-18,-18,-18,  3, 24,-18},
	{  0,  0,  0,  0,  0,  0,  0,  0}};


	int PST_Rook_mg[8][8]={
	{ 75, 57, 66,-12, 90, 90, 30, 90},
	{-27,-18, 60, 87, 90, 90, 90, 90},
	{-54,  6, 30,-36, 90, 90, 90, 72},
	{-69,-15,  3,-18,  0,  6, 66,-24},
	{-69,-45,-75,-54,-39,-33,-15, -6},
	{-78,-72,-90,-57,-51,-51,  9,-51},
	{-75,-81,-63,-75,-60,-45,-21,-90},
	{-69,-60,-36,-45,-39,-51,-60,-69}};


	int PST_Knight_mg[8][8]={
	{-90, -6,-90, 36,-51,-90,-90,-90},
	{ 30, -3, 90, 75, 90, 90, 21, 90},
	{-51,  3, 60, 90, 90, 90, 90, 78},
	{ 42, 24, 36, 69, 21, 57, 45, 45},
	{-27,-54, 12,-15, -3, 45, 48,-15},
	{-54,-42,-24,-21,  6,-15, -6,-27},
	{-90,-90,-33,-30,-27,-21,-66,-54},
	{-90,-60,-81,-60,-66,-48,-42,-90}};


	int PST_Bishop_mg[8][8]={
	{-60, 66,-33,-90, 75,-90,-45,-90},
	{ 60,-33, 84,-18, 90, 90, 30, 66},
	{ 69, 60, 51, 90, 90, 90, 90, 90},
	{ 42, 33, 60, 90, 84, 75, 36, 33},
	{ 33, 51, 36, 69, 39, 48, 24, 39},
	{ 30, 39, 18, 30, 21,  6, 51, 36},
	{ 33, 33, 51,  0,  3, 27, 33, 33},
	{ -9, -9, -3,-60,-18, -9, 51,-24}};


	int PST_Queen_mg[8][8]={
	{ 10, 91,148,190,190,190,190,190},
	{ 88, 43, 85,187,121,190,190,190},
	{ 79, 97, 64,136,190,190,190,187},
	{ 58, 46, 55, 94,151,136,106,115},
	{ 73,112, 73, 76, 82, 73,115, 91},
	{ 88, 76, 64, 43, 49, 61,100,103},
	{ 34, 73, 73, 76, 67,100, 94,127},
	{ 97, 58, 19, 61, 31, 10, 46, 70}};


	int PST_King_mg[8][8]={
	{ 75,-90, 90, 90, 42, 90, 90, 48},
	{-42,  0, 90, 90,-63, 90, 90, 90},
	{-90, 90, 90, 60, 12, 90, 90,-21},
	{-87,-12, 27, 48, 54,-12,-24,-90},
	{-90, 90,-72,  0,-33,-66,-90,-90},
	{ 54,-90,-24,-90,-90,-90,-84,-90},
	{ 90,-27,  3,-36,-24,-72, 18,  6},
	{  0, 60, 48,-84, 27,-63, 45, 48}};


	int PST_Pawn_eg[8][8]={
	{  0,  0,  0,  0,  0,  0,  0,  0},
	{147,141,156,102,138,162, 84,120},
	{117,111,120,108, 69, 69, 96,105},
	{ 57, 72, 60, 33, 30, 30, 54, 45},
	{ 36, 51,  9, 21, 15, 33, 39, 21},
	{ 12, 57, 27, 45, 57, 39, 39, 18},
	{ 27, 60, 42, 60, 69, 48, 45, 15},
	{  0,  0,  0,  0,  0,  0,  0,  0}};


	int PST_Rook_eg[8][8]={
	{ 36, 75, 45, 90, 42, 48, 78, 39},
	{ 84, 90, 87, 81, 63, 90, 78, 36},
	{ 69, 69, 57, 87, 18, 39, 27, 18},
	{ 84, 45, 54, 84, 48, 30, 30, 48},
	{ 39, 69, 84, 60, 45, 72, 51, 27},
	{ 27, 42, 27, 36, -6, 21, 33, 42},
	{ -6, 21, 18, 15, 12, -6,-21, 54},
	{ 21,  6,  6, 18, 12, 21, 15,  6}};


	int PST_Knight_eg[8][8]={
	{-90, -3, 24, -3, 27, 15,-18,-90},
	{ 24,  0,  0, 48, 21, 36,-12, 15},
	{ 42, 42, 45, 72, 90, 90, 63, 36},
	{ 24, 30, 39, 66, 87, 51, 54, 15},
	{-21, 66, 69, 84, 81, 72, 30, -3},
	{-57, -9, 24, 48, 27,  9,-18,-66},
	{-30,-21,-54, -3,-27,-15, 30,-51},
	{-90,-90,-69,-63,-21,-45,-87,-90}};


	int PST_Bishop_eg[8][8]={
	{ 90, 24,  0, 51,  3, 81, 12, 36},
	{  6, 30,-18, 39, -9, 42, 24, 42},
	{ 12, 81, 18, 21, 60, 72, 75, 60},
	{ 45, 36, 51, 36, 42, 39, 21, 27},
	{-36, 39, 33, 27, 42, 24,  9,  9},
	{  0, 15, 57, 48, 60, 36,-18, 45},
	{-30,-24, -3,  6, 36,-27,  9,-12},
	{  6,-51,-24, 18,-24, -3,-24,-45}};


	int PST_Queen_eg[8][8]={
	{ 42, 90, 60, 45, 75, 90, 90, 45},
	{-39, 75, 78,-27, 90, 90, 90, 81},
	{ 18,-21, 60, 24, 90, 90, 90, 30},
	{ 66, 60, 51, 90, 90, 90, 90, 84},
	{-15,-66, 63, 90, 90, 90, -6, 18},
	{-18,-45, 45, 51, 51, 36,-36,-90},
	{ 36,-15,-66,-36,-39,-75,-90,-90},
	{-72,-90,-60,-87,-18,-57,-12, 54}};


	int PST_King_eg[8][8]={
	{-90,-90,-45,-18,-39, 21,-39,-90},
	{ 39,  0, 69, 72, 57, 81, 84,-24},
	{-18, 81, 90, 78, 90, 90, 78, 42},
	{ 15, 63, 72, 87, 90, 84, 75,  3},
	{ -9, 39, 57, 69, 69, 48, 42, -6},
	{-54, -3,  0, 27, 33, 24,  0,-21},
	{-78,-12,-12, -6,  9,  6,-12,-33},
	{-45,-33,-48,-24,-90,-27,-27,-90}};


	/*************************
	*** Further processing ***
	*************************/
	
	
	for (int i=0; i<8;i++){
		for (int j=0; j<8;j++){
			for (int piece=-6; piece < 7; piece++){

				if (piece==-6){ PST_mg[6+piece][i][j] =-PST_King_mg[j][i];}				
				if (piece==-5){ PST_mg[6+piece][i][j] =-PST_Queen_mg[j][i];}
				if (piece==-4){ PST_mg[6+piece][i][j] =-PST_Bishop_mg[j][i];}
				if (piece==-3){ PST_mg[6+piece][i][j] =-PST_Knight_mg[j][i];}
				if (piece==-2){ PST_mg[6+piece][i][j] =-PST_Rook_mg[j][i];}
				if (piece==-1){ PST_mg[6+piece][i][j] =-PST_Pawn_mg[j][i];}
				if (piece== 0){ PST_mg[6+piece][i][j] = 0;}
				if (piece== 1){ PST_mg[6+piece][i][j] = PST_Pawn_mg[7-j][i];}
				if (piece== 2){ PST_mg[6+piece][i][j] = PST_Rook_mg[7-j][i];}
				if (piece== 3){ PST_mg[6+piece][i][j] = PST_Knight_mg[7-j][i];}
				if (piece== 4){ PST_mg[6+piece][i][j] = PST_Bishop_mg[7-j][i];}
				if (piece== 5){ PST_mg[6+piece][i][j] = PST_Queen_mg[7-j][i];}
				if (piece== 6){ PST_mg[6+piece][i][j] = PST_King_mg[7-j][i];}
				
				
				if (piece==-6){ PST_eg[6+piece][i][j] =-PST_King_eg[j][i];}				
				if (piece==-5){ PST_eg[6+piece][i][j] =-PST_Queen_eg[j][i];}
				if (piece==-4){ PST_eg[6+piece][i][j] =-PST_Bishop_eg[j][i];}
				if (piece==-3){ PST_eg[6+piece][i][j] =-PST_Knight_eg[j][i];}
				if (piece==-2){ PST_eg[6+piece][i][j] =-PST_Rook_eg[j][i];}
				if (piece==-1){ PST_eg[6+piece][i][j] =-PST_Pawn_eg[j][i];}
				if (piece== 0){ PST_eg[6+piece][i][j] = 0;}
				if (piece== 1){ PST_eg[6+piece][i][j] = PST_Pawn_eg[7-j][i];}
				if (piece== 2){ PST_eg[6+piece][i][j] = PST_Rook_eg[7-j][i];}
				if (piece== 3){ PST_eg[6+piece][i][j] = PST_Knight_eg[7-j][i];}
				if (piece== 4){ PST_eg[6+piece][i][j] = PST_Bishop_eg[7-j][i];}
				if (piece== 5){ PST_eg[6+piece][i][j] = PST_Queen_eg[7-j][i];}
				if (piece== 6){ PST_eg[6+piece][i][j] = PST_King_eg[7-j][i];}
				
				
			}
		}
	}
	
	/*********************************************
	*** Rook and Bishop Mobility Bonus/Penalty ***
	*********************************************/
	
	int dx,dy,mobilityWhite,mobilityBlack,endpiece,targetRow,targetCol;
	
	for (int col=0;col<8;col++){// Rook
		for (int row=0;row<8;row++){
			mobilityWhite=0;
			mobilityBlack=0;
			
			for (int i=0; i<4;i++){
				if (i==0){dx=-1;dy= 0;}
				if (i==1){dx= 1;dy= 0;}
				if (i==2){dx= 0;dy=-1;}
				if (i==3){dx= 0;dy= 1;}
				for (int j=1; j<8; j++){
					targetRow=row+dy*j;targetCol=col+dx*j;endpiece=board[targetCol][targetRow]; 
					if (targetRow<0 || targetRow >7 || targetCol<0 || targetCol>7){break;}
					if (endpiece==0){mobilityWhite++;mobilityBlack++;}
					if (endpiece<0){mobilityWhite+=2;}
					if (endpiece>0){mobilityBlack+=2;}
					if (endpiece!=0){break;}
				}
			}
			
			PST_mg[6+2][col][row]+=(mobilityWhite-5)*4;
			PST_mg[6-2][col][row]-=(mobilityBlack-5)*4;
		}
	}
	
	
	for (int col=0;col<8;col++){// Bishop
		for (int row=0;row<8;row++){
			mobilityWhite=0;
			mobilityBlack=0;
			
			for (int i=0; i<4;i++){
				if (i==0){dx=-1;dy= 1;}
				if (i==1){dx= 1;dy=-1;}
				if (i==2){dx= 1;dy= 1;}
				if (i==3){dx=-1;dy=-1;}
				for (int j=1; j<8; j++){
					targetRow=row+dy*j;targetCol=col+dx*j;endpiece=board[targetCol][targetRow]; 
					if (targetRow<0 || targetRow >7 || targetCol<0 || targetCol>7){break;}
					if (endpiece==0){mobilityWhite++;mobilityBlack++;}
					if (endpiece<0){mobilityWhite+=2;}
					if (endpiece>0){mobilityBlack+=2;}
					if (endpiece!=0){break;}
				}
			}
			
			PST_mg[6+4][col][row]+=(mobilityWhite-5)*4;
			PST_mg[6-4][col][row]-=(mobilityBlack-5)*4;
		}
	}
	
	
	
	
	
	/******************************
	*** Only one pawn remaining ***
	******************************/

	
	if (nrWhitePawns+nrBlackPawns==1){
		int colPawn;
		int rowPawn;
		int colorPawn;
		for (int col=0;col<8;col++){
			for (int row=0;row<8;row++){
				piece=board[col][row];
				if (abs(piece)==1){
					colPawn=col;
					rowPawn=row;
					colorPawn=piece;
				}
			}
		}
		
		for (int col=0;col<8;col++){
			for (int row=0;row<8;row++){
				for (int piece=-6;piece<=6;piece++){PST_eg[6+piece][col][row]=0;}
				for (int piece=-6;piece<=6;piece++){PST_eg[6+piece][col][row]=0;}
				PST_eg[6+1][col][row]=row*30;
				PST_eg[6-1][col][row]=-(7-row)*30;
				
				
				if ((row>rowPawn && colorPawn==1) || (row<rowPawn && colorPawn==-1)){ // in front of the pawn
					PST_eg[6-6][col][row]= -(64-abs(col-colPawn)*8);
					PST_eg[6+6][col][row]= +(64-abs(col-colPawn)*8);
				} else {
					PST_eg[6-6][col][row]= -(64-8*max(abs(col-colPawn),abs(row-rowPawn-colorPawn)));
					PST_eg[6+6][col][row]= +(64-8*max(abs(col-colPawn),abs(row-rowPawn-colorPawn)));
				}
				
				// copy to middlegame pst
				for (int piece=-6;piece<=6;piece++){PST_mg[6+piece][col][row]=PST_eg[6+piece][col][row];}
				
				
			}
		}
	}
	
	
	
	/**************************************************
	**** defining the average PST for moveordering ****
	**************************************************/

	
	float gP=gamePhaseFunction(board,key);
	for (int i=0; i<8;i++){
		for (int j=0; j<8;j++){
			for (int piece=-6; piece < 7; piece++){
				PST_average[6+piece][i][j]=gP/24.*PST_mg[6+piece][i][j]+(24.-gP)/24.*PST_eg[6+piece][i][j];
			}
		}
	}
	
}
	
#endif