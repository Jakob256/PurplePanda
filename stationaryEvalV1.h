/**************************
*** function depends on ***
**************************/

#include "stationaryEvalTB.h"
#include "stationaryEvalOpponentAtEdge.h"

/************
*** Guard ***
************/

#ifndef stationaryEvalV1FILE
#define stationaryEvalV1FILE

/***************
*** Let's go ***
***************/

float stationaryEvalV1 (int board[8][8],unsigned long long int key){
	countingStationaryEvalCalled++;
	float eval;
	int turn=-1+2*(key%2);
	int nrPieces=((key>>9)&63);
	eval=int((key>>15)&255)-128;
	int bonus=(int((key>>23)&1023)-512);  // since PP10

	if (abs(eval)==3 && nrPieces==3){return 0.;}  // lone bishop or knight cant checkmate

	if (abs(eval)==1 && nrPieces==3){return stationaryEvalTB(board,turn);} // pawn endgame
	
	if (abs(eval)==9 && nrPieces==3){eval=100*eval;}// to keep in line with the next line

	if (moveOpponentToEdge!=0){eval+=stationaryEvalOpponentAtEdge(board,turn,moveOpponentToEdge);}
	
	eval+=bonus/100.;  
		
	return eval;
	
	
	
	
	/*
	float eval=0.;
	float evalWhite=0.;
	float evalBlack=0.;
	
	float factor=1.;
	int piece;
	
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			piece=board[col][row];
			if (abs(piece)==0){continue;}
			//factor=(7-abs(row-3.5)-abs(col-3.5))*0.01+1.;   // center has factor=6*0.01+1=1.06 // corner has factor 1
			//cout <<factor<<"\n";
			if (piece==-6){evalBlack+=1000.;}
			if (piece==-5){evalBlack+=10.;}
			if (piece==-4){evalBlack+=3.;}
			if (piece==-3){evalBlack+=3.;}
			if (piece==-2){evalBlack+=5.;}
			if (piece==-1){evalBlack+=1.;}
			if (piece==1){evalWhite+=1.;}
			if (piece==2){evalWhite+=5.;}
			if (piece==3){evalWhite+=3.;}
			if (piece==4){evalWhite+=3.;}
			if (piece==5){evalWhite+=10.;}
			if (piece==6){evalWhite+=1000.;}
			
		}
	}
	eval=evalWhite-evalBlack;
	if (evalWhite+evalBlack<2000){return eval;} // there is a king missing
	if (evalWhite+evalBlack==2001.){return stationaryEvalTB(board,turn);}
	
	/************************
	**** casteling bonus ****
	*************************	
	

	if (evalBlack>=1025){
		if (board[4][0]==6){eval+=0.01;}// better still on home square

		bool whiteKingIsCastled=false;

		if (board[0][0]==6){whiteKingIsCastled=true;}
		if (board[1][0]*board[1][0]+board[0][0]==36){whiteKingIsCastled=true;}
		if (board[6][0]*board[6][0]+board[7][0]==36){whiteKingIsCastled=true;}
		if (board[7][0]==6){whiteKingIsCastled=true;}
		
		if (whiteKingIsCastled){eval+=0.03;}
	}
	if (evalWhite>=1025){
		if (board[4][7]==-6){eval-=0.01;}// better still on home square

		bool blackKingIsCastled=false;

		if (board[0][7]==-6){blackKingIsCastled=true;}
		if (board[1][7]*board[1][7]+board[0][7]==36){blackKingIsCastled=true;}
		if (board[6][7]*board[6][7]+board[7][7]==36){blackKingIsCastled=true;}
		if (board[7][7]==-6){blackKingIsCastled=true;}
		
		if (blackKingIsCastled){eval-=0.03;}
	}
	
	
	/*******************************************************
	**** return imediately if no further eval necessary ****
	*******************************************************

	
	if (evalWhite>1013 && evalBlack>1013){return eval;} 

	/*********************************************
	**** get more exact evaluation in endgame ****
	*********************************************

	int nrWhitePawns=0;
	int nrWhiteRooks=0;
	int nrWhiteKnights=0;
	int nrWhiteBishops=0;
	int nrWhiteQueens=0;	
	int nrBlackPawns=0;
	int nrBlackRooks=0;
	int nrBlackKnights=0;
	int nrBlackBishops=0;
	int nrBlackQueens=0;

	
	
	for (int col=0;col<8;col++){
		for (int row=0;row<8;row++){
			piece=board[col][row];
			if (abs(piece)==0){continue;}
			if (piece==-5){nrBlackQueens++;}
			if (piece==-4){nrBlackBishops++;}
			if (piece==-3){nrBlackKnights++;}
			if (piece==-2){nrBlackRooks++;}
			if (piece==-1){nrBlackPawns++;}
			if (piece==1){nrWhitePawns++;}
			if (piece==2){nrWhiteRooks++;}
			if (piece==3){nrWhiteKnights++;}
			if (piece==4){nrWhiteBishops++;}
			if (piece==5){nrWhiteQueens++;}
		}
	}
	
	// one side has only King:
	
	//k vs Q+ 	-> eval=910 + Edge
	//k vs R+ 	-> eval=910 + Edge	
	//k vs B/N	-> eval=0
	//k vs NN 	-> eval=0
	//k vs BN/BB 	-> eval=Edge
	//k vs PPPPPP  -> eval=nrPawns*3

	if (evalWhite==1000.){
		if (nrBlackQueens>=1 || nrBlackRooks>=1 ){ // there are queens or rooks
			return -910+stationaryEvalOpponentAtEdge(board,turn,-1);
		} else if (nrBlackPawns==0 && evalBlack==3){ // lone B or N is draw.
			return 0;
		} else if (evalBlack==1006. && nrBlackKnights==2){
			return 0; //two knights are a draw
		} else if (nrBlackPawns==0 && nrBlackBishops>=1 && nrBlackBishops+nrBlackKnights>=2 ){ // we have only minorpieces
			return stationaryEvalOpponentAtEdge(board,turn,-1);
		} else if (nrBlackPawns+1000==evalBlack){// has only pawns
			return (-3*nrBlackPawns);
		}
	}

	if (evalBlack==1000.){
		if (nrWhiteQueens>=1 || nrWhiteRooks>=1 ){ // there are queens or rooks
			return 910+stationaryEvalOpponentAtEdge(board,turn,1);
		} else if (nrWhitePawns==0 && evalWhite==3){ // lone B or N is draw.
			return 0;
		} else if (evalWhite==1006. && nrWhiteKnights==2){
			return 0; //two knights are a draw
		} else if (nrWhitePawns==0 && nrWhiteBishops>=1 && nrWhiteBishops+nrWhiteKnights>=2 ){ // we have only minorpieces
			return stationaryEvalOpponentAtEdge(board,turn,1);
		} else if (nrWhitePawns+1000==evalWhite){// has only pawns
			return (3*nrWhitePawns);
		}
	}

	if (evalWhite==1003 && evalBlack==1003){ // if both players a single minor piece
		if (nrWhiteBishops+nrWhiteKnights==1 && nrBlackKnights + nrBlackBishops==1){
			return 0;
		}
	}
	
	
			
	
	
	//* final eval corrigation **
	
	if (evalWhite==1003 && nrWhiteBishops+nrWhiteKnights==1){ // when white one minor piece, return only piece value of black
		return eval-=3;
	}
	if (evalBlack==1003 && nrBlackBishops+nrBlackKnights==1){ // when white one minor piece, return only piece value of white
		return evalWhite+=3;
	}	



	return eval;
	
	*/
}

#endif