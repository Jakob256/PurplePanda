/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef hashFunctionFILE
#define hashFunctionFILE

/***************
*** Let's go ***
***************/

long long int hashFunction(int board[8][8],unsigned long long int key){
	int turn;
	long long int hash=0;
	turn=key%2;
	
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			hash+=board[i][j]*hashSquareNumbers[i][j];
		}
	}
	hash+=turn*hashTurnNumber; //hash+= hashTurnNumber*[WhiteToMove]
	
	
	return hash;
}

#endif