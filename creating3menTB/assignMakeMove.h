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

void assignMakeMove (int board[8][8],int c1, int r1, int c2, int r2, int adInfo){
	//using the method 'assigning values to input array'
	
	int piece=board[c1][r1];
	
	int turn=piece/abs(piece);
	
	if (adInfo==128){// it is an en pessant move:
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
	if (piece==1 && r2==7){board[c2][r2]=adInfo/16;} // promotion
	if (piece==-1 && r2==0){board[c2][r2]=-adInfo/16;} // promotion
}

#endif