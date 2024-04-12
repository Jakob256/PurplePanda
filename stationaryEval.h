/**************************
*** function depends on ***
**************************/

#include "TABLEBASE_3men.h"

/************
*** Guard ***
************/

#ifndef stationaryEvalFILE
#define stationaryEvalFILE

/***************
*** Let's go ***
***************/

float stationaryEval(int board[8][8],unsigned long long int key){
	
	/***********************************
	** unpacking information from key **
	***********************************/
	
	countingStationaryEvalCalled++;
	float eval=int((key>>15)&255)-128;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	int phase24=int((key>>23)&63);
	int PST_mg_score=int((key>>29)&8191)-4096;
	int PST_eg_score=int((key>>42)&8191)-4096;
	int tb_result;
	
	/******************
	** endgame stuff **
	******************/
	
	if (nrPieces<=3){
		if (abs(eval)==3){return 0.;}  // lone bishop or knight can't checkmate
		
		tb_result=TABLEBASE_3men[board2Hash3men(board,turn)];
		if (tb_result==0){return 0;}
		tb_result=1000*tb_result/abs(tb_result)-tb_result;
		return turn*tb_result;
	}
	
	
	eval+=PST_mg_score/100.*(phase24/24.)+PST_eg_score/100.*(1-phase24/24.)+turn*0.3;
	
	return eval;
	
	
}

#endif