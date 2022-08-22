/**************************
*** function depends on ***
**************************/

#include "assignMakeMove.h"
#include "newKey.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "printMoves.h"

/************
*** Guard ***
************/

#ifndef perftFILE
#define perftFILE

/***************
*** Let's go ***
***************/

long int perftBackground(int board[8][8], unsigned long long int key, int depth, int atDepth){
	int moveList[250*5];
	long int count;
	unsigned long long int newKeyy;
	
	assignMoveList(board,key,moveList, true);

	/*
	// testing if key generation is flawless: eval
	float score1;
	score1=stationaryEvalV1(board,1);
	int score2;
	score2= int((key>>15)&255)-128;
	if (score1 != score2){cout << "score is calculated wrong\n";}
	
	
	// testing if key generation is flawless: nrPieces	
	int nrPieces1;
	nrPieces1=nrPieces(board);
	int nrPieces2;
	nrPieces2=((key>>9)&63);
	if (nrPieces1 != nrPieces2){cout << "nrPieces is calculated wrong\n";}
	*/
	
	
	
	
	if (depth ==1){
		return moveList[0];
	}
		
	int positions=0;
	for (int i=1; i<=moveList[0];i++){
		newKeyy=newKey(board,key,moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
		assignMakeMove(board,moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
		
		count=perftBackground(board,newKeyy,depth-1,atDepth+1);
		positions+=count;
		if (atDepth==0){
			//cout << "\n\n\n";
			printMove(moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
			cout <<": "<<count<<"\n";
			//plotBoard(board);
		}
		
		assignUndoMove(board,moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
	}
	return positions;
}

void perft(int board[8][8], unsigned long long int key, int depth){
	long int count;
	count=perftBackground(board,key, depth, 0);
	cout << "\nNodes: "<<count<<"\n";
}

#endif