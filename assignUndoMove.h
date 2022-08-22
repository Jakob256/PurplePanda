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

void assignUndoMove (int board[8][8],int c1, int r1, int c2, int r2, int adInfo){
	//using the method 'assigning values to input array'
	
	int piece=board[c2][r2];

  	int turn=piece/abs(piece);
	
	if (adInfo==128){// it is an en pessant move:
		board[c1][r1]=piece;
		board[c2][r2]=0;
		board[c2][r1]=-piece;
		return;
	}
  
  
	//castle is important first:
	if ((c1==4)&(c2==2)&(piece==6)){//white Queen-side
		//cout <<"aarg";sleep(1000);
		board[0][0]=2;board[1][0]=0;board[2][0]=0;board[3][0]=0;board[4][0]=6;return;
	}
	if ((c1==4)&(c2==6)&(piece==6)){//white King-side
		//cout <<"aarg";sleep(1000);
		board[4][0]=6;board[5][0]=0;board[6][0]=0;board[7][0]=2;return;
	}
	if ((c1==4)&(c2==2)&(piece==-6)){//black Queen-side
		//cout <<"aarg";sleep(1000);
		board[0][7]=-2;board[1][7]=0;board[2][7]=0;board[3][7]=0;board[4][7]=-6;return;
	}
	if ((c1==4)&(c2==6)&(piece==-6)){//black King-side
		//cout <<"aarg";sleep(1000);
		board[4][7]=-6;board[5][7]=0;board[6][7]=0;board[7][7]=-2;return;
	}

	if (abs(adInfo)<=7){  // no promotion
		board[c1][r1]=piece;
		board[c2][r2]=-turn*adInfo;
	} else {	// promotion
		board[c1][r1]=turn; // will be either 1 or -1
		board[c2][r2]=-turn*adInfo%16;		
	}

}

#endif