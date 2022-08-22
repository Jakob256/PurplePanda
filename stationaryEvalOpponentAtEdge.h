/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef stationaryEvalOpponentAtEdgeFILE
#define stationaryEvalOpponentAtEdgeFILE

/***************
*** Let's go ***
***************/


float stationaryEvalOpponentAtEdge(int board[8][8],int turn,int winningSide){
	int winningSideKingCol=0;
	int winningSideKingRow=0;
	int loosingSideKingCol=0;
	int loosingSideKingRow=0;
	int piece;
	
	for (int col=0;col<8;col++){
		for (int  row=0;row<8;row++){
			piece=board[col][row];
			if (piece==6*winningSide){
				winningSideKingCol=col;
				winningSideKingRow=row;
			}
			if (piece==-6*winningSide){
				loosingSideKingCol=col;
				loosingSideKingRow=row;
			}
		}
	}
	
	float eval=0.;
	eval+= winningSide*(abs(loosingSideKingCol-3.5)+abs(loosingSideKingRow-3.5))*0.1;
	eval+= -winningSide*(abs(loosingSideKingCol-winningSideKingCol)+abs(loosingSideKingRow-winningSideKingRow))*0.1;
	eval+= winningSide*10.;

	
	return eval;
	
}

#endif