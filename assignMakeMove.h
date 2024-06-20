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
	
	if (enPassant){// it is an en passant move:
		board[c1][r1]=0;
		board[c2][r2]=piece;
		board[c2][r1]=0;
		return;
	}
		
	
  
	//castle is important next:
	if ((c1==4)&(c2==2)&(piece==6)){//white Queen-side
		board[0][0]=0;board[1][0]=0;board[2][0]=6;board[3][0]=2;board[4][0]=0;return;
	}
	if ((c1==4)&(c2==6)&(piece==6)){//white King-side
		board[4][0]=0;board[5][0]=2;board[6][0]=6;board[7][0]=0;return;
	}
	if ((c1==4)&(c2==2)&(piece==-6)){//black Queen-side
		board[0][7]=0;board[1][7]=0;board[2][7]=-6;board[3][7]=-2;board[4][7]=0;return;
	}
	if ((c1==4)&(c2==6)&(piece==-6)){//black King-side
		board[4][7]=0;board[5][7]=-2;board[6][7]=-6;board[7][7]=0;return;
	}

	
	board[c2][r2]=piece;
	board[c1][r1]=0;

	// check if this was a pawn move to the last rank:
	if (piece==+1 && r2==7){board[c2][r2]=+absPromotedTo;} // promotion
	if (piece==-1 && r2==0){board[c2][r2]=-absPromotedTo;} // promotion
}

#endif