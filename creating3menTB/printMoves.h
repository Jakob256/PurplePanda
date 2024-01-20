/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef printMovesFILE
#define printMovesFILE

/***************
*** Let's go ***
***************/


void printMove (int c1, int r1, int c2, int r2, int adInfo){
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
	
	// is promotion then explain which promotion
	int promotedPiece=int((adInfo>>4)%8);
	if (promotedPiece==2){cout << "r";}
	if (promotedPiece==3){cout << "n";}
	if (promotedPiece==4){cout << "b";}
	if (promotedPiece==5){cout << "q";}
}

void printMoveList (int moveList[]){
	for (int i=0; i<moveList[0];i++){
		cout << i+1<< ": ";
		printMove(moveList[5*i+1],moveList[5*i+2],moveList[5*i+3],moveList[5*i+4],moveList[5*i+5]);
		if (moveList[5*i+5]%8==1){cout << " capturing a pawn";}
		if (moveList[5*i+5]%8==2){cout << " capturing a roook";}
		if (moveList[5*i+5]%8==3){cout << " capturing a knight";}
		if (moveList[5*i+5]%8==4){cout << " capturing a bishop";}
		if (moveList[5*i+5]%8==5){cout << " capturing a queen";}
		if (moveList[5*i+5]%8==6){cout << " capturing a king";}
		
		cout<< "\n";
	}
}

#endif