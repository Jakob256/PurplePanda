/**************************
*** function depends on ***
**************************/

//nothing - however optimized for PP9+ probably

/************
*** Guard ***
************/

#ifndef assignSortMovesV1FILE
#define assignSortMovesV1FILE

/***************
*** Let's go ***
***************/


void assignSortMovesV1 (int moveList[], int board[8][8]){
	// will sort the moves in moveList;
	// for now, it might not use the board
	if (moveList[0]<=1){return;}
	



	int quickEvals[moveList[0]+1]; // will start with position 1
	int startPiece,endPiece;
	// start with a score of 0
	// add: captures get a score of abs(takenPiece)-abs(myPiece)+10
	// add: promotions get an extra score of 10
	
	int nrNonZeroMoves=0;
	
	for (int i=1; i<=moveList[0]; i++){
		quickEvals[i]=0;
		if (moveList[5*i]==0){continue;} // there is nothing
		nrNonZeroMoves++;
		if (moveList[5*i]==16){quickEvals[i]=10;continue;} // there was only promotion
		
		startPiece=board[moveList[5*i-4]][moveList[5*i-3]];
		endPiece=board[moveList[5*i-2]][moveList[5*i-1]];
		
		if (abs(startPiece)==6){startPiece=100;}
		if (abs(startPiece)==5){startPiece=10;}
		if (abs(startPiece)==2){startPiece=5;}
		if (abs(startPiece)==4){startPiece=3;}
		startPiece=abs(startPiece);
		if (abs(endPiece)==6){endPiece=100;}
		if (abs(endPiece)==5){endPiece=10;}
		if (abs(endPiece)==2){endPiece=5;}
		if (abs(endPiece)==4){endPiece=3;}
		endPiece=abs(endPiece);
		
		if (abs(moveList[5*i])<=7){ quickEvals[i]=endPiece-startPiece+10;}// there was only captures
		if (abs(moveList[5*i])>7){ quickEvals[i]=endPiece-startPiece+10;}// there was only capture + promotion
	}
	
	
	// now we need to order the moveList according to the quickEvals
	// will now successifely move good moves to the left. Will only have to do this nrNonZeroMoves-times.
	int hv;
	
	for (int counter=0; counter<=nrNonZeroMoves; counter++){
		for (int j=moveList[0];j>=2;j--){ // will take element j-1 and j
			if (quickEvals[j-1]<quickEvals[j]){
				hv=quickEvals[j-1];quickEvals[j-1]=quickEvals[j];quickEvals[j]=hv;
				for (int k=0;k<=4;k++){
					hv=moveList[5*(j-1)-k];moveList[5*(j-1)-k]=moveList[5*j-k];moveList[5*j-k]=hv;
				}
			}
		}
	}
	return;
	
}

#endif