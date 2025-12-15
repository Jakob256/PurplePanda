/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef assignMakeMoveFILE
#define assignMakeMoveFILE

/***************
*** Let's go ***
***************/

void assignMakeMove(int board[8][8],unsigned int move){
	short int c1=move&0b111;
	short int r1=(move>>3)&0b111;
	short int c2=(move>>6)&0b111;
	short int r2=(move>>9)&0b111;
	short int absPromotedTo=(move>>12)&0b111;
	//short int absCaptured=(move>>15)&0b111;
	bool enPassant =(move>>18)&0b1;
	
	
	int piece=board[c1][r1];

	board[c2][r2]=piece;
	board[c1][r1]=0;

	
	if (enPassant) {board[c2][r1]=0;return;}	
	
  
	//moving rooks when castling
	if ((c1==4)&(c2==2)&(piece== 6)) {board[0][0]=0;board[3][0]= 2;return;}//white queen-side
	if ((c1==4)&(c2==6)&(piece== 6)) {board[7][0]=0;board[5][0]= 2;return;}//white king-side
	if ((c1==4)&(c2==2)&(piece==-6)) {board[0][7]=0;board[3][7]=-2;return;}//black queen-side
	if ((c1==4)&(c2==6)&(piece==-6)) {board[7][7]=0;board[5][7]=-2;return;}//black king-side


	// check if this was a pawn move to the last rank:
	if (piece==+1 && r2==7){board[c2][r2]=+absPromotedTo;} // promotion
	if (piece==-1 && r2==0){board[c2][r2]=-absPromotedTo;} // promotion
}

#endif