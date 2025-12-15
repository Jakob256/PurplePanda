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
// move2string
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




string move2string(unsigned int move){
	short int c1=move&0b111;
	short int r1=(move>>3)&0b111;
	short int c2=(move>>6)&0b111;
	short int r2=(move>>9)&0b111;
	short int absPromotedTo=(move>>12)&0b111;
	//short int absCaptured=(move>>15)&0b111;
	//bool enPassant =(move>>18)&0b1;
	
	string moveString;
	moveString+=char('a'+c1);
	moveString+=to_string(r1+1);
	moveString+=char('a'+c2);
	moveString+=to_string(r2+1);
	
	// if promotion then print which promotion
	if (absPromotedTo==2){moveString+="r";}
	if (absPromotedTo==3){moveString+="n";}
	if (absPromotedTo==4){moveString+="b";}
	if (absPromotedTo==5){moveString+="q";}
	
	return moveString;
}



void printMoveList (unsigned int moveList[]){
	short int absCaptured;
	for (int i=1; i<=moveList[0];i++){
		unsigned int move=moveList[i];
		absCaptured=(move>>15)&0b111;
		
		if (absCaptured==0){cout << i << ": "<<move2string(move)<<"\n";}
		if (absCaptured==1){cout << i << ": "<<move2string(move)<<" capturing a pawn\n";}
		if (absCaptured==2){cout << i << ": "<<move2string(move)<<" capturing a rook\n";}
		if (absCaptured==3){cout << i << ": "<<move2string(move)<<" capturing a knight\n";}
		if (absCaptured==4){cout << i << ": "<<move2string(move)<<" capturing a bishop\n";}
		if (absCaptured==5){cout << i << ": "<<move2string(move)<<" capturing a queen\n";}
		if (absCaptured==6){cout << i << ": "<<move2string(move)<<" capturing a king\n";}
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
}



#endif