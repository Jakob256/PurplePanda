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


bool isKingInCheck(int board[8][8],int color){
	int targetCol,targetRow,piece,dx,dy;
	
	
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			if (board[col][row]==color*6){ // king found on that square
			
				// check knight
				targetRow=row-2;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row-2;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row-1;targetCol=col-2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row-1;targetCol=col+2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row+1;targetCol=col-2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row+1;targetCol=col+2;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row+2;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}
				targetRow=row+2;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-3*color){return true;}

				// check king (this is necessary to avoid a king stepping next to the other king)
				targetRow=row-1;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row-1;targetCol=col+0;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row-1;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row+0;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row+0;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row+1;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row+1;targetCol=col+0;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}
				targetRow=row+1;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-6*color){return true;}


				//cout << "here\n";
				for (int i=0; i<4; i++){  //rook (and queen)
					if (i==0){dx=1;dy=0;}
					if (i==1){dx=-1;dy=0;}
					if (i==2){dx=0;dy=1;}
					if (i==3){dx=0;dy=-1;}
					for (int j=1; j<8; j++){
						targetRow=row+dy*j;targetCol=col+dx*j;piece=board[targetCol][targetRow]; 
						if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7){ // target square on board
							if (piece==-2*color){return true;}
							if (piece==-5*color){return true;}
							if (piece!=0){break;}
							
						}
					}
				}
				
				//cout << "here2\n";
				
				for (int i=0; i<4; i++){  //bishop (and queen)
					if (i==0){dx=1;dy=1;}
					if (i==1){dx=1;dy=-1;}
					if (i==2){dx=-1;dy=1;}
					if (i==3){dx=-1;dy=-1;}
					for (int j=1; j<8; j++){
						targetRow=row+dy*j;targetCol=col+dx*j;piece=board[targetCol][targetRow]; 
						if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7){ // target square on board
							if (piece==-4*color){return true;}
							if (piece==-5*color){return true;}
							if (piece!=0){break;}
							
						}
					}
				}
				
				
				//check pawn
				targetRow=row+color;targetCol=col-1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-1*color){return true;}
				targetRow=row+color;targetCol=col+1;piece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece==-1*color){return true;}
				
			}
		}
	}
	return false;
}


#endif