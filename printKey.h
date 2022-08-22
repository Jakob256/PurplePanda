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



// 64bit-key in Binary: ??? BBBBBBBBBB SSSSSSSS PPPPPP eeeE CCCC T
//                          1000000000 10000000 100000 0000 1111 1

// PPPPPP = nrPiecesOnBoard: 
// SSSSSS = Score +128
// BBBBBBBBBB = Bonus from Piece Square Table + 512



void printKey(unsigned long long int key){
	/******************
	*** printingKey ***
	******************/
	
	cout << "Key: "<<key <<" = "<< bitset<64>(key)<<"\n";
	cout << "Readable: " ;
	for (int i=63;i>=0;i--){
		if (i==0){cout << " ";}
		if (i==4){cout << " ";}
		if (i==5){cout << "_";}
		if (i==8){cout << " ";}
		if (i==14){cout << " ";}
		if (i==22){cout << " ";}
		if (i==32){cout << " ";}  // try PP10
		cout << bitset<1>((key>>i)&1);
	}
	cout << "\n";
	
	/***********
	*** Turn ***
	***********/
	
	if (key&1){cout << "Turn (1bit): White\n";}else{cout <<"Turn (1bit): Black\n";}
	
	/****************
	*** Casteling ***
	****************/
	
	cout << "Casteling (4bit): ";
	if (key&2){cout <<"K";}
	if (key&4){cout <<"Q";}
	if (key&8){cout <<"k";}
	if (key&16){cout <<"q";}
	cout << "\n";

	/*****************
	*** En Passant ***
	*****************/

	cout << "En Passant (3+1bit): ";
	if (!(key &32)){
		cout<< "none available\n";
	} else {
		cout << "available in column "<< ((key>>6)&7)+1<<" (numbered 1-8)\n";
	}

	
	/**********************
	*** NrPiecesOnBoard ***
	**********************/

	cout << "NrPiecesOnBoard (6 bit): ";
	cout<< "There are "<< ((key>>9)&63)<<" Pieces\n";
	
	/******************
	*** Naive Score ***
	******************/

	cout << "Naive Score (8 bit): ";
	cout<< int((key>>15)&255)-128<<"\n";

	cout << "\n";
	
	
	/************************************
	*** Bonus from Piece Square Table *** - try for PP10
	************************************/

	cout << "Bonus from Piece Square Table (10 bit): ";
	cout<< int((key>>23)&1023)-512<<"\n";

	cout << "\n";
	return;
}

#endif

