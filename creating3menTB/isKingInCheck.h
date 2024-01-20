/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef isKingInCheckFILE
#define isKingInCheckFILE

/***************
*** Let's go ***
***************/


int isKingInCheck(int board[8][8],int color,int col, int row){ 
	// if (col,row) are known, please pass them to the function; otherwise pass (0,0).
	// will return 0 if not in check
	// will return 1+attackingPieceCol+8*attackingPieceRow if in check
	
	if (board[col][row]!=6*color){
		//cout << "king was not where expected!\n";
		for (int r=0;r<8;r++){
			for (int c=0; c<8; c++){
				if (board[c][r]==6*color){
					col=c;
					row=r;
				}
			}
		}
	}
	
	
	int targetCol,targetRow,piece,dx,dy;
	
	

	// check knight
	targetRow=row-2;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row-2;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row-1;targetCol=col-2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row-1;targetCol=col+2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row+1;targetCol=col-2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row+1;targetCol=col+2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row+2;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}
	targetRow=row+2;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return 1+targetCol+8*targetRow;}

	// check king (this is necessary to avoid a king stepping next to the other king)
	targetRow=row-1;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row-1;targetCol=col+0;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row-1;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row+0;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row+0;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row+1;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row+1;targetCol=col+0;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}
	targetRow=row+1;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return 1+targetCol+8*targetRow;}

	for (int i=0; i<4; i++){  //rook (and queen)
		if (i==0){dx=1;dy=0;}
		if (i==1){dx=-1;dy=0;}
		if (i==2){dx=0;dy=1;}
		if (i==3){dx=0;dy=-1;}
		for (int j=1; j<8; j++){
			targetRow=row+dy*j;targetCol=col+dx*j;piece=board[targetCol][targetRow]; 
			if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7){ // target square on board
				if (piece==-2*color){return 1+targetCol+8*targetRow;}
				if (piece==-5*color){return 1+targetCol+8*targetRow;}
				if (piece!=0){break;}
				
			}
		}
	}
	
	for (int i=0; i<4; i++){  //bishop (and queen)
		if (i==0){dx=1;dy=1;}
		if (i==1){dx=1;dy=-1;}
		if (i==2){dx=-1;dy=1;}
		if (i==3){dx=-1;dy=-1;}
		for (int j=1; j<8; j++){
			targetRow=row+dy*j;targetCol=col+dx*j;piece=board[targetCol][targetRow]; 
			if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7){ // target square on board
				if (piece==-4*color){return 1+targetCol+8*targetRow;}
				if (piece==-5*color){return 1+targetCol+8*targetRow;}
				if (piece!=0){break;}
				
			}
		}
	}
	
	
	//check pawn
	targetRow=row+color;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-1*color){return 1+targetCol+8*targetRow;}
	targetRow=row+color;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-1*color){return 1+targetCol+8*targetRow;}

	return 0;
}


#endif