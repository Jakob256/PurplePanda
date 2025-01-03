/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef newHashFILE
#define newHashFILE

/***************
*** Let's go ***
***************/


// has to applied to the board before making the move on the board

long long int newHash(int board[8][8],unsigned long long int key, long long int hash, unsigned int move){
	short int c1=move&0b111;
	short int r1=(move>>3)&0b111;
	short int c2=(move>>6)&0b111;
	short int r2=(move>>9)&0b111;
	short int absPromotedTo=(move>>12)&0b111;
	//short int absCaptured=(move>>15)&0b111;
	bool enPassant =(move>>18)&0b1;
	
	long long int newHashh=hash;
	int turn=key%2;
	
	
	/***********************
	**** calculate Turn ****
	***********************/
	
	newHashh += (1-2*turn)*hashTurnNumber;
	if (move==0){return newHashh;} //nullmove
	
	/*******************************************
	**** calculate Piece Square Table Bonus ****
	*******************************************/
	
	long long int a=board[c1][r1]*hashSquareNumbers[c1][r1]; //movedPiece*startSquare
	long long int b=board[c1][r1]*hashSquareNumbers[c2][r2]; //movedPiece*endSquare
	long long int c=board[c2][r2]*hashSquareNumbers[c2][r2]; //capturedPiece*endSquare
	newHashh -= a;
	newHashh += b;
	newHashh -= c;
	
	/****************************
	**** calculate casteling ****
	****************************/
	
	if (abs(board[c1][r1])==6){ // we have a king move -> check if it was casteling:    // the most beautiful bug ever was: ... board[c1][r2] ... ! <3 
		if (abs(c1-c2)==2){// it was casteling
			if (c2==2 && r1==0){// white queen side casteling:
				newHashh -= 2*hashSquareNumbers[0][0];
				newHashh += 2*hashSquareNumbers[3][0];
			}
			if (c2==6&& r1==0){// white king side casteling:
				newHashh -= 2*hashSquareNumbers[7][0];
				newHashh += 2*hashSquareNumbers[5][0];
			}
			if (c2==2 && r1==7){// black queen side casteling:
				newHashh -= -2*hashSquareNumbers[0][7];
				newHashh += -2*hashSquareNumbers[3][7];
			}
			if (c2==6&& r1==7){// black king side casteling:
				newHashh -= -2*hashSquareNumbers[7][7];
				newHashh += -2*hashSquareNumbers[5][7];
			}
			
		}
	}
	
	/*****************************
	**** calculate en pessant ****
	*****************************/
	
	
	if (enPassant){
		newHashh -= board[c2][r1]*hashSquareNumbers[c2][r1];
	}
	
	/****************************
	**** calculate promotion ****
	****************************/
	
	if (absPromotedTo!=0){ // move is a promotion
		newHashh -= board[c1][r1]*hashSquareNumbers[c2][r2]; // remove the pawn from promotion square
		newHashh += board[c1][r1]*absPromotedTo*hashSquareNumbers[c2][r2]; // add with the right color the promoted piece
	}
	
	return newHashh;
}

#endif