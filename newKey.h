/**************************
*** function depends on ***
**************************/

#include "isKingInCheck.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"

/************
*** Guard ***
************/

#ifndef newKeyFILE
#define newKeyFILE

/***************
*** Let's go ***
***************/


// has to applied to the board before making the move on the board

unsigned long long int newKey (int board[8][8],unsigned long long int key, int c1, int r1, int c2, int r2, int adInfo){
	
	unsigned long long int newKeyy;
	
	/***********************
	**** calculate turn ****
	***********************/
	
	newKeyy=key^1; 
	
	
	/***********************************
	**** calculate casteling rights ****
	***********************************/
	
	if ((key>>1)&15){   // this might be a source of error...
		if (c1==4&&r1==0){newKeyy=newKeyy&(newKeyy^(2+4));}
		if (c2==4&&r2==0){newKeyy=newKeyy&(newKeyy^(2+4));}
		if (c1==0&&r1==0){newKeyy=newKeyy&(newKeyy^(4));}
		if (c2==0&&r2==0){newKeyy=newKeyy&(newKeyy^(4));}
		if (c1==7&&r1==0){newKeyy=newKeyy&(newKeyy^(2));}
		if (c2==7&&r2==0){newKeyy=newKeyy&(newKeyy^(2));}
		
		if (c1==4&&r1==7){newKeyy=newKeyy&(newKeyy^(8+16));}
		if (c2==4&&r2==7){newKeyy=newKeyy&(newKeyy^(8+16));}
		if (c1==0&&r1==7){newKeyy=newKeyy&(newKeyy^(16));}
		if (c2==0&&r2==7){newKeyy=newKeyy&(newKeyy^(16));}
		if (c1==7&&r1==7){newKeyy=newKeyy&(newKeyy^(8));}
		if (c2==7&&r2==7){newKeyy=newKeyy&(newKeyy^(8));}
	}
	
	/*****************************
	**** calculate en pessant ****
	*****************************/
	
	
	if (abs(board[c1][r1])+abs(board[c2][r2])==1 && abs(r1-r2)==2){ // last move was a pawn double push 
		newKeyy=newKeyy|32; // set the flag bit
		newKeyy=newKeyy&(newKeyy^(64+128+256)); // set the column bits to 0
		newKeyy=newKeyy|(c1<<6); // set the column bits to the right column
	} else { // need to delete potential en pessant marked columns
		newKeyy=newKeyy&(newKeyy^(32)); // set the flag bit
	}
		
	/***************************
	**** calculate nrPieces ****
	***************************/
	
	if (board[c2][r2]!=0){ // a piece was on the endsquare
		newKeyy-=512; //directly effect the corresponding bits
	} else if (adInfo==128){ // this is an en pessant capture
		newKeyy-=512;
	}
	
	/*****************************
	**** calculate evaluation ****
	*****************************/

	if (board[c2][r2]!=0){ // a piece was on the endsquare
		if (abs(board[c2][r2])==1){newKeyy-=32768*board[c2][r2]/1*1;}
		if (abs(board[c2][r2])==2){newKeyy-=32768*board[c2][r2]/2*5;}
		if (abs(board[c2][r2])==3){newKeyy-=32768*board[c2][r2]/3*3;}
		if (abs(board[c2][r2])==4){newKeyy-=32768*board[c2][r2]/4*3;}
		if (abs(board[c2][r2])==5){newKeyy-=32768*board[c2][r2]/5*9;}
		if (abs(board[c2][r2])==6){newKeyy-=32768*board[c2][r2]/6*20;} // this is close to the maximal value, to ensure fitting in a signed 256 number
	}
	
	if (adInfo==128){ // this is an en pessant capture
		newKeyy+=32768*board[c1][r1];
	}
	
	// check promoted piece
	if (adInfo>7 && adInfo!=128){
		if (adInfo/16==2){newKeyy+=32768*board[c1][r1]*(5-1);}
		if (adInfo/16==3){newKeyy+=32768*board[c1][r1]*(3-1);}
		if (adInfo/16==4){newKeyy+=32768*board[c1][r1]*(3-1);}
		if (adInfo/16==5){newKeyy+=32768*board[c1][r1]*(9-1);}
	}
	
	
	/*******************************************
	**** calculate Piece Square Table Bonus ****
	*******************************************/
	
	
	unsigned long long int hugeNumber=8388608;  //=2^23
	long long int a=pieceSquareTable[6+board[c1][r1]][c1][r1]*hugeNumber;
	long long int b=pieceSquareTable[6+board[c2][r2]][c2][r2]*hugeNumber;
	long long int c=pieceSquareTable[6+board[c1][r1]][c2][r2]*hugeNumber;
	newKeyy -= a;  // that start piece is no longer where it was there
	newKeyy -= b;  // that target square is for a moment empty
	newKeyy += c;  // that target square is now the start piece
	
	/***********************
	**** PST: casteling ****
	***********************/
	
	if (abs(c1-c2)==2 && abs(board[c1][r1])==6){ // casteling: need to update Rook position
		a=pieceSquareTable[6+board[c1][r1]/3][((c2-2)/4)*7][r1]*hugeNumber;  // column 6->column 7; column 2 -> column 0
		b=pieceSquareTable[6+board[c1][r1]/3][c2/2+2      ][r1]*hugeNumber;  // column 6->column 5; column 2 -> column 3
		newKeyy -= a;  // that rook is removed
		newKeyy += b;  // that rook is added
	}
	
	/***********************
	**** PST: enpessant ****
	***********************/
	
	if (adInfo==128){
		a=pieceSquareTable[6-board[c1][r1]][c2][r1]*hugeNumber;
		newKeyy -= a;  // that pawn is removed
	}

	/***********************
	**** PST: promotion ****
	***********************/
	if (adInfo>=16 && adInfo !=128){
		a=pieceSquareTable[6+board[c1][r1]][c2][r2]*hugeNumber;
		b=pieceSquareTable[6+board[c1][r1]*adInfo/16][c2][r2]*hugeNumber;
		newKeyy -= a;
		newKeyy += b;
	}
	
	
	
	
	if (newKeyy > 1125899906842624){
		cout << "onoooo\n";
	}
	
	return newKeyy;
}

#endif