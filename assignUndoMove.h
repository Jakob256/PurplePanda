/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef assignUndoMoveFILE
#define assignUndoMoveFILE

/***************
*** Let's go ***
***************/

void assignUndoMove (int board[8][8],unsigned int move){
	short int c1=move&0b111;
	short int r1=(move>>3)&0b111;
	short int c2=(move>>6)&0b111;
	short int r2=(move>>9)&0b111;
	short int absPromotedTo=(move>>12)&0b111;
	short int absCaptured=(move>>15)&0b111;
	bool enPassant =(move>>18)&0b1;
	
	int piece=board[c2][r2];
  	int turn=piece/abs(piece);
	
	board[c1][r1]=piece;
	board[c2][r2]=-turn*absCaptured;
	
	if (enPassant){
		board[c2][r2]=0;
		board[c2][r1]=-piece;
		return;
	}
  
  
	//moving rooks when castling
	if ((c1==4)&(c2==2)&(piece== 6)) {board[0][0]= 2;board[3][0]=0;return;}//white queen-side
	if ((c1==4)&(c2==6)&(piece== 6)) {board[7][0]= 2;board[5][0]=0;return;}//white king-side
	if ((c1==4)&(c2==2)&(piece==-6)) {board[0][7]=-2;board[3][7]=0;return;}//black queen-side
	if ((c1==4)&(c2==6)&(piece==-6)) {board[7][7]=-2;board[5][7]=0;return;}//black king-side

	if (absPromotedTo!=0){  // promotion
		board[c1][r1]=turn; // will be either 1 or -1
		board[c2][r2]=-turn*absCaptured;		
	}
}

#endif