/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef printKeyFILE
#define printKeyFILE

/***************
*** Let's go ***
***************/



// 64bit-key bits: EEEEEEEEEEEEE MMMMMMMMMMMMM PHPHPH SSSSSSSS PPPPPP eeeE CCCC T
// start key bits: 1000000000000 1000000000000 011000 10000000 100000 0000 1111 1

// T: 1 bit = turn
// C: 4 bit = casteling rights
// P: 6 bit = nrPiecesOnBoard 
// S: 8 bit = naiveScore +128
// PH: 6 bit = game Phase
// M: 13 bit = midlegame Bonus from Piece Square Table + 4096
// E: 13 bit = midlegame Bonus from Piece Square Table + 4096


void printKey(unsigned long long int key){
	/******************
	*** printingKey ***
	******************/
	
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
	cout << "\n";
	
	cout << "Seperate Parts: ????????? 8888888888888 7777777777777 666666 55555555 444444 333_3 2222 1\n\n";
	
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

	cout << "3) En Passant (3+1bit): ";
	if (!(key &32)){
		cout<< "none available\n";
	} else {
		cout << "available in column "<< ((key>>6)&7)+1<<" (numbered 1-8)\n";
	}

	
	/**********************
	*** NrPiecesOnBoard ***
	**********************/

	cout << "4) NrPiecesOnBoard (6 bit): There are " << ((key>>9)&63) <<" Pieces\n";
	
	/******************
	*** Naive Score ***
	******************/

	cout << "5) Naive Score (8 bit): "<< int((key>>15)&255)-128 <<"\n";

	/******************
	*** GamePhase ***
	******************/

	cout << "6) GamePhase (6 bit): "<< int((key>>23)&63) <<"\n";

	
	/********************************************
	*** Piece Square Table Middlegame/Endgame ***
	********************************************/

	cout << "7) Piece Square Table Middlegame (13 bit): "<< int((key>>29)&8191)-4096 <<" cp\n";

	cout << "8) Piece Square Table Endgame (13 bit): "<< int((key>>42)&8191)-4096 <<" cp\n";

	cout << "\n";
	
	return;
}

#endif

