/**************************
*** function depends on ***
**************************/

#include "isKingInCheck.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "printMoves.h"

/************
*** Guard ***
************/

#ifndef assignMoveListAndSortFILE
#define assignMoveListAndSortFILE

/***************
*** Let's go ***
***************/


void assignMoveListAndSort (int board[8][8],unsigned long long int key, int moveList[], bool sortMoves){	// we use the 5 integer notation:
	// 1. column of start square
	// 2. row of start square
	// 3. column of end square
	// 4. row of end square
	// 5. abs(capturedPiece) + 16*abs(promotedTo)
	//    OR simply 128 if en passant
	
	countingMoveGenerationCalled++;
	int turn=int(-1+2*(key%2));
	int found=0;
	int piece,dx,dy,targetRow,targetCol,startpiece,endpiece,dir,piecesFoundInThatDirection,currentlyInCheck;
	bool plsDelete,markPinnedPiece;

	/****************************
	**** 1. finding own King ****
	****************************/
	
	int kingCol,kingRow;
	
	for (int col=0;col<8;col++){
		for (int row=0; row<8; row++){
			if (board[col][row]==6*turn){
				kingCol=col;
				kingRow=row;
			}
		}
	}
	
	/**********************************************
	**** 2. finding opponents attacked squares ****
	**********************************************/
	
	bool isAttacked[8][8];
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			isAttacked[col][row]=false;
		}
	}
	
	board[kingCol][kingRow]=0; // remove own king for the moment
	
	for (int row=0; row<8; row++){
		for (int col=0; col<8; col++){
			piece=board[col][row];
			if (piece*turn>=0){continue;} // searching for opponents pieces(!)
			
			if (abs(piece)==3){// knight
				if (col>1 && row>0){isAttacked[col-2][row-1]=true;}
				if (col>1 && row<7){isAttacked[col-2][row+1]=true;}
				if (col>0 && row>1){isAttacked[col-1][row-2]=true;}
				if (col>0 && row<6){isAttacked[col-1][row+2]=true;}
				if (col<7 && row>1){isAttacked[col+1][row-2]=true;}
				if (col<7 && row<6){isAttacked[col+1][row+2]=true;}
				if (col<6 && row>0){isAttacked[col+2][row-1]=true;}
				if (col<6 && row<7){isAttacked[col+2][row+1]=true;}
			}
			
			if (abs(piece)==6){// king
				if (col>0 && row>0){isAttacked[col-1][row-1]=true;}
				if (col>0         ){isAttacked[col-1][row  ]=true;}
				if (col>0 && row<7){isAttacked[col-1][row+1]=true;}
				if (         row>0){isAttacked[col  ][row-1]=true;}
				if (         row<7){isAttacked[col  ][row+1]=true;}
				if (col<7 && row>0){isAttacked[col+1][row-1]=true;}
				if (col<7         ){isAttacked[col+1][row  ]=true;}
				if (col<7 && row<7){isAttacked[col+1][row+1]=true;}
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
							isAttacked[targetCol][targetRow]=true;
							if (endpiece!=0){break;}
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
							isAttacked[targetCol][targetRow]=true;
							if (endpiece!=0){break;}
						} else {
							break;
						}
					}
				}
			}
			
			if (abs(piece)==1){// pawn
				if (col>=1){
					isAttacked[col-1][row-turn]=true;					
				}
				if (col<=6){
					isAttacked[col+1][row-turn]=true;					
				}
			}
			
		}	
	}
	
	
	
	board[kingCol][kingRow]=6*turn; // moving the king back on the board
	
	
	
	/****************************
	**** 3. are we in check? ****
	****************************/
	
	if (isAttacked[kingCol][kingRow]){
		currentlyInCheck=isKingInCheck(board,turn,kingCol,kingRow);
	} else {
		currentlyInCheck=0;
	}
	

	/*********************************
	**** 4. finding pinned pieces ****
	*********************************/
	
	// note: also marks the direction; also marks pieces of the opponent that are pinned to my king (important for en passant validation)
	int pinnedDirections[8][8]{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	//direction 0=N
	//direction 1=E
	//direction 2=NW
	//direction 3=NE
	
	for (int i=0; i<8; i++){
		dir=(i-i%2)/2; // beautiful simplicity
		dx=dx_LIST[i];
		dy=dy_LIST[i];
		
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
	
	
	
	/*******************************************************************************
	**** 5. pseudo-legal move generator (without any king moves and en passant) ****
	*******************************************************************************/
	
	// no moves are generated, that puts oneself into check.
	
	for (int row=0; row<8; row++){
		for (int col=0; col<8; col++){

			piece=board[col][row];
			
			if (piece*turn<=0){continue;} // there is indeed a piece of the right color here
			
			if (abs(piece)==3 && pinnedDirections[col][row]==-1){// knight
				targetRow=row-2;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol >=0 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-2;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-1;targetCol=col-2;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol >=0 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row-1;targetCol=col+2;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col-2;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol >=0 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+1;targetCol=col+2;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+2;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol >=0 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
				targetRow=row+2;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol <=7 && piece*endpiece<=0){	moveList[5*found+1]=col;moveList[5*found+2]=row;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
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
					dir=3-(turn+1)/2;
					if (endpiece*piece<0 && (pinnedDirections[col][row]==-1 || pinnedDirections[col][row]==dir)){ // there is something to capture and not pinned or pinned in the right direction
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

				//check if right capture is possible
				if (col<=6){
					endpiece=board[col+1][row+turn];
					dir=2+(turn+1)/2;
					if (endpiece*piece<0 && (pinnedDirections[col][row]==-1 || pinnedDirections[col][row]==dir)){ // there is something to capture and not pinned or pinned in the right direction
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
	
	
	
	
	/*********************************************
	**** 6. flaged illegal moves: if in check ****
	*********************************************/

	int deleted=0;
	
	
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
				if (board[targetCol][targetRow]!=0){break;}
			}
		}	
		
		for (int i=1;i<=found;i++){
			// will flag for deletion automatically, unless a piece lands on a blocking square (note: all king moves and en passant moves are not generated yet)
			
			plsDelete=true;
			if (blockingSquares[moveList[5*i-2]][moveList[5*i-1]]!=0){
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
				if (isKingInCheck(board,turn,kingCol,kingRow)==0){
					plsDelete=false;
				}
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
	
	
	
	/*************************************************
	**** 7. flaged illegal moves: if not in check ****
	*************************************************/

	// this section is empty :D
	
	/*********************************
	**** 8. deleting flaged moves ****
	*********************************/
	
	int move2position=1;
	if (deleted>0){
		for (int i=1; i<=5*found;i++){
			if (moveList[i]!=-100){
				moveList[move2position]=moveList[i];
				move2position++;
			}
		}
		found=found-deleted;
	}
	
	
	/*****************************
	**** 9. adding king moves ****
	*****************************/

	targetRow=kingRow-1;targetCol=kingCol-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow-1;targetCol=kingCol+0;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow-1;targetCol=kingCol+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow+0;targetCol=kingCol+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow+0;targetCol=kingCol-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow+1;targetCol=kingCol-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow+1;targetCol=kingCol+0;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	targetRow=kingRow+1;targetCol=kingCol+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetRow <=7 && targetCol>=0 && targetCol <=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){	moveList[5*found+1]=kingCol;moveList[5*found+2]=kingRow;moveList[5*found+3]=targetCol;moveList[5*found+4]=targetRow;moveList[5*found+5]=abs(endpiece);found++;}
	
	
	/*********************************
	**** 10. adding castles moves ****
	*********************************/
	
	
	if (turn==1 && currentlyInCheck==0){
		if ((key&4) && board[0][0]==2 && board[1][0]==0 && board[2][0]==0 && board[3][0]==0 && board[4][0]==6 && !isAttacked[2][0] && !isAttacked[3][0]){ //white queen side
			moveList[5*found+1]=4;moveList[5*found+2]=0;moveList[5*found+3]=2;moveList[5*found+4]=0;moveList[5*found+5]=0;found++;
		}
		if ((key&2) && board[4][0]==6 && board[5][0]==0 && board[6][0]==0 && board[7][0]==2 && !isAttacked[5][0] && !isAttacked[6][0]){ //white king side
			moveList[5*found+1]=4;moveList[5*found+2]=0;moveList[5*found+3]=6;moveList[5*found+4]=0;moveList[5*found+5]=0;found++;
		}
	}
	if (turn==-1 && currentlyInCheck==0){
		if ((key&16) && board[0][7]==-2 && board[1][7]==0 && board[2][7]==0 && board[3][7]==0 && board[4][7]==-6 && !isAttacked[2][7] && !isAttacked[3][7]){ //black queen side
			moveList[5*found+1]=4;moveList[5*found+2]=7;moveList[5*found+3]=2;moveList[5*found+4]=7;moveList[5*found+5]=0;found++;
		}
		if ((key&8) && board[4][7]==-6 && board[5][7]==0 && board[6][7]==0 && board[7][7]==-2 && !isAttacked[5][7] && !isAttacked[6][7]){ //black king side
			moveList[5*found+1]=4;moveList[5*found+2]=7;moveList[5*found+3]=6;moveList[5*found+4]=7;moveList[5*found+5]=0;found++;
		}
	}
	
	/******************************
	**** 11. adding en passant ****
	******************************/
	

	int startRow;
	int pawnColumn;
	int result;
	if (key&32){
		pawnColumn=int((key>>6)&7);
		startRow=3+(key&1);
		if (pawnColumn>=1){ // check possible capture from left
			if (board[pawnColumn-1][startRow]==turn){ // there is indeed a pawn there -> en passant is possible
				moveList[5*found+1]=pawnColumn-1;moveList[5*found+2]=startRow;moveList[5*found+3]=pawnColumn;moveList[5*found+4]=startRow+turn;moveList[5*found+5]=128;
				
				// need to check if the move is indeed legal, if my pawn was pinned, if my opponent's pawn was pinned or if the pawns are on the same row as my king or if we are in check
				if ((pinnedDirections[pawnColumn-1][startRow]!=-1) || (pinnedDirections[pawnColumn][startRow]!=-1) || (startRow==kingRow) || (currentlyInCheck!=0)){
					assignMakeMove(board,moveList[5*found+1],moveList[5*found+2],moveList[5*found+3],moveList[5*found+4],moveList[5*found+5]);
					result=isKingInCheck(board,turn,kingCol,kingRow);
					assignUndoMove(board,moveList[5*found+1],moveList[5*found+2],moveList[5*found+3],moveList[5*found+4],moveList[5*found+5]);

					if (result==0){
						found++;
					}
				} else {
					found++; // only when found is increased is the move "recorded"
				}
				
			}
		}
		
		if (pawnColumn<=6){ // check possible capture from right
			if (board[pawnColumn+1][startRow]==turn){ // there is indeed a pawn there -> en passant is possible
				moveList[5*found+1]=pawnColumn+1;moveList[5*found+2]=startRow;moveList[5*found+3]=pawnColumn;moveList[5*found+4]=startRow+turn;moveList[5*found+5]=128;
				
				// need to check if the move is indeed legal, if my pawn was pinned, if my opponent's pawn was pinned or if the pawns are on the same row as my king
				if ((pinnedDirections[pawnColumn+1][startRow]!=-1) || (pinnedDirections[pawnColumn][startRow]!=-1) || (startRow==kingRow) || (currentlyInCheck!=0)){
					assignMakeMove(board,moveList[5*found+1],moveList[5*found+2],moveList[5*found+3],moveList[5*found+4],moveList[5*found+5]);
					result=isKingInCheck(board,turn,kingCol,kingRow);
					assignUndoMove(board,moveList[5*found+1],moveList[5*found+2],moveList[5*found+3],moveList[5*found+4],moveList[5*found+5]);

					if (result==0){
						found++;
					}
				} else {
					found++; // only when found is increased is the move "recorded"
				}


			}
		}
	}
	
	moveList[0]=found; // finally assign moveList[0] to the number of moves
	moveList[5*found+1]=isAttacked[kingCol][kingRow]; // add an extra number indicating if we are in check
	
	
	
	/*******************************************
	**** 12. Moveordering: evaluating moves ****
	*******************************************/
	
	
	if (!sortMoves){return;} // turning off moveordering

	int quickEvals[250]; // will start with move 1
	int quickEval;

	int startPieceValue,endPieceValue;
	for (int i=1; i<=moveList[0]; i++){
		startpiece=board[moveList[5*i-4]][moveList[5*i-3]];
		endpiece=board[moveList[5*i-2]][moveList[5*i-1]];
		
		startPieceValue=abs(startpiece);
		if (startPieceValue>=5){startPieceValue=10;}
		if (startPieceValue==2){startPieceValue=5;}
		if (startPieceValue==4){startPieceValue=3;}
		


		endPieceValue=abs(endpiece);
		if (endPieceValue>=5){endPieceValue=10;} // however, endpiece==King should not be possible
		if (endPieceValue==2){endPieceValue=5;}
		if (endPieceValue==4){endPieceValue=3;}
		

		
		// here come the elo points: //
		
		
		quickEvals[i]=endPieceValue                                                               // bonus for capturing a piece
					-startPieceValue*(isAttacked[moveList[5*i-2]][moveList[5*i-1]])               // bonus if the square we are moving to is not attacked
					+3*(startPieceValue>=6)*isAttacked[moveList[5*i-4]][moveList[5*i-3]]          // bonus if we move a queen or a rook and the startsquare was attacked
					+(PST_average[6+startpiece][moveList[5*i-4]][moveList[5*i-3]]*turn <  
					PST_average[6+startpiece][moveList[5*i-2]][moveList[5*i-1]]*turn)        // minimal bonus if moving to a better PST square
					+10;                                                             	          // offset: now takes values from 0-10+0+0+10=0 to 10-0+3+1+10=24

		
		
		if ((quickEvals[i]<0) || (quickEvals[i]>24)){
			cout << "ono, we have a eval of "<< quickEvals[i]<< "\n";
			cout << endpiece << "\n";
			cout << startpiece << "\n";
			cout << isAttacked[moveList[5*i-2]][moveList[5*i-1]] << "\n";
			cout << startpiece*(!isAttacked[moveList[5*i-2]][moveList[5*i-1]]) << "\n";
			cout << "\n";
		}
		
		
		//printMove(moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i]);
		//cout << ":  "<<quickEvals[i]<<"\n";
		// could also incorporate en passant
		// could also incorporate promotions
		// could also incorporate moving away from attacked squares, e.g. saving an attacked queen
		// could also incorporate (with additional work) being attacked by a pawn or a different piece
		// could also incorporate (with additional work) giving a check
		// could also incorporate (with additional work) the PST
	}
	
	
	/*******************************************
	**** 13. Moveordering: reordering moves ****
	*******************************************/
	
	
	
	int sortingTable[24+1][219];
	// will use entries sortingTable[i][0] to store how many entries sortingTable[i][.] has

	for (int i=0;i<=24;i++){
		sortingTable[i][0]=0;
	}	

	for (int i=1; i<=found; i++){
		quickEval=quickEvals[i];
		sortingTable[quickEval][0]++;
		sortingTable[quickEval][sortingTable[quickEval][0]]=i;
	}

	int moveListCopy[250*5];
	for (int i=0; i<=5*found; i++){
		moveListCopy[i]=moveList[i];
	}

	int filled=0;
	int hv;
	for (int eval=24; eval>=0; eval--){
		for (int j=1;j<=sortingTable[eval][0];j++){
			hv=sortingTable[eval][j];
			moveList[5*filled+1]=moveListCopy[hv*5-4];
			moveList[5*filled+2]=moveListCopy[hv*5-3];
			moveList[5*filled+3]=moveListCopy[hv*5-2];
			moveList[5*filled+4]=moveListCopy[hv*5-1];
			moveList[5*filled+5]=moveListCopy[hv*5-0];
			filled++;
		}
	}


	return;


}

#endif