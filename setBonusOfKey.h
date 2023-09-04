/**************************
*** function depends on ***
**************************/

#include "positionFeatures.h"

/************
*** Guard ***
************/

#ifndef setBonusOfKeyFILE
#define setBonusOfKeyFILE

/***************
*** Let's go ***
***************/

unsigned long long int setBonusOfKey(int board[8][8],unsigned long long int key){
	int bonus=bonusFunction(board,key);
	

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