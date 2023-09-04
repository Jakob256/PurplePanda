/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"

/******************
*** Description ***
******************/

// here the functions for calculating nrPieces, score, bonus and hash are defined.
// however, they aren't used in the search, as they are iterativly updated
// here they are mainly for debugging purposes

/************
*** Guard ***
************/

#ifndef positionFeaturesFILE
#define positionFeaturesFILE


/***************
*** Let's go ***
***************/

int nrPiecesFunction(int board[8][8], unsigned long long int key){
	int nrPieces=0;
	
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			if (board[col][row]!=0){
				nrPieces+=1;
			}
		}
	}
	
	return nrPieces;
}


int scoreFunction(int board[8][8], unsigned long long int key){
	int score=0;
	
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			if (board[col][row]== 1){score+=1;}
			if (board[col][row]== 2){score+=5;}
			if (board[col][row]== 3){score+=3;}
			if (board[col][row]== 4){score+=3;}
			if (board[col][row]== 5){score+=9;}
			if (board[col][row]==-1){score-=1;}
			if (board[col][row]==-2){score-=5;}
			if (board[col][row]==-3){score-=3;}
			if (board[col][row]==-4){score-=3;}
			if (board[col][row]==-5){score-=9;}
		}
	}
	
	return score;
}


int bonusFunction(int board[8][8], unsigned long long int key){
	int bonus=0;
	
	for (int col=0; col<8; col++){
		for (int row=0; row<8; row++){
			bonus+= pieceSquareTable[6+board[col][row]][col][row];
		}
	}
	
	return bonus;
}


long long int hashFunction(int board[8][8],unsigned long long int key){
	int turn=key%2; // either 0 or 1
	long long int hash=0;
	
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			hash+=board[i][j]*hashSquareNumbers[i][j];
		}
	}
	hash+=turn*hashTurnNumber; //hash+= hashTurnNumber*[WhiteToMove]
	
	
	return hash;
}


#endif