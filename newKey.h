/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef newKeyFILE
#define newKeyFILE

/***************
*** Let's go ***
***************/


// has to applied to the board before making the move on the board


unsigned long long int newKey (int board[8][8],unsigned long long int key, unsigned int move){
	short int c1=move&0b111;
	short int r1=(move>>3)&0b111;
	short int c2=(move>>6)&0b111;
	short int r2=(move>>9)&0b111;
	short int absPromotedTo=(move>>12)&0b111;
	//short int absCaptured=(move>>15)&0b111;
	bool enPassant =(move>>18)&0b1;
	
	unsigned long long int newKeyy;
	
	/***********************
	**** calculate turn ****
	***********************/
	
	newKeyy=key^1; 
	
	if (move==0){ // nullmove
		newKeyy=newKeyy&(newKeyy^(32)); // delete the en passant flag
		return newKeyy;
	}
	
	/***********************************
	**** calculate casteling rights ****
	***********************************/
	
	if ((key>>1)&15){
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
	**** calculate en passant ****
	*****************************/
	
	
	if (abs(board[c1][r1])==1 && abs(r1-r2)==2){ // last move was a pawn double push 
		newKeyy=newKeyy|32; // set the flag bit
		newKeyy=newKeyy&(newKeyy^(64+128+256)); // set the column bits to 0
		newKeyy=newKeyy|(c1<<6); // set the column bits to the right column
	} else { // need to delete potential en passant marked columns
		newKeyy=newKeyy&(newKeyy^(32)); // set the flag bit
	}
		
	/***************************
	**** calculate nrPieces ****
	***************************/
	
	if (board[c2][r2]!=0){ // a piece was on the endsquare
		newKeyy-=512; //directly effect the corresponding bits
	} else if (enPassant){ // this is an en passant capture
		newKeyy-=512;
	}
	
	/*******************************************
	**** calculate evaluation and gamePhase ****
	*******************************************/

	if (board[c2][r2]!=0){ // a piece was on the endsquare
		if (abs(board[c2][r2])==1){newKeyy-=32768*board[c2][r2]/1*1;}
		if (abs(board[c2][r2])==2){newKeyy-=32768*board[c2][r2]/2*5;newKeyy-=2*8388608;}
		if (abs(board[c2][r2])==3){newKeyy-=32768*board[c2][r2]/3*3;newKeyy-=1*8388608;}
		if (abs(board[c2][r2])==4){newKeyy-=32768*board[c2][r2]/4*3;newKeyy-=1*8388608;}
		if (abs(board[c2][r2])==5){newKeyy-=32768*board[c2][r2]/5*9;newKeyy-=4*8388608;}
		if (abs(board[c2][r2])==6){newKeyy-=32768*board[c2][r2]/6*20;} // this is close to the maximal value, to ensure fitting in a signed 256 number
	}
	
	if (enPassant){ // this is an en passant capture
		newKeyy+=32768*board[c1][r1];
	}
	
	// check promoted piece
	if (absPromotedTo!=0){
		if (absPromotedTo==2){newKeyy+=32768*board[c1][r1]*(5-1);newKeyy+=2*8388608;}
		if (absPromotedTo==3){newKeyy+=32768*board[c1][r1]*(3-1);newKeyy+=1*8388608;}
		if (absPromotedTo==4){newKeyy+=32768*board[c1][r1]*(3-1);newKeyy+=1*8388608;}
		if (absPromotedTo==5){newKeyy+=32768*board[c1][r1]*(9-1);newKeyy+=4*8388608;}
	}
	
	
	
	/******************************
	**** calculate PST updates ****
	******************************/
	
	
	unsigned long long int hugeMgNumber=536870912;  //2^29
	unsigned long long int hugeEgNumber=4398046511104;  //2^42
	long long int a_mg=PST_mg[6+board[c1][r1]][c1][r1]*hugeMgNumber;
	long long int b_mg=PST_mg[6+board[c2][r2]][c2][r2]*hugeMgNumber;
	long long int c_mg=PST_mg[6+board[c1][r1]][c2][r2]*hugeMgNumber;
	newKeyy -= a_mg;  // that start piece is no longer where it was there
	newKeyy -= b_mg;  // that target square is for a moment empty
	newKeyy += c_mg;  // that target square is now the start piece
	
	long long int a_eg=PST_eg[6+board[c1][r1]][c1][r1]*hugeEgNumber;
	long long int b_eg=PST_eg[6+board[c2][r2]][c2][r2]*hugeEgNumber;
	long long int c_eg=PST_eg[6+board[c1][r1]][c2][r2]*hugeEgNumber;
	newKeyy -= a_eg;  // that start piece is no longer where it was there
	newKeyy -= b_eg;  // that target square is for a moment empty
	newKeyy += c_eg;  // that target square is now the start piece
	
	/***********************
	**** PST: casteling ****
	***********************/
	
	if (abs(c1-c2)==2 && abs(board[c1][r1])==6){ // casteling: need to update Rook position
		a_mg=PST_mg[6+board[c1][r1]/3][((c2-2)/4)*7][r1]*hugeMgNumber;  // column 6->column 7; column 2 -> column 0
		b_mg=PST_mg[6+board[c1][r1]/3][c2/2+2      ][r1]*hugeMgNumber;  // column 6->column 5; column 2 -> column 3
		newKeyy -= a_mg;  // that rook is removed
		newKeyy += b_mg;  // that rook is added
		
		a_eg=PST_eg[6+board[c1][r1]/3][((c2-2)/4)*7][r1]*hugeEgNumber;  // column 6->column 7; column 2 -> column 0
		b_eg=PST_eg[6+board[c1][r1]/3][c2/2+2      ][r1]*hugeEgNumber;  // column 6->column 5; column 2 -> column 3
		newKeyy -= a_eg;  // that rook is removed
		newKeyy += b_eg;  // that rook is added
	}
	
	/***********************
	**** PST: enpassant ****
	***********************/
	
	if (enPassant){
		a_mg=PST_mg[6-board[c1][r1]][c2][r1]*hugeMgNumber;
		newKeyy -= a_mg;  // that pawn is removed
		
		a_eg=PST_eg[6-board[c1][r1]][c2][r1]*hugeEgNumber;
		newKeyy -= a_eg;  // that pawn is removed
	}

	/***********************
	**** PST: promotion ****
	***********************/
	
	if (absPromotedTo!=0){
		a_mg=PST_mg[6+board[c1][r1]][c2][r2]*hugeMgNumber;
		b_mg=PST_mg[6+board[c1][r1]*absPromotedTo][c2][r2]*hugeMgNumber;
		newKeyy -= a_mg;
		newKeyy += b_mg;
		
		a_eg=PST_eg[6+board[c1][r1]][c2][r2]*hugeEgNumber;
		b_eg=PST_eg[6+board[c1][r1]*absPromotedTo][c2][r2]*hugeEgNumber;
		newKeyy -= a_eg;
		newKeyy += b_eg;
	}
	
	
	return newKeyy;
}

#endif