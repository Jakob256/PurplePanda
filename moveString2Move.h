/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef moveString2MoveFILE
#define moveString2MoveFILE

/***************
*** Let's go ***
***************/


unsigned int moveString2Move (int board[8][8],string moveString){
	int c1=moveString[0]-'a'; // C magic
	int r1=moveString[1]-'1';
	int c2=moveString[2]-'a';
	int r2=moveString[3]-'1';
	
	int startPiece=board[c1][r1];
	int endPiece=board[c2][r2];	
	
	unsigned int move=c1+8*r1+64*c2+512*r2+32768*abs(endPiece);
	
	if (abs(startPiece)==1 && (r1+startPiece)%7==0){ // pawn moved to row 0 or row 7, aka promotion
		if (tolower(moveString[4])=='r'){move+=4096*2;}
		if (tolower(moveString[4])=='n'){move+=4096*3;}
		if (tolower(moveString[4])=='b'){move+=4096*4;}
		if (tolower(moveString[4])=='q'){move+=4096*5;}
	}

	if (abs(startPiece)==1 && c1!=c2 && endPiece==0){ //en passant capture
		move+=262144;
	}
	
	return move;
}

#endif