/**************************
*** function depends on ***
**************************/

#include "isKingInCheck.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "printingStuff.h"

/************
*** Guard ***
************/

#ifndef assignMoveListAndSortFILE
#define assignMoveListAndSortFILE

/***************
*** Let's go ***
***************/

// there are 4 directions: N,E, NW, NW in that order. Here in the list are the pieces marked that can go that way (i.e. rook, rook, bishop, bishop)
const int moveGenerationHelper[4]={2,2,4,4};
// directions in that order N,S,E,W,NW,SE,NE,SW,8 knight directions
const int dx_LIST[16]={ 0, 0, 1,-1,-1, 1, 1,-1,-2,-2,-1,-1,+1,+1,+2,+2};
const int dy_LIST[16]={ 1,-1, 0, 0, 1,-1, 1,-1,-1,+1,-2,+2,-2,+2,-1,+1};



void assignMoveListAndSort (int board[8][8],unsigned long long int key, unsigned int moveList[], int sortMoves, unsigned short int thisMoveFirst=0){
	// The heart of any chess engine!
	
	// all legal moves are assigned to "moveList"
	// a move is stored as a 32-bit number, where only the last 19 bits are used
	// it is of the form 7.666.555.444.333.222.111
	
	// 111: start column
	// 222: start row
	// 333: end column
	// 444: end row
	// 555: abs(promotedTo)
	// 666: abs(capturedPiece)
	// 7: is en passant
	
	// this way is chosen, because for a fixed position the rightmost 16 bits of the moves are unique
	// hence 2 Byte suffice to identify any move in a given position
	
	
	movegen++;
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
	bool isPawnAttacked[8][8];
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			isAttacked[col][row]=false;
			isPawnAttacked[col][row]=false;
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
					isPawnAttacked[col-1][row-turn]=true;
				}
				if (col<=6){
					isAttacked[col+1][row-turn]=true;					
					isPawnAttacked[col+1][row-turn]=true;
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
	int pinnedDirections[8][8]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
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
				targetRow=row-2;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol >=0 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row-2;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol <=7 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row-1;targetCol=col-2;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol >=0 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row-1;targetCol=col+2;endpiece=board[targetCol][targetRow]; if (targetRow >=0 && targetCol <=7 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row+1;targetCol=col-2;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol >=0 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row+1;targetCol=col+2;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol <=7 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row+2;targetCol=col-1;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol >=0 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
				targetRow=row+2;targetCol=col+1;endpiece=board[targetCol][targetRow]; if (targetRow <=7 && targetCol <=7 && piece*endpiece<=0){	moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;}
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
							if (endpiece==0){moveList[found+1]=col+row*8+targetCol*64+targetRow*512;found++;}
							if (piece*endpiece<0){moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;break;}
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
							if (endpiece==0){moveList[found+1]=col+row*8+targetCol*64+targetRow*512;found++;}
							if (piece*endpiece<0){moveList[found+1]=col+row*8+targetCol*64+targetRow*512+abs(endpiece)*32768;found++;break;}
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
						moveList[found+1]=col+row*8+col*64+(row+turn)*512+5*4096;found++;
						moveList[found+1]=col+row*8+col*64+(row+turn)*512+3*4096;found++;
						moveList[found+1]=col+row*8+col*64+(row+turn)*512+2*4096;found++;
						moveList[found+1]=col+row*8+col*64+(row+turn)*512+4*4096;found++;
					} else {
						moveList[found+1]=col+row*8+col*64+(row+turn)*512;found++;
					}
					
					
					// check if pawn on start square + if 2 move push possible
					if (row==1 && turn==1 && board[col][row+2*turn]==0){
						moveList[found+1]=col+row*8+col*64+(row+2*turn)*512;found++;
					}
					if (row==6 && turn==-1 && board[col][row+2*turn]==0){
						moveList[found+1]=col+row*8+col*64+(row+2*turn)*512;found++;
					}
					
				}

				//check if left capture is possible
				if (col>=1){
					endpiece=board[col-1][row+turn];
					dir=3-(turn+1)/2;
					if (endpiece*piece<0 && (pinnedDirections[col][row]==-1 || pinnedDirections[col][row]==dir)){ // there is something to capture and not pinned or pinned in the right direction
						if (row+turn==0 || row+turn==7){ // this is additionally a promotion
							moveList[found+1]=col+row*8+(col-1)*64+(row+turn)*512+5*4096+32768*abs(endpiece);found++;
							moveList[found+1]=col+row*8+(col-1)*64+(row+turn)*512+3*4096+32768*abs(endpiece);found++;
							moveList[found+1]=col+row*8+(col-1)*64+(row+turn)*512+2*4096+32768*abs(endpiece);found++;
							moveList[found+1]=col+row*8+(col-1)*64+(row+turn)*512+4*4096+32768*abs(endpiece);found++;
						} else {
							moveList[found+1]=col+row*8+(col-1)*64+(row+turn)*512+32768*abs(endpiece);found++;
						}
					}
				}

				//check if right capture is possible
				if (col<=6){
					endpiece=board[col+1][row+turn];
					dir=2+(turn+1)/2;
					if (endpiece*piece<0 && (pinnedDirections[col][row]==-1 || pinnedDirections[col][row]==dir)){ // there is something to capture and not pinned or pinned in the right direction
						if (row+turn==0 || row+turn==7){ // this is additionally a promotion
							moveList[found+1]=col+row*8+(col+1)*64+(row+turn)*512+5*4096+32768*abs(endpiece);found++;
							moveList[found+1]=col+row*8+(col+1)*64+(row+turn)*512+3*4096+32768*abs(endpiece);found++;
							moveList[found+1]=col+row*8+(col+1)*64+(row+turn)*512+2*4096+32768*abs(endpiece);found++;
							moveList[found+1]=col+row*8+(col+1)*64+(row+turn)*512+4*4096+32768*abs(endpiece);found++;
						} else {
							moveList[found+1]=col+row*8+(col+1)*64+(row+turn)*512+32768*abs(endpiece);found++;
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
			if (blockingSquares[(moveList[i]>>6)&0b111][(moveList[i]>>9)&0b111]!=0){
				assignMakeMove(board,moveList[i]);
				if (isKingInCheck(board,turn,kingCol,kingRow)==0){
					plsDelete=false;
				}
				assignUndoMove(board,moveList[i]);
			}
			
			if (plsDelete){	
				deleted++;
				moveList[i]=-100;
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
		for (int i=1; i<=found;i++){
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

	targetRow=kingRow-1;targetCol=kingCol-1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetCol>=0 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow-1;targetCol=kingCol+0;endpiece=board[targetCol][targetRow]; if (targetRow>=0                 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow-1;targetCol=kingCol+1;endpiece=board[targetCol][targetRow]; if (targetRow>=0 && targetCol<=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow+0;targetCol=kingCol-1;endpiece=board[targetCol][targetRow]; if (                targetCol>=0 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow+0;targetCol=kingCol+1;endpiece=board[targetCol][targetRow]; if (                targetCol<=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow+1;targetCol=kingCol-1;endpiece=board[targetCol][targetRow]; if (targetRow<=7 && targetCol>=0 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow+1;targetCol=kingCol+0;endpiece=board[targetCol][targetRow]; if (targetRow<=7                 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	targetRow=kingRow+1;targetCol=kingCol+1;endpiece=board[targetCol][targetRow]; if (targetRow<=7 && targetCol<=7 && turn*endpiece<=0 && !isAttacked[targetCol][targetRow]){ moveList[found+1]=kingCol+kingRow*8+targetCol*64+targetRow*512+32768*abs(endpiece);found++;}
	
	
	/*********************************
	**** 10. adding castles moves ****
	*********************************/
	
	
	if (turn==1 && currentlyInCheck==0){
		if ((key&4) && board[0][0]==2 && board[1][0]==0 && board[2][0]==0 && board[3][0]==0 && board[4][0]==6 && !isAttacked[2][0] && !isAttacked[3][0]){ //white queen side
			moveList[found+1]=4+0*8+2*64+0*512;found++;
		}
		if ((key&2) && board[4][0]==6 && board[5][0]==0 && board[6][0]==0 && board[7][0]==2 && !isAttacked[5][0] && !isAttacked[6][0]){ //white king side
			moveList[found+1]=4+0*8+6*64+0*512;found++;
		}
	}
	if (turn==-1 && currentlyInCheck==0){
		if ((key&16) && board[0][7]==-2 && board[1][7]==0 && board[2][7]==0 && board[3][7]==0 && board[4][7]==-6 && !isAttacked[2][7] && !isAttacked[3][7]){ //black queen side
			moveList[found+1]=4+7*8+2*64+7*512;found++;
		}
		if ((key&8) && board[4][7]==-6 && board[5][7]==0 && board[6][7]==0 && board[7][7]==-2 && !isAttacked[5][7] && !isAttacked[6][7]){ //black king side
			moveList[found+1]=4+7*8+6*64+7*512;found++;
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
				moveList[found+1]=(pawnColumn-1)+startRow*8+pawnColumn*64+(startRow+turn)*512+262144;
				
				// need to check if the move is indeed legal, if my pawn was pinned, if my opponent's pawn was pinned or if the pawns are on the same row as my king or if we are in check
				if ((pinnedDirections[pawnColumn-1][startRow]!=-1) || (pinnedDirections[pawnColumn][startRow]!=-1) || (startRow==kingRow) || (currentlyInCheck!=0)){
					assignMakeMove(board,moveList[found+1]);
					result=isKingInCheck(board,turn,kingCol,kingRow);
					assignUndoMove(board,moveList[found+1]);

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
				moveList[found+1]=(pawnColumn+1)+startRow*8+pawnColumn*64+(startRow+turn)*512+262144;
				
				// need to check if the move is indeed legal, if my pawn was pinned, if my opponent's pawn was pinned or if the pawns are on the same row as my king
				if ((pinnedDirections[pawnColumn+1][startRow]!=-1) || (pinnedDirections[pawnColumn][startRow]!=-1) || (startRow==kingRow) || (currentlyInCheck!=0)){
					assignMakeMove(board,moveList[found+1]);
					result=isKingInCheck(board,turn,kingCol,kingRow);
					assignUndoMove(board,moveList[found+1]);

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
	moveList[found+1]=isAttacked[kingCol][kingRow]; // add an extra number indicating if we are in check
	
	
	
	/*******************************************
	**** 12. Moveordering: evaluating moves ****
	*******************************************/
	
	
	if (sortMoves==false || found <=1){return;} // turning off moveordering


	vector<int> quickEvals(found, 0);
	vector<int> PSTEvals(found, 0);
	
	
	int startPieceValue,endPieceValue,startCol,endCol,endRow,PST_gain;
	
	
	unsigned int move;
	
	for (int i=1; i<=moveList[0]; i++){
		move=moveList[i];
		startCol=move&0b111;
		startRow=(move>>3)&0b111;
		endCol=(move>>6)&0b111;
		endRow=(move>>9)&0b111;
		
		startpiece=board[startCol][startRow];
		endpiece=board[endCol][endRow];
		
		startPieceValue=abs(startpiece);
		if (startPieceValue>=5){startPieceValue=10;}
		if (startPieceValue==2){startPieceValue=5;}
		if (startPieceValue==4){startPieceValue=3;}

		endPieceValue=abs(endpiece);
		if (endPieceValue>=5){endPieceValue=10;} // however, endpiece==King should not be possible
		if (endPieceValue==2){endPieceValue=5;}
		if (endPieceValue==4){endPieceValue=3;}
		
		// pure PST difference (this is only an estimated, since we don't use the current gamePhase)
		PST_gain=PST_average[6+startpiece][endCol][endRow]*turn-PST_average[6+startpiece][startCol][startRow]*turn;
		
		
		// I want: 
		// + 100* score of captured piece
		// - 100* score of moveing Piece if the target square is attacked
		// + 300* [moving piece is a queen or rook and it was attacked]
		// + 300* [moving piece is a bishop or knight and it was attacked by a pawn]
		// + difference of the average PST from moving that piece
		// + 10000 if move=thisMoveFirst
		
		// before calculating these quickEvals, one could count how often a square is attacked ...
		// One could also quickly check for checks
		


		quickEvals[i-1]=100*endPieceValue                                                 // bonus for capturing a piece
						-100*startPieceValue*(isAttacked[endCol][endRow])                 // bonus if the square we are moving to is not attacked
						+300*(startPieceValue>=5)*isAttacked[startCol][startRow]          // bonus if we move a queen or a rook and the startsquare was attacked
						+300*(startPieceValue==3)*isPawnAttacked[startCol][startRow]      // bonus if we move a bishop or knight and the startsquare was attacked by a pawn
						+PST_gain;
						
						
		PSTEvals[i-1]=PST_gain;
		
		
		//printMove(moveList[i]);
		//cout << ":  "<<quickEvals[i]<<"\n";
	}
	
	if (thisMoveFirst!=0){
		for (int i=1; i<=moveList[0]; i++){
			quickEvals[i-1]+=10000*(moveList[i]%65536==thisMoveFirst);
		}
	}
	
	
	/*******************************************
	**** 13. Moveordering: reordering moves ****
	*******************************************/
	
	vector<pair<int, int>> value_index_pairs(found);
    for (int i=0; i<found; i++) {
        value_index_pairs[i] = {-quickEvals[i], i};
    }

    sort(value_index_pairs.begin(), value_index_pairs.end());

	unsigned int moveListCopy[250];
	for (int i=1;i<=found;i++){
		moveListCopy[i]=moveList[i];
	}
	
	int filled=1;
    for (const auto& pair : value_index_pairs) {
        //cout << pair.second << " ";
		moveList[filled]=moveListCopy[pair.second+1];filled++;
    }
	
	moveList[moveList[0]+1]=moveList[found+1];// fixing the check bit
	
	
	return;

}

#endif