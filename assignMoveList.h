/**************************
*** function depends on ***
**************************/

#include "isKingInCheck.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "globalVariables.h"

/************
*** Guard ***
************/

#ifndef assignMoveListFILE
#define assignMoveListFILE

/***************
*** Let's go ***
***************/


void assignMoveList (int board[8][8],unsigned long long int key, int moveList[]){
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
	int piece,dx,dy,targetRow,targetCol,endpiece,row,dir,kingCol,kingRow,piecesFoundInThatDirection;
	bool plsDelete,markPinnedPiece;
	

	for (int r=0;r<8;r++){
		for (int c=0; c<8; c++){
			if (board[c][r]==6*turn){
				kingCol=c;
				kingRow=r;
			}
		}
	}
	//cout << "King found at "<< kingCol << " "<< kingRow<< "\n";
	
	int currentlyInCheck;
	currentlyInCheck=isKingInCheck(board,turn,kingCol,kingRow);
	
	
	int pinnedDirections[8][8]{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	//direction 0=N
	//direction 1=E
	//direction 2=NW
	//direction 3=NE
	
	
	for (int i=0; i<8; i++){
		if (i==0){dx=0 ;dy= 1;dir=0;}//N
		if (i==1){dx=0 ;dy=-1;dir=0;}//S
		if (i==2){dx= 1;dy= 0;dir=1;}//E
		if (i==3){dx=-1;dy= 0;dir=1;}//W
		if (i==4){dx=-1;dy= 1;dir=2;}//NW
		if (i==5){dx= 1;dy=-1;dir=2;}//SE
		if (i==6){dx= 1;dy= 1;dir=3;}//NE
		if (i==7){dx=-1;dy=-1;dir=3;}//SW

		markPinnedPiece=false;
		piecesFoundInThatDirection=0;
		for (int j=1; j<8; j++){
			targetRow=kingRow+dy*j;
			targetCol=kingCol+dx*j;
			if (targetRow<0 || targetRow >7 || targetCol<0 || targetCol >7){break;} // target square not on board
			
			if (board[targetCol][targetRow]!=0){
				piecesFoundInThatDirection++;
				if (piecesFoundInThatDirection==2){
					if (board[targetCol][targetRow]==-turn*moveGenerationHelper[dir] || board[targetCol][targetRow]==-turn*5){
						markPinnedPiece=true;
					}
					break;
				}
			}
		}
		
		if (markPinnedPiece==true){
			for (int j=1; j<8; j++){
				targetRow=kingRow+dy*j;
				targetCol=kingCol+dx*j;
				if (board[targetCol][targetRow]!=0){
					pinnedDirections[targetCol][targetRow]=dir;
					break;
				}
			}
		}
	}
	
	
	/************************************
	**** pseudo-legal move generator ****
	************************************/
	
	for (int rowBAR=0; rowBAR<8; rowBAR++){
		row=7-rowBAR; // this part is to have the same move ordering for white and black
		if (turn==1){row=rowBAR;}
		
		for (int col=0; col<8; col++){

			piece=board[col][row];
			
			if (piece*turn<=0){continue;} // there is indeed a piece of the right color here
			
			if (abs(piece)==3 && pinnedDirections[col][row]==-1){// knight
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
					if ((i==0||i==1) && pinnedDirections[col][row]!=-1 && pinnedDirections[col][row]!=0){continue;}
					if ((i==2||i==3) && pinnedDirections[col][row]!=-1 && pinnedDirections[col][row]!=1){continue;}
					if (i==0){dx=0 ;dy=+1;}  //N
					if (i==1){dx=0 ;dy=-1;}  //S
					if (i==2){dx= 1;dy=0;}   //E
					if (i==3){dx=-1;dy=0;}   //W

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
					if ((i==0||i==1) && pinnedDirections[col][row]!=-1 && pinnedDirections[col][row]!=2){continue;}
					if ((i==2||i==3) && pinnedDirections[col][row]!=-1 && pinnedDirections[col][row]!=3){continue;}
					if (i==0){dx=-1;dy= 1;}  //NW
					if (i==1){dx= 1;dy=-1;}  //NW
					if (i==2){dx= 1;dy= 1;}  //NE
					if (i==3){dx=-1;dy=-1;}  //NE
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
				if (board[col][row+turn]==0 && (pinnedDirections[col][row]==-1 || pinnedDirections[col][row]==0)){ //1 move push is possible and not pinned
					
					
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
	if (turn==1 && currentlyInCheck==0){
		if ((key&4) && board[0][0]==2 && board[1][0]==0 && board[2][0]==0 && board[3][0]==0 && board[4][0]==6){ //white queen side
			moveList[5*found+1]=4;moveList[5*found+2]=0;moveList[5*found+3]=2;moveList[5*found+4]=0;moveList[5*found+5]=0;found++;
		}
		if ((key&2) && board[4][0]==6 && board[5][0]==0 && board[6][0]==0 && board[7][0]==2){ //white king side
			moveList[5*found+1]=4;moveList[5*found+2]=0;moveList[5*found+3]=6;moveList[5*found+4]=0;moveList[5*found+5]=0;found++;
		}
	}
	if (turn==-1 && currentlyInCheck==0){
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
		startRow=3+(key&1);
		if (pawnColumn>=1){ // check possible capture from left
			if (board[pawnColumn-1][startRow]==turn){ // there is indeed a pawn there -> en pessant is possible
				moveList[5*found+1]=pawnColumn-1;moveList[5*found+2]=startRow;moveList[5*found+3]=pawnColumn;moveList[5*found+4]=startRow+turn;moveList[5*found+5]=128;found++;
			}
		}
		
		if (pawnColumn<=6){ // check possible capture from right
			if (board[pawnColumn+1][startRow]==turn){ // there is indeed a pawn there -> en pessant is possible
				moveList[5*found+1]=pawnColumn+1;moveList[5*found+2]=startRow;moveList[5*found+3]=pawnColumn;moveList[5*found+4]=startRow+turn;moveList[5*found+5]=128;found++;
			}
		}
	}
	



	/************************************************
	**** deleting illegal moves: if not in check ****
	************************************************/
	
	int deleted=0;
	int move2position=1;
	bool need2check;
	
	if (currentlyInCheck==0){	
		for (int i=1;i<=found;i++){     //will replace illegal moves with -100,-100,-100,-100,-100

			// only king moves and (pawn captures of pinned pawns) and en pessent need to be checked
			need2check=false;
			if (abs(board[moveList[5*i-4]][moveList[5*i-3]])==6){need2check=true;}  // king move
			if (abs(board[moveList[5*i-4]][moveList[5*i-3]])==1 && moveList[5*i-4]!=moveList[5*i-2] && pinnedDirections[moveList[5*i-4]][moveList[5*i-3]]!=-1){need2check=true;}  // pawn capture of pinned pawn
			if (moveList[5*i-0]==128){need2check=true;}  // en pessant
			
			if (!need2check){continue;} 
			plsDelete=false;		
			
			
			// check if castles out of check
			if (board[moveList[5*i-4]][moveList[5*i-3]]==6*turn){//king move
				if (abs(moveList[5*i-4]-moveList[5*i-2])>1){ // king moves horizontally more than one square (aka castles)
					
					// check if castles around a protected square
					// will sneakily move the king onto that possible protected square -> check if check -> move King back
					
					board[(moveList[5*i-4]+moveList[5*i-2])/2][moveList[5*i-3]]=6*turn; // moved king one square
					board[moveList[5*i-4]][moveList[5*i-3]]=0; // replace original king
					if (isKingInCheck(board,turn,(moveList[5*i-4]+moveList[5*i-2])/2,moveList[5*i-3])!=0){ //wanted to castle out of check
						plsDelete=true;
					}
					board[(moveList[5*i-4]+moveList[5*i-2])/2][moveList[5*i-3]]=0;
					board[moveList[5*i-4]][moveList[5*i-3]]=6*turn;
				}
			}
			
			// will now check that I'm not in check after the move.
			// after move should not be in check. should distinguish between king moves and non-king moves
			
			if (abs(board[moveList[5*i-4]][moveList[5*i-3]])==6){

				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				if (isKingInCheck(board,turn,moveList[5*i-2],moveList[5*i-1])!=0){plsDelete=true;}
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				
			} else {
				
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				if (isKingInCheck(board,turn,kingCol,kingRow)!=0){plsDelete=true;}
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			}
			
			
			if (plsDelete){ // was there a flag
				deleted++;
				moveList[5*i-4]=-100;
				moveList[5*i-3]=-100;
				moveList[5*i-2]=-100;
				moveList[5*i-1]=-100;
				moveList[5*i-0]=-100;
			}
		}
	}
	
	/********************************************
	**** deleting illegal moves: if in check ****
	********************************************/

	if (currentlyInCheck!=0){
		// will calculate blocking squares:
		int blockingSquares[8][8]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		
		int attackingCol=(currentlyInCheck-1)%8;
		int attackingRow=(currentlyInCheck-1-attackingCol)/8;
		
		
		if (abs(attackingCol-kingCol)*abs(attackingRow-kingRow)==2){ // knight move
			blockingSquares[attackingCol][attackingRow]=1;
		} else {
			dx=0;
			if (attackingCol-kingCol>0){dx=1;}
			if (attackingCol-kingCol<0){dx=-1;}
			dy=0;
			if (attackingRow-kingRow>0){dy=1;}
			if (attackingRow-kingRow<0){dy=-1;}

			for (int j=1; j<8; j++){
				targetRow=kingRow+dy*j;targetCol=kingCol+dx*j;
				blockingSquares[targetCol][targetRow]=1;
				//cout << targetCol << " " << targetRow << "was set to blockingSquares\n";
				if (board[targetCol][targetRow]!=0){break;}
			}
		}	
		
		for (int i=1;i<=found;i++){     //will replace illegal moves with -100,-100,-100,-100,-100
			// will need to check moves that land on a blocking square or enpessent or king moves; other moves will be deleted
			
			plsDelete=true;
			if (blockingSquares[moveList[5*i-2]][moveList[5*i-1]]!=0 && board[moveList[5*i-4]][moveList[5*i-3]]!=6*turn){ // move to a blocking square by a non-king				
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				if (isKingInCheck(board,turn,kingCol,kingRow)==0){plsDelete=false;}
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			}
			if (board[moveList[5*i-4]][moveList[5*i-3]]==6*turn){ // it was a king move
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				if (isKingInCheck(board,turn,moveList[5*i-2],moveList[5*i-1])==0){plsDelete=false;}
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			}
			if (moveList[5*i-0]==128){ // it was an en pessent
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				if (isKingInCheck(board,turn,kingCol,kingRow)==0){plsDelete=false;}
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
			}
			
			if (plsDelete){	
				deleted++;
				moveList[5*i-4]=-100;
				moveList[5*i-3]=-100;
				moveList[5*i-2]=-100;
				moveList[5*i-1]=-100;
				moveList[5*i-0]=-100;
			}
			
		}
	}	
	
	
	/******************************
	**** deleting flaged moves ****
	******************************/
	
	// now move moves to the left, replacing the -100 moves

	for (int i=1; i<=5*found;i++){
		if (moveList[i]!=-100){
			moveList[move2position]=moveList[i];
			move2position++;
		}
	}

	
	found=found-deleted;
	moveList[0]=found;
}

#endif