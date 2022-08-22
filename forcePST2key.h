/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef forcePST2keyFILE
#define forcePST2keyFILE

/***************
*** Let's go ***
***************/

unsigned long long int forcePST2key(int board[8][8],unsigned long long int key){
	int bonus=0;
	
	for (int c=0; c<8; c++){
		for (int r=0; r<8; r++){
			bonus+= pieceSquareTable[6+board[c][r]][c][r];
		}
	}
	

	// adding bonus 2 key ==== this part is long because of type conversion issues
	unsigned long long int mask=0b111111111100000000000000000000000;
	key=key & (key^mask); // delets bonus bits 
	bonus=bonus+512;
	unsigned long long int bonus2add= bonus;
	bonus2add=bonus2add*8388608;
	
	
	key=key+ bonus2add;
	
	
	return key;
}
	
#endif