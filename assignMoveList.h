/**************************
*** function depends on ***
**************************/

#include "isKingInCheck.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"

/************
*** Guard ***
************/

#ifndef assignMoveListFILE
#define assignMoveListFILE

/***************
*** Let's go ***
***************/


void assignMoveList (int board[8][8],unsigned long long int key, int moveList[], bool strongLegal){
	// we use the 5 integer notation:
	// 1. column of start square
	// 2. row of start square
	// 3. column of end square
	// 4. row of end square
	// 5. abs(capturedPiece) + 16*abs(promotedTo)
	//    OR simply 128 if en pessent
	
	countingMoveGenerationCalled++;
	
	int turn=int(-1+2*(key%2));


	int found=0;
	int promotion;
	int piece;
	int dx;
	int dy;
	int targetRow;
	int targetCol;
	int endpiece;
	bool plsDelete;
	
	int row;
	for (int rowBAR=0; rowBAR<8; rowBAR++){
		row=7-rowBAR; // this part is to have the same move ordering for white and black
		if (turn==1){row=rowBAR;}
		
		for (int col=0; col<8; col++){

			piece=board[col][row];
			
			if (piece*turn<=0){continue;} // there is indeed a piece of the right color here
			
			if (abs(piece)==3){// knight
				targetRow=row-2;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-2;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-1;targetCol=col-2;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-1;targetCol=col+2;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col-2;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col+2;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+2;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+2;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
			}
			
			if (abs(piece)==6){// king
				targetRow=row-1;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-1;targetCol=col+0;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-1;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+0;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+0;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col+0;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
			}
			
			
			if (abs(piece)==2 || abs(piece)==5){// rook (or queen)
				for (int i=0; i<4; i++){
					if (i==0){dx=1;dy=0;}
					if (i==1){dx=-1;dy=0;}
					if (i==2){dx=0;dy=1;}
					if (i==3){dx=0;dy=-1;}
					for (int j=1; j<8; j++){
						targetRow=row+dy*j;targetCol=col+dx*j;endpiece=board[targetCol][targetRow]; 
						if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7){ // target square on board
							if (endpiece==0){moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=0;found++;}
							if (piece*endpiece<0){moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;;moveList[5*found+5]=abs(endpiece);found++;break;}
							if (piece*endpiece>0){break;}
						} else {
							break;
						}
					}
				}
			}
			
			if (abs(piece)==4 || abs(piece)==5){// bishop (or queen)
				for (int i=0; i<4; i++){
					if (i==0){dx=-1;dy=-1;}
					if (i==1){dx=-1;dy=1;}
					if (i==2){dx=1;dy=-1;}
					if (i==3){dx=1;dy=1;}
					for (int j=1; j<8; j++){
						targetRow=row+dy*j;targetCol=col+dx*j;endpiece=board[targetCol][targetRow]; 
						if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7){ // target square on board
							if (endpiece==0){moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=0;found++;}
							if (piece*endpiece<0){moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;break;}
							if (piece*endpiece>0){break;}
						} else {
							break;
						}
					}
				}
			}
			
			if (abs(piece)==1){// pawn
				if (board[col][row+turn]==0){ //1 move push is possible
					
					
					if (row+turn==0 || row+turn==7){// this move is a promotion
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+turn;moveList[5*found+5]=16*5;found++;
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+turn;moveList[5*found+5]=16*3;found++;
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+turn;moveList[5*found+5]=16*2;found++;
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+turn;moveList[5*found+5]=16*4;found++;
					} else {
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+turn;moveList[5*found+5]=0;found++;
					}
					
					
					// check if pawn on start square + if 2 move push possible
					if (row==1 && turn==1 && board[col][row+2*turn]==0){
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+2*turn;moveList[5*found+5]=0;found++;
					}
					if (row==6 && turn==-1 && board[col][row+2*turn]==0){
						moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col;moveList[5*found+4]=row+2*turn;moveList[5*found+5]=0;found++;
					}

				}

				//check if left capture is possible
				if (col>=1){
					endpiece=board[col-1][row+turn];
					if (endpiece*piece<0){ // there is something to capture
						if (row+turn==0 || row+turn==7){ // this is additionally a promotion
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col-1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*5;found++;
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col-1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*3;found++;
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col-1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*2;found++;
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col-1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*4;found++;
						} else {
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col-1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece);found++;
						}
					}
				}

				//check if left capture is possible
				if (col<=6){
					endpiece=board[col+1][row+turn];
					if (endpiece*piece<0){ // there is something to capture
						if (row+turn==0 || row+turn==7){ // this is additionally a promotion
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col+1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*5;found++;
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col+1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*3;found++;
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col+1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*2;found++;
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col+1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece)+16*4;found++;
						} else {
							moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=col+1;moveList[5*found+4]=row+turn;moveList[5*found+5]=abs(endpiece);found++;
						}
					}
				}
			}
			
		}	
	}

	// castles - we will check for castles through check only when we consider strong legal moves.
	if (turn==1){
		if ((key&4) && board[0][0]==2 && board[1][0]==0 && board[2][0]==0 && board[3][0]==0 && board[4][0]==6){ //white queen side
			moveList[5*found+1]=4;moveList[5*found+2]=0;moveList[5*found+3]=2;moveList[5*found+4]=0;moveList[5*found+5]=0;found++;
		}
		if ((key&2) && board[4][0]==6 && board[5][0]==0 && board[6][0]==0 && board[7][0]==2){ //white king side
			moveList[5*found+1]=4;moveList[5*found+2]=0;moveList[5*found+3]=6;moveList[5*found+4]=0;moveList[5*found+5]=0;found++;
		}
	}
	if (turn==-1){
		if ((key&16) && board[0][7]==-2 && board[1][7]==0 && board[2][7]==0 && board[3][7]==0 && board[4][7]==-6){ //black queen side
			moveList[5*found+1]=4;moveList[5*found+2]=7;moveList[5*found+3]=2;moveList[5*found+4]=7;moveList[5*found+5]=0;found++;
		}
		if ((key&8) && board[4][7]==-6 && board[5][7]==0 && board[6][7]==0 && board[7][7]==-2){ //black king side
			moveList[5*found+1]=4;moveList[5*found+2]=7;moveList[5*found+3]=6;moveList[5*found+4]=7;moveList[5*found+5]=0;found++;
		}
	}

	// en pessant:
	int startRow;
	int pawnColumn;
	if (key&32){
		pawnColumn=int((key>>6)&7);
		startRow=3+(key&1); ///HERE MIGHT BE A MISTAKE
		//cout << "will search for an epass in column"<< pawnColumn<< " and row "<< startRow<< "\n";
		if (pawnColumn>=1){ // check possible capture from left
			//cout << "I am here\n";
			//cout << "turn: "<< turn <<"\n";
			//cout << "pawnColumn-1: "<< pawnColumn-1 <<"\n";
			//cout << "startRow: "<< startRow <<"\n";
			
			if (board[pawnColumn-1][startRow]==turn){ // there is indeed a pawn there -> en pessant is possible
				//cout << "I am also here\n";
				moveList[5*found+1]=pawnColumn-1;moveList[5*found+2]=startRow;moveList[5*found+3]=pawnColumn;moveList[5*found+4]=startRow+turn;moveList[5*found+5]=128;found++;
			}
		}
		
		if (pawnColumn<=6){ // check possible capture from right
			if (board[pawnColumn+1][startRow]==turn){ // there is indeed a pawn there -> en pessant is possible
				moveList[5*found+1]=pawnColumn+1;moveList[5*found+2]=startRow;moveList[5*found+3]=pawnColumn;moveList[5*found+4]=startRow+turn;moveList[5*found+5]=128;found++;
			}
		}
	}
	
	
	

	int deleted=0;
	int move2position=1;
	//bool isKingCurrentlyInCheck;
	//isKingCurrentlyInCheck=isKingInCheck(board,turn);
	
	if (strongLegal==true){ // want only legal moves (duh)
		for (int i=1;i<=found;i++){     // will iterate over all moves currently found; will replace ones with -100,-100,-100,-100,-100 if not legal
			plsDelete=false;			
			
			// check if castles out of check
			if (board[moveList[5*i-4]][moveList[5*i-3]]==6*turn){//king move
				///////////////////cout << "a king move -> possibly castles move\n";
				if (abs(moveList[5*i-4]-moveList[5*i-2])>1){ // king moves horizontally more than one square (aka castles)
					///////////////////cout << "a castles move -> "<<i<<"\n";
					if (isKingInCheck(board,turn)==true){ //wanted to castle out of check
						plsDelete=true;
					}
					
					// check if castles around a protected square
					// will sneakily move the king onto that possible protected square -> check if check -> move King back
					
					board[(moveList[5*i-4]+moveList[5*i-2])/2][moveList[5*i-3]]=6*turn; // moved king one square
					board[moveList[5*i-4]][moveList[5*i-3]]=0; // replace original king
					if (isKingInCheck(board,turn)==true){ //wanted to castle out of check
						plsDelete=true;
					}
					board[(moveList[5*i-4]+moveList[5*i-2])/2][moveList[5*i-3]]=0;
					board[moveList[5*i-4]][moveList[5*i-3]]=6*turn;
				}
			}
			
			// don't move into check
			assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			if (isKingInCheck(board,turn)){   // my King is in check after my next move
				plsDelete=true;
			}
			assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			
			if (plsDelete){ // was there a flag
				///////////////////cout << "want to delete move "<<i<<"\n";
				deleted++;
				moveList[5*i-4]=-100;
				moveList[5*i-3]=-100;
				moveList[5*i-2]=-100;
				moveList[5*i-1]=-100;
				moveList[5*i-0]=-100;
			}
			
			
		}

		// now move moves to the left, replacing the -100 moves
	

		for (int i=1; i<=5*found;i++){
			if (moveList[i]!=-100){
				moveList[move2position]=moveList[i];
				move2position++;
			}
		}

	}
	found=found-deleted;
	moveList[0]=found;
}

#endif