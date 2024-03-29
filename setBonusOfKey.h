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
	int gamePhase=gamePhaseFunction(board,key);
	int PSTMiddlegame=PSTMiddlegameFunction(board,key)+4096;
	int PSTEndgame=PSTEndgameFunction(board,key)+4096;
	
	unsigned long long int PSTMiddlegame_=PSTMiddlegame;
	unsigned long long int PSTEndgame_=PSTEndgame;

	// we will keep the key intact and only change the gamePhase, PSTMiddlegame PSTEndgame.
	// the "eval" is already handled by "fen2key.h". This is not very consistent and should probably be changed
	
	unsigned long long int deleteMask=0b11111111111111111111111111111111111111100000000000000000000000;
	key=key & (key^deleteMask); // delets bits 
	
	
	
	
	key+= gamePhase*8388608;
	key+= PSTMiddlegame_*536870912;  //2^29
	key+= PSTEndgame_*4398046511104; //2^42
	
	
	return key;
}
	
#endif