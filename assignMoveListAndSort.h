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


void assignMoveListAndSort (int board[8][8],unsigned long long int key, unsigned int moveList[], bool sortMoves, unsigned short int thisMoveFirst=0){
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
	int piece,dx,dy,targetRow,targetCol,startpiece,endpiece,dir,piecesFoundInThatDirection,currentlyInCheck,kingSquare;
	bool plsDelete,markPinnedPiece;
	int square,startSquare,endSquare;
	int BOARD[64];
	
	
	/**********************
	**** 0. Initialize ****
	**********************/
	
	const int dx_LIST[16]={ 0, 0, 1,-1,-1, 1, 1,-1,-2,-2,-1,-1,+1,+1,+2,+2};
	const int dy_LIST[16]={ 1,-1, 0, 0, 1,-1, 1,-1,-1,+1,-2,+2,-2,+2,-1,+1};

	for (int col=0;col<8;col++){
		for (int row=0; row<8; row++){
			BOARD[col+8*row]=board[col][row];
		}
	}
	
	
	bool isAttacked[64];
	bool isPawnAttacked[64];
	bool defendTable[64];
	int pinnedDirections[64];
	
	for (int i=0;i<64;i++){
		isAttacked[i]=false;
		isPawnAttacked[i]=false;
		defendTable[i]=false;
		pinnedDirections[i]=-1;
	}
	
	
	/****************************
	**** 1. finding own King ****
	****************************/
	
	int kingCol,kingRow;
	
	for (int col=0;col<8;col++){
		for (int row=0; row<8; row++){
			if (BOARD[col+8*row]==6*turn){
				kingCol=col;
				kingRow=row;
				kingSquare=col+8*row;
			}
		}
	}
	
	
	
	/************************************
	**** 2. finding attacked squares ****
	************************************/
	
	
	BOARD[kingSquare]=0; // remove own king for the moment
	int absPiece;
	
	for (int row=0; row<8; row++){
		for (int col=0; col<8; col++){
			piece=BOARD[col+8*row];
			if (piece*turn>=0){continue;} // searching for opponents pieces(!)
			
			absPiece=abs(piece);
			
			switch (absPiece){
				case 1:
					if (col>=1){
						square=(col-1)+8*(row-turn);
						isAttacked         [square]=true;
						isPawnAttacked     [square]=true;
					}
					if (col<=6){
						square=(col+1)+8*(row-turn);
						isAttacked         [square]=true;	
						isPawnAttacked     [square]=true;
					}
					break;
					
				case 2: 
					for (int i=0; i<4; i++){
						dx=dx_LIST[i];
						dy=dy_LIST[i];
						
						for (int j=1; j<8; j++){
							targetCol=col+dx*j;targetRow=row+dy*j;endpiece=BOARD[targetCol+targetRow*8]; 
							if (targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7){ // target square on BOARD
								square=targetCol+targetRow*8;
								isAttacked[square]=true;
								if (endpiece!=0){break;}
							} else {
								break;
							}
						}
					}
					break;
					
				case 3:
					if (col>1 && row>0){isAttacked[(col-2)+8*(row-1)]=true;}
					if (col>1 && row<7){isAttacked[(col-2)+8*(row+1)]=true;}
					if (col>0 && row>1){isAttacked[(col-1)+8*(row-2)]=true;}
					if (col>0 && row<6){isAttacked[(col-1)+8*(row+2)]=true;}
					if (col<7 && row>1){isAttacked[(col+1)+8*(row-2)]=true;}
					if (col<7 && row<6){isAttacked[(col+1)+8*(row+2)]=true;}
					if (col<6 && row>0){isAttacked[(col+2)+8*(row-1)]=true;}
					if (col<6 && row<7){isAttacked[(col+2)+8*(row+1)]=true;}
					break;
					
					
				case 4:				
					for (int i=4; i<8; i++){
						dx=dx_LIST[i];
						dy=dy_LIST[i];
						
						for (int j=1; j<8; j++){
							targetCol=col+dx*j;targetRow=row+dy*j;endpiece=BOARD[targetCol+targetRow*8]; 
							if (targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7){ // target square on BOARD
								square=targetCol+targetRow*8;
								isAttacked[square]=true;
								if (endpiece!=0){break;}
							} else {
								break;
							}
						}
					}
					break;
					
				
				case 5:				
					for (int i=0; i<8; i++){
						dx=dx_LIST[i];
						dy=dy_LIST[i];
						
						for (int j=1; j<8; j++){
							targetCol=col+dx*j;targetRow=row+dy*j;endpiece=BOARD[targetCol+targetRow*8]; 
							if (targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7){ // target square on BOARD
								square=targetCol+targetRow*8;
								isAttacked[square]=true;
								if (endpiece!=0){break;}
							} else {
								break;
							}
						}
					}
					break;
					
					
					
				case 6:
					if (col>0 && row>0){isAttacked[(col-1)+8*(row-1)]=true;}
					if (col>0         ){isAttacked[(col-1)+8*(row  )]=true;}
					if (col>0 && row<7){isAttacked[(col-1)+8*(row+1)]=true;}
					if (         row>0){isAttacked[(col  )+8*(row-1)]=true;}
					if (         row<7){isAttacked[(col  )+8*(row+1)]=true;}
					if (col<7 && row>0){isAttacked[(col+1)+8*(row-1)]=true;}
					if (col<7         ){isAttacked[(col+1)+8*(row  )]=true;}
					if (col<7 && row<7){isAttacked[(col+1)+8*(row+1)]=true;}
					break;
					
			}
		}	
	}
	
	
	
	BOARD[kingSquare]=6*turn; // moving the king back on the BOARD
	
	
	
	/****************************
	**** 3. are we in check? ****
	****************************/
	
	if (isAttacked[kingCol+kingRow*8]){
		currentlyInCheck=isKingInCheck(board,turn,kingCol,kingRow);
	} else {
		currentlyInCheck=0;
	}
	

	/*********************************
	**** 4. finding pinned pieces ****
	*********************************/
	
	// note: also marks the direction; also marks pieces of the opponent that are pinned to my king (important for en passant validation)
	//direction 0=N
	//direction 1=E
	//direction 2=NW
	//direction 3=NE
	
	const int pinHelper[4]={2,2,4,4};
	
	for (int i=0; i<8; i++){
		dir=i/2;
		dx=dx_LIST[i];
		dy=dy_LIST[i];
		
		markPinnedPiece=false;
		piecesFoundInThatDirection=0;
		for (int j=1; j<8; j++){
			targetCol=kingCol+dx*j;
			targetRow=kingRow+dy*j;
			if (targetCol<0 || targetCol >7 || targetRow<0 || targetRow >7){break;} // target square not on BOARD
			
			
			
			if (BOARD[targetCol+targetRow*8]!=0){
				piecesFoundInThatDirection++;
				if (piecesFoundInThatDirection==2){
					if (BOARD[targetCol+targetRow*8]==-turn*pinHelper[dir] || BOARD[targetCol+targetRow*8]==-turn*5){
						markPinnedPiece=true;
					}
					break;
				}
			}
		}
		
		if (markPinnedPiece==true){
			for (int j=1; j<8; j++){
				targetCol=kingCol+dx*j;
				targetRow=kingRow+dy*j;
				if (BOARD[targetCol+targetRow*8]!=0){
					pinnedDirections[targetCol+targetRow*8]=dir;
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
			startSquare=col+row*8;
			piece=BOARD[startSquare];
			
			if (piece*turn<=0){continue;} // there is indeed a piece of the right color here
			
			absPiece=abs(piece);
			
			switch (absPiece){
				case 1:
					if (BOARD[col+(row+turn)*8]==0 && (pinnedDirections[startSquare]==-1 || pinnedDirections[startSquare]==0)){ //1 move push is possible and not pinned
					
						if (row+turn==0 || row+turn==7){// this move is a promotion
							moveList[++found]=startSquare+col*64+(row+turn)*512+5*4096;
							moveList[++found]=startSquare+col*64+(row+turn)*512+3*4096;
							moveList[++found]=startSquare+col*64+(row+turn)*512+2*4096;
							moveList[++found]=startSquare+col*64+(row+turn)*512+4*4096;
						} else {
							moveList[++found]=startSquare+col*64+(row+turn)*512;
						}
						
						
						// check if pawn on start square + if 2 move push possible
						if (row==1 && turn==1 && BOARD[startSquare+16*turn]==0){
							moveList[++found]=startSquare+col*64+(row+2*turn)*512;
						}
						if (row==6 && turn==-1 && BOARD[startSquare+16*turn]==0){
							moveList[++found]=startSquare+col*64+(row+2*turn)*512;
						}
						
					}

					//check if left capture is possible
					if (col>=1){
						endSquare=(col-1)+(row+turn)*8;
						defendTable[endSquare]=true;
						endpiece=BOARD[endSquare];
						dir=3-(turn+1)/2;
						if (endpiece*piece<0 && (pinnedDirections[startSquare]==-1 || pinnedDirections[startSquare]==dir)){ // there is something to capture and not pinned or pinned in the right direction
							if (row+turn==0 || row+turn==7){ // this is additionally a promotion
								moveList[++found]=startSquare+64*endSquare+5*4096+32768*abs(endpiece);
								moveList[++found]=startSquare+64*endSquare+3*4096+32768*abs(endpiece);
								moveList[++found]=startSquare+64*endSquare+2*4096+32768*abs(endpiece);
								moveList[++found]=startSquare+64*endSquare+4*4096+32768*abs(endpiece);
							} else {
								moveList[++found]=startSquare+64*endSquare+32768*abs(endpiece);
							}
						}
					}

					//check if right capture is possible
					if (col<=6){
						endSquare=(col+1)+(row+turn)*8;
						defendTable[endSquare]=true;
						endpiece=BOARD[endSquare];
						dir=2+(turn+1)/2;
						if (endpiece*piece<0 && (pinnedDirections[startSquare]==-1 || pinnedDirections[startSquare]==dir)){ // there is something to capture and not pinned or pinned in the right direction
							if (row+turn==0 || row+turn==7){ // this is additionally a promotion
								moveList[++found]=startSquare+64*endSquare+5*4096+32768*abs(endpiece);
								moveList[++found]=startSquare+64*endSquare+3*4096+32768*abs(endpiece);
								moveList[++found]=startSquare+64*endSquare+2*4096+32768*abs(endpiece);
								moveList[++found]=startSquare+64*endSquare+4*4096+32768*abs(endpiece);
							} else {
								moveList[++found]=startSquare+64*endSquare+32768*abs(endpiece);
							}
						}
					}
					break;
				
				case 2:
					for (int i=0; i<4; i++){
						if (pinnedDirections[startSquare]!=-1 && pinnedDirections[startSquare]!=i/2){continue;}
						dx=dx_LIST[i];
						dy=dy_LIST[i];
						
						for (int j=1; j<8; j++){
							targetCol=col+dx*j;
							targetRow=row+dy*j;
							endSquare=targetCol+targetRow*8;
							endpiece=BOARD[endSquare]; 
							if (targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7){ // target square on BOARD
								defendTable[targetCol+targetRow*8]=true;
								if (endpiece==0){moveList[++found]=startSquare+64*endSquare;}
								if (piece*endpiece<0){moveList[++found]=startSquare+64*endSquare+abs(endpiece)*32768;break;}
								if (piece*endpiece>0){break;}
							} else {
								break;
							}
						}
					}
					break;
					
					
				case 3:
					if (pinnedDirections[startSquare]==-1){
						if (row>=2 && col>=1 ){ square=startSquare-1-16;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row>=2 && col<=6 ){ square=startSquare+1-16;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row>=1 && col>=2 ){ square=startSquare-2-8 ;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row>=1 && col<=5 ){ square=startSquare+2-8 ;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row<=6 && col>=2 ){ square=startSquare-2+8 ;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row<=6 && col<=5 ){ square=startSquare+2+8 ;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row<=5 && col>=1 ){ square=startSquare-1+16;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
						if (row<=5 && col<=6 ){ square=startSquare+1+16;defendTable[square]=true; endpiece=BOARD[square]; if (piece*endpiece<=0){ moveList[++found]=startSquare+64*square+abs(endpiece)*32768;}}
					}
					break;
			
			
				case 4:
					for (int i=4; i<8; i++){
						if (pinnedDirections[startSquare]!=-1 && pinnedDirections[startSquare]!=i/2){continue;}
						dx=dx_LIST[i];
						dy=dy_LIST[i];
						
						for (int j=1; j<8; j++){
							targetCol=col+dx*j;
							targetRow=row+dy*j;
							endSquare=targetCol+targetRow*8;
							endpiece=BOARD[endSquare]; 
							if (targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7){ // target square on BOARD
								defendTable[endSquare]=true;
								if (endpiece==0){moveList[++found]=startSquare+64*endSquare;}
								if (piece*endpiece<0){moveList[++found]=startSquare+64*endSquare+abs(endpiece)*32768;break;}
								if (piece*endpiece>0){break;}
							} else {
								break;
							}
						}
					}
					break;
				
				
				
				case 5:
					for (int i=0; i<8; i++){
						if (pinnedDirections[startSquare]!=-1 && pinnedDirections[startSquare]!=i/2){continue;}
						dx=dx_LIST[i];
						dy=dy_LIST[i];
						
						for (int j=1; j<8; j++){
							targetCol=col+dx*j;
							targetRow=row+dy*j;
							endSquare=targetCol+targetRow*8;
							endpiece=BOARD[endSquare]; 
							if (targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7){ // target square on BOARD
								defendTable[targetCol+targetRow*8]=true;
								if (endpiece==0){moveList[++found]=startSquare+64*endSquare;}
								if (piece*endpiece<0){moveList[++found]=startSquare+64*endSquare+abs(endpiece)*32768;break;}
								if (piece*endpiece>0){break;}
							} else {
								break;
							}
						}
					}
					break;
			}
		}	
	}
	
	
	
	/*********************************************
	**** 6. flaged illegal moves: if in check ****
	*********************************************/

	int deleted=0;
	
	
	if (currentlyInCheck!=0){
		// will calculate blocking squares:
		bool blockingSquares[64];
		for (int i=0;i<64;i++){blockingSquares[i]=false;}
		
		int attackingCol=(currentlyInCheck-1)%8;
		int attackingRow=(currentlyInCheck-1-attackingCol)/8;
		
		
		if (abs(attackingCol-kingCol)*abs(attackingRow-kingRow)==2){ // knight move
			blockingSquares[attackingCol+attackingRow*8]=true;
		} else {
			dx=0;
			if (attackingCol-kingCol>0){dx=1;}
			if (attackingCol-kingCol<0){dx=-1;}
			dy=0;
			if (attackingRow-kingRow>0){dy=1;}
			if (attackingRow-kingRow<0){dy=-1;}

			for (int j=1; j<8; j++){
				targetCol=kingCol+dx*j;targetRow=kingRow+dy*j;
				blockingSquares[targetCol+targetRow*8]=true;
				if (BOARD[targetCol+targetRow*8]!=0){break;}
			}
		}	
		
		for (int i=1;i<=found;i++){
			// will flag for deletion automatically, unless a piece lands on a blocking square (note: all king moves and en passant moves are not generated yet)
			
			plsDelete=true;
			if (blockingSquares[(moveList[i]>>6)&0b111111]==true){
				assignMakeMove(board,moveList[i]);
				if (isKingInCheck(board,turn,kingCol,kingRow)==0){
					plsDelete=false;
				}
				assignUndoMove(board,moveList[i]);
			}
			
			if (plsDelete){	
				deleted++;
				moveList[i]=0;
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
			if (moveList[i]!=0){
				moveList[move2position]=moveList[i];
				move2position++;
			}
		}
		found=found-deleted;
	}
	
	
	/*****************************
	**** 9. adding king moves ****
	*****************************/
	
	
	if (kingCol>=1 && kingRow>=1 ){ endSquare=kingSquare-9; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (              kingRow>=1 ){ endSquare=kingSquare-8; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (kingCol<=6 && kingRow>=1 ){ endSquare=kingSquare-7; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (kingCol>=1               ){ endSquare=kingSquare-1; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (kingCol<=6               ){ endSquare=kingSquare+1; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (kingCol>=1 && kingRow<=6 ){ endSquare=kingSquare+7; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (              kingRow<=6 ){ endSquare=kingSquare+8; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	if (kingCol<=6 && kingRow<=6 ){ endSquare=kingSquare+9; defendTable[endSquare]=true; endpiece=BOARD[endSquare]; if (turn*endpiece<=0 && !isAttacked[endSquare]){ moveList[++found]=kingSquare+64*endSquare+32768*abs(endpiece);}}
	
	/*********************************
	**** 10. adding castles moves ****
	*********************************/
	
	
	if (turn==1 && currentlyInCheck==0){
		if ((key&4) && BOARD[0+0*8]==2 && BOARD[1+0*8]==0 && BOARD[2+0*8]==0 && BOARD[3+0*8]==0 && BOARD[4+0*8]==6 && !isAttacked[2+0*8] && !isAttacked[3+0*8]){ //white queen side
			moveList[++found]=4+0*8+2*64+0*512;
		}
		if ((key&2) && BOARD[4+0*8]==6 && BOARD[5+0*8]==0 && BOARD[6+0*8]==0 && BOARD[7+0*8]==2 && !isAttacked[5+0*8] && !isAttacked[6+0*8]){ //white king side
			moveList[++found]=4+0*8+6*64+0*512;
		}
	}
	if (turn==-1 && currentlyInCheck==0){
		if ((key&16) && BOARD[0+7*8]==-2 && BOARD[1+7*8]==0 && BOARD[2+7*8]==0 && BOARD[3+7*8]==0 && BOARD[4+7*8]==-6 && !isAttacked[2+7*8] && !isAttacked[3+7*8]){ //black queen side
			moveList[++found]=4+7*8+2*64+7*512;
		}
		if ((key&8) && BOARD[4+7*8]==-6 && BOARD[5+7*8]==0 && BOARD[6+7*8]==0 && BOARD[7+7*8]==-2 && !isAttacked[5+7*8] && !isAttacked[6+7*8]){ //black king side
			moveList[++found]=4+7*8+6*64+7*512;
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
			if (BOARD[(pawnColumn-1)+startRow*8]==turn){ // there is indeed a pawn there -> en passant is possible
				moveList[found+1]=(pawnColumn-1)+startRow*8+pawnColumn*64+(startRow+turn)*512+262144;
				
				// need to check if the move is indeed legal, if my pawn was pinned, if my opponent's pawn was pinned or if the pawns are on the same row as my king or if we are in check
				if ((pinnedDirections[(pawnColumn-1)+startRow*8]!=-1) || (pinnedDirections[pawnColumn+startRow*8]!=-1) || (startRow==kingRow) || (currentlyInCheck!=0)){
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
			if (BOARD[(pawnColumn+1)+startRow*8]==turn){ // there is indeed a pawn there -> en passant is possible
				moveList[found+1]=(pawnColumn+1)+startRow*8+pawnColumn*64+(startRow+turn)*512+262144;
				
				// need to check if the move is indeed legal, if my pawn was pinned, if my opponent's pawn was pinned or if the pawns are on the same row as my king
				if ((pinnedDirections[(pawnColumn+1)+startRow*8]!=-1) || (pinnedDirections[pawnColumn+startRow*8]!=-1) || (startRow==kingRow) || (currentlyInCheck!=0)){
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
	moveList[found+1]=isAttacked[kingCol+kingRow*8]; // add an extra number indicating if we are in check
	
	
	
	/*******************************************
	**** 12. Moveordering: evaluating moves ****
	*******************************************/
	
	
	if (sortMoves==false || found <=1){return;} // turning off moveordering
	

	int startPieceValue,endPieceValue,PST_gain,quickEval;
	unsigned int move;
	int pieceValues[13]={10,10,3,3,5,1,0,1,5,3,3,10,10};
	
	
	
	/**************************************
	**** 13. Defending Attacked Pieces ****
	**************************************/
	
	int nrOpponentThreats=0;
	int plsDefend=0;
	for (int s=0;s<64;s++){
		if (BOARD[s]*turn>0 && abs(BOARD[s])<5 && isAttacked[s] && !defendTable[s]){
			nrOpponentThreats++;
			plsDefend=s;
		}
	}
	
	int theseWouldHelp[64];
	for (int i=0;i<64;i++){theseWouldHelp[i]=0;}
	
	if (nrOpponentThreats==1){
		int row=plsDefend/8;
		int col=plsDefend%8;
		for (int i=0; i<8; i++){
			dx=dx_LIST[i];
			dy=dy_LIST[i];
			
			for (int j=1; j<8; j++){
				targetCol=col+dx*j;targetRow=row+dy*j;endpiece=BOARD[targetCol+targetRow*8]; 
				if (!(targetCol>=0 && targetCol <=7 && targetRow>=0 && targetRow <=7)){break;}
				if (endpiece!=0){break;}
				theseWouldHelp[targetCol+8*targetRow]=2+(i/4)*2;
			}
		}
		
		if (col>0 && row-turn>=0 && row+turn<=7){theseWouldHelp[plsDefend-8*turn-1]=1;}
		if (col<7 && row-turn>=0 && row+turn<=7){theseWouldHelp[plsDefend-8*turn+1]=1;}
		
		if (row>=2 && col>=1 ){ theseWouldHelp[plsDefend-1-16]=3;}
		if (row>=2 && col<=6 ){ theseWouldHelp[plsDefend+1-16]=3;}
		if (row>=1 && col>=2 ){ theseWouldHelp[plsDefend-2-8 ]=3;}
		if (row>=1 && col<=5 ){ theseWouldHelp[plsDefend+2-8 ]=3;}
		if (row<=6 && col>=2 ){ theseWouldHelp[plsDefend-2+8 ]=3;}
		if (row<=6 && col<=5 ){ theseWouldHelp[plsDefend+2+8 ]=3;}
		if (row<=5 && col>=1 ){ theseWouldHelp[plsDefend-1+16]=3;}
		if (row<=5 && col<=6 ){ theseWouldHelp[plsDefend+1+16]=3;}
	}
	
	
	
	/**************************
	**** 14. The move Loop ****
	**************************/
	
	int phase24=int((key>>23)&63);
	int END,START;
	int PST_gain_mg,PST_gain_eg,scores[250];
	int bestQuietMoveEval=-10000000;
	int bestQuietPSTgain=-10000;
	
	for (int i=1; i<=moveList[0]; i++){
		move=moveList[i];
		//startCol=move&0b111;
		//startRow=(move>>3)&0b111;
		//endCol=(move>>6)&0b111;
		//endRow=(move>>9)&0b111;
		
		START=move&0b111111;//startCol+startRow*8;
		END=(move>>6)&0b111111;//endCol+endRow*8;
		
		
		startpiece=BOARD[START];
		endpiece=BOARD[END];
		
		startPieceValue=pieceValues[startpiece+6];
		endPieceValue=pieceValues[endpiece+6];
		
		PST_gain_mg=(PST_mg_vector[6+startpiece+13*END]-PST_mg_vector[6+startpiece+13*START])*turn;
		PST_gain_eg=(PST_eg_vector[6+startpiece+13*END]-PST_eg_vector[6+startpiece+13*START])*turn;
		PST_gain=(PST_gain_mg*phase24+PST_gain_eg*(24-phase24))/24;
		

		
		// start computing quickEval
		quickEval=100*endPieceValue                                           // bonus for capturing a piece
			+PST_gain
			+10000*(move%65536==thisMoveFirst)                                // hashmove
			+(move>4095)*20;                                                  // captures get an additional bonus
		
		
		
		bool iaStart,iaEnd,ipAS;
		
		iaStart=isAttacked[START];
		iaEnd  =isAttacked[END];
		ipAS   =isPawnAttacked[START];
		
		
		switch (startPieceValue){
			case 1:
				quickEval+= 100*(iaStart && !defendTable[START] && defendTable[END]);
				quickEval-= 100*(isPawnAttacked[END] || (iaEnd && !defendTable[END]));
				break;
				
			case 3:
				quickEval+=
				-200 * iaEnd               // penalty for moving to an attacked square.
				-100 * isPawnAttacked[END] // additional penalty, if attacked by a pawn
				+300*(ipAS || (iaStart && !defendTable[START]));
				break;
				
			case 5:
				quickEval+=300*(iaStart)- 500*iaEnd;
				break;
				
			case 10:
				quickEval+=300*(iaStart)- 1000*iaEnd;
				break;
		}
		
		if (move<4096 && quickEval>bestQuietMoveEval){
			bestQuietMoveEval=quickEval;
			bestQuietPSTgain=PST_gain;
		}
		
		
		if (abs(quickEval)<50 && move<4096 && nrOpponentThreats==1){// defending an attacked piece
			if (theseWouldHelp[END]==abs(startpiece)){
				quickEval+=40;
			} else if ((theseWouldHelp[END]==1 || theseWouldHelp[END]==2 || theseWouldHelp[END]==4) && abs(startpiece)==5){ 
				quickEval+=40;
			} else if (theseWouldHelp[END]==1 && abs(startpiece)==4){ 
				quickEval+=40;
			}
		}
		
		if (move<4096){quickEval+=moveHistoryScore[move+4096*(turn==1)];}
		
		
		scores[i]=quickEval;
	}
	
	
	
	/*************************
	**** 15. Moveordering ****
	*************************/
	
	// insertion sort is faster than C++ sort for small arrays.
	int score;
	for (int i=2; i<=found; i++){
		score=scores[i];
		move=moveList[i];
		int j=i-1;

		while (j>=1 && scores[j]<score){
			scores[j+1]=scores[j];
			moveList[j+1]=moveList[j];
			j--;
		}
		scores[j+1]=score;
		moveList[j+1]=move;
	}
	
	moveList[found+2]=bestQuietPSTgain+10000; 
	
	return;

}

#endif