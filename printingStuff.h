/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef printingStuffFILE
#define printingStuffFILE

/******************
*** Description ***
******************/

// This file includes:
// plotBoard
// printMove
// printMoveList
// printKey
// printFen


/***************
*** Let's go ***
***************/


void plotBoard(int board[8][8]){
	for (int row = 7; row >= 0; row--) {
		cout << "+---+---+---+---+---+---+---+---+\n|";
		for (int column = 0; column < 8; column++) {
			cout << " ";
			
			if (board[column][row]==1){cout <<"P";}
			if (board[column][row]==2){cout <<"R";}
			if (board[column][row]==3){cout <<"N";}
			if (board[column][row]==4){cout <<"B";}
			if (board[column][row]==5){cout <<"Q";}
			if (board[column][row]==6){cout <<"K";}
			
			if (board[column][row]==-1){cout <<"p";}
			if (board[column][row]==-2){cout <<"r";}
			if (board[column][row]==-3){cout <<"n";}
			if (board[column][row]==-4){cout <<"b";}
			if (board[column][row]==-5){cout <<"q";}
			if (board[column][row]==-6){cout <<"k";}
			
			if (board[column][row]==0){cout <<" ";}
			
			cout <<" |";
		}
		cout <<"\n";
	}
	cout << "+---+---+---+---+---+---+---+---+\n\n";
}




void printMove(unsigned int move){
	short int c1=move&0b111;
	short int r1=(move>>3)&0b111;
	short int c2=(move>>6)&0b111;
	short int r2=(move>>9)&0b111;
	short int absPromotedTo=(move>>12)&0b111;
	//short int absCaptured=(move>>15)&0b111;
	//bool enPassant =(move>>18)&0b1;
	
	
	if (c1==0){cout <<"a";}
	if (c1==1){cout <<"b";}
	if (c1==2){cout <<"c";}
	if (c1==3){cout <<"d";}
	if (c1==4){cout <<"e";}
	if (c1==5){cout <<"f";}
	if (c1==6){cout <<"g";}
	if (c1==7){cout <<"h";}
	cout << r1+1;
	if (c2==0){cout <<"a";}
	if (c2==1){cout <<"b";}
	if (c2==2){cout <<"c";}
	if (c2==3){cout <<"d";}
	if (c2==4){cout <<"e";}
	if (c2==5){cout <<"f";}
	if (c2==6){cout <<"g";}
	if (c2==7){cout <<"h";}
	cout << r2+1;
	
	// if promotion then print which promotion
	if (absPromotedTo==2){cout << "r";}
	if (absPromotedTo==3){cout << "n";}
	if (absPromotedTo==4){cout << "b";}
	if (absPromotedTo==5){cout << "q";}
}



void printMoveList (unsigned int moveList[]){
	short int absCaptured;
	for (int i=1; i<=moveList[0];i++){
		cout << i << ": ";
		printMove(moveList[i]);
		absCaptured=(moveList[i]>>15)&0b111;
		if (absCaptured==1){cout << " capturing a pawn";}
		if (absCaptured==2){cout << " capturing a roook";}
		if (absCaptured==3){cout << " capturing a knight";}
		if (absCaptured==4){cout << " capturing a bishop";}
		if (absCaptured==5){cout << " capturing a queen";}
		if (absCaptured==6){cout << " capturing a king";}
		
		cout<< "\n";
	}
}




void printKey(unsigned long long int key){
	
	// 64bit-key bits: EEEEEEEEEEEEE MMMMMMMMMMMMM PHPHPH SSSSSSSS PPPPPP eeeE CCCC T
	// start key bits: 1000000000000 1000000000000 011000 10000000 100000 0000 1111 1

	// T: 1 bit = turn
	// C: 4 bit = casteling rights
	// e: 4 bit = enPassant + in which column
	// P: 6 bit = nrPiecesOnBoard 
	// S: 8 bit = naiveScore +128
	// PH: 6 bit = game Phase
	// M: 13 bit = middlegame Bonus from Piece Square Table + 4096
	// E: 13 bit = endgame Bonus from Piece Square Table + 4096
	
	
	cout << "Key: "<<key <<" = "<< bitset<64>(key)<<"\n";
	cout << "Seperate Parts: " ;
	for (int i=63;i>=0;i--){
		if (i==0){cout << " ";}
		if (i==4){cout << " ";}
		if (i==5){cout << "_";}
		if (i==8){cout << " ";}
		if (i==14){cout << " ";}
		if (i==22){cout << " ";}
		if (i==28){cout << " ";}
		if (i==41){cout << " ";}
		if (i==54){cout << " ";}
		cout << bitset<1>((key>>i)&1);
	}
	cout << "\nSeperate Parts: ????????? 8888888888888 7777777777777 666666 55555555 444444 333_3 2222 1\n\n";
	
	/***********
	*** Turn ***
	***********/
	
	cout << "1) Turn (1bit): ";
	if (key&1){cout << "White\n";}else{cout <<"Black\n";}
	
	/****************
	*** Casteling ***
	****************/
	
	cout << "2) Casteling (4bit): ";
	if (key&2){cout <<"K";}
	if (key&4){cout <<"Q";}
	if (key&8){cout <<"k";}
	if (key&16){cout <<"q";}
	cout << "\n";

	/*****************
	*** En Passant ***
	*****************/
	
	if (!(key &32)){
		cout<< "3) En Passant (3+1bit): none available\n";
	} else {
		cout << "3) En Passant (3+1bit): available in column "<< ((key>>6)&7)+1<<" (numbered 1-8)\n";
	}

	cout << "4) NrPiecesOnBoard (6 bit): There are " << ((key>>9)&63) <<" Pieces\n";
	cout << "5) Naive Score (8 bit): "<< int((key>>15)&255)-128 <<"\n";
	cout << "6) GamePhase (6 bit): "<< int((key>>23)&63) <<"\n";
	cout << "7) Piece Square Table Middlegame (13 bit): "<< int((key>>29)&8191)-4096 <<" cp\n";
	cout << "8) Piece Square Table Endgame (13 bit): "<< int((key>>42)&8191)-4096 <<" cp\n";
	cout << "\n";
}


void printFen(int board[8][8],unsigned long long int key){
	int nrEmpty,piece;
	
	for (int pointerRow=7; pointerRow>=0; pointerRow--){
		nrEmpty=0;
		
		for (int pointerCol=0; pointerCol<=7; pointerCol++){
			piece=board[pointerCol][pointerRow];
			if (piece==0){
				nrEmpty++;
			} else {
				if (nrEmpty!=0){cout << nrEmpty;}
				nrEmpty=0;
				if (piece== 1){cout << "P";}
				if (piece== 2){cout << "R";}
				if (piece== 3){cout << "N";}
				if (piece== 4){cout << "B";}
				if (piece== 5){cout << "Q";}
				if (piece== 6){cout << "K";}
				if (piece==-1){cout << "p";}
				if (piece==-2){cout << "r";}
				if (piece==-3){cout << "n";}
				if (piece==-4){cout << "b";}
				if (piece==-5){cout << "q";}
				if (piece==-6){cout << "k";}
			}
			
			if (pointerCol==7 && nrEmpty!=0){cout << nrEmpty;}
		}
		
		if (pointerRow!=0){cout << "/";}
	}
	cout<<" ";
	if (key& 1){cout << "w ";}else{cout <<"b ";}
	if (key& 2){cout << "K";}
	if (key& 4){cout << "Q";}
	if (key& 8){cout << "k";}
	if (key&16){cout << "q";}
	cout << "\n";
}



#endif