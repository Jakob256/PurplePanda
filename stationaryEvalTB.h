/**************************
*** function depends on ***
**************************/

#include "TABLEBASE_KPk.h"

/************
*** Guard ***
************/

#ifndef stationaryEvalTBFILE
#define stationaryEvalTBFILE

/***************
*** Let's go ***
***************/



int KPk_hashFunction(int board[8][8],int turn){
	int position_K;
	int position_P;
	int position_k;
	
	int atPosition=-1;
	for (int col=0; col<8;col++){
		for (int row=0; row<8;row++){
			atPosition++;
			if (board[col][row]==6){position_K=atPosition;}
			if (board[col][row]==1){position_P=atPosition;}
			if (board[col][row]==-6){position_k=atPosition;}
		}
	}
	int hash=position_K+64*position_k+64*64*position_P;
	if (turn==1){hash+=64*64*32;}
	
	return hash;
}



float stationaryEvalTB (int board[8][8],int turn){
	int board2[8][8];
	
	int pawnColor;
	int pawnRow;
	int pawnCol;
	int hv;
	int piece;
	
	for (int col=0;col<8;col++){
		for (int  row=0;row<8;row++){
			
			piece=board[col][row];
			
			if (abs(piece)==1){
				pawnColor=piece;
				pawnCol=col;
				pawnRow=row;
			}
		}
	}
	
	
	for (int col=0; col<8; col++){
		for (int row=0;row<8;row++){
			if (pawnColor==1 && pawnCol<=3){board2[col][row]=board[col][row];} // no flipping necessary
			if (pawnColor==1 && pawnCol>3){board2[col][row]=board[7-col][row];} // vertical flipping
			if (pawnColor==-1 && pawnCol<=3){board2[col][row]=-board[col][7-row];} // horizontal flipping
			if (pawnColor==-1 && pawnCol>3){board2[col][row]=-board[7-col][7-row];} // vertical+horizontal flipping
		}
	}
	
	float eval;
	//cout << "pawn color: "<< pawnColor<<"\n";
	if (pawnColor==1){eval = 0.0 + TABLEBASE_KPk[KPk_hashFunction(board2,turn)];}
	if (pawnColor==-1){eval =0.0 - TABLEBASE_KPk[KPk_hashFunction(board2,-turn)];}
	
	//plotBoard(board2);
	
	//cout << "\n\n\nnow";
	//cout << pawnColor;
	//if (pawnColor==1){cout << KPk_hashFunction(board2,turn);}
	//if (pawnColor==-1){cout <<  KPk_hashFunction(board2,-turn);}
	//cout << "there\n\n\n";
	
	
	
	
	return eval;
	
	
}

#endif