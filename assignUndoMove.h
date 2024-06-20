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
	
	if (enPassant){
		board[c1][r1]=piece;
		board[c2][r2]=0;
		board[c2][r1]=-piece;
		return;
	}
  
  
	//castling
	if ((c1==4)&(c2==2)&(piece==6)){//white Queen-side
		board[0][0]=2;board[1][0]=0;board[2][0]=0;board[3][0]=0;board[4][0]=6;return;
	}
	if ((c1==4)&(c2==6)&(piece==6)){//white King-side
		board[4][0]=6;board[5][0]=0;board[6][0]=0;board[7][0]=2;return;
	}
	if ((c1==4)&(c2==2)&(piece==-6)){//black Queen-side
		board[0][7]=-2;board[1][7]=0;board[2][7]=0;board[3][7]=0;board[4][7]=-6;return;
	}
	if ((c1==4)&(c2==6)&(piece==-6)){//black King-side
		board[4][7]=-6;board[5][7]=0;board[6][7]=0;board[7][7]=-2;return;
	}

	if (absPromotedTo==0){  // no promotion
		board[c1][r1]=piece;
		board[c2][r2]=-turn*absCaptured;
	} else {	// promotion
		board[c1][r1]=turn; // will be either 1 or -1
		board[c2][r2]=-turn*absCaptured;		
	}
}

#endif