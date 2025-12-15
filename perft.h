/**************************
*** function depends on ***
**************************/

#include "assignMoveListAndSort.h"
#include "newKey.h"
#include "newHash.h"
#include "positionFeatures.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "printingStuff.h"


/************
*** Guard ***
************/

#ifndef perftFILE
#define perftFILE

/***************
*** Let's go ***
***************/

long long int perftBackground(int board[8][8], unsigned long long int key, long long int hash, int depth, int atDepth){
	
	/*******************************************
	**** Debugging/Testing: key (nrOfPiece) ****
	*******************************************/
	
	int nrPieces1=nrPiecesFunction(board,key);
	int nrPieces2=((key>>9)&63);

	if (nrPieces1 != nrPieces2){
		cout << "nrPieces is wrong\n";
		cout << "according to calculation: " << nrPieces1<< "\n";
		cout << "according to updated key: " << nrPieces2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	
	
	/***************************************
	**** Debugging/Testing: key (Score) ****
	***************************************/
	
	int score1= scoreFunction(board,key);
	int score2= int((key>>15)&255)-128;
	
	if (score1 != score2){
		cout << "score calculated is wrong\n";
		cout << "according to calculation: " << score1<< "\n";
		cout << "according to updated key: " << score2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	
	
	/*******************************************
	**** Debugging/Testing: key (gamePhase) ****
	*******************************************/
	
	int gamePhase1= gamePhaseFunction(board,key);
	int gamePhase2= int((key>>23)&63);
	
	if (gamePhase1 != gamePhase2){
		cout << "gamePhase calculated is wrong\n";
		cout << "according to calculation: " << gamePhase1<< "\n";
		cout << "according to updated key: " << gamePhase2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	
	
	
	/***************************************************
	**** Debugging/Testing: key (PieceSquareTables) ****
	***************************************************/
	
	
	int PSTMiddlegame1=PSTMiddlegameFunction(board,key);
	int PSTMiddlegame2=int((key>>29)&8191)-4096;
	if (PSTMiddlegame1 != PSTMiddlegame2){
		cout << "PSTMiddlegame calculated is wrong\n";
		cout << "according to calculation: " << PSTMiddlegame1<< "\n";
		cout << "according to updated key: " << PSTMiddlegame2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	

	int PSTEndgame1=PSTEndgameFunction(board,key);
	int PSTEndgame2=int((key>>42)&8191)-4096;
	if (PSTEndgame1 != PSTEndgame2){
		cout << "PSTEndgame calculated is wrong\n";
		cout << "according to calculation: " << PSTEndgame1<< "\n";
		cout << "according to updated key: " << PSTEndgame2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}

	/****************************************
	**** Debugging/Testing: HashFunction ****
	****************************************/
	
	if (hash!=hashFunction(board,key)){
		cout << "hash is wrong\n";
		cout << "according to hashFunction: " << hashFunction(board,key)<< "\n";
		cout << "according to updated Hash: " << hash<< "\n";
		cout << "difference is: " << hash - hashFunction(board,key)<< "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	
	/*********************
	**** Going deeper ****
	*********************/
	
	
	unsigned int moveList[250];
	long long int count;
	long long int positions=0;
	unsigned long long int newKeyy;
	long long int newHashh;
	
	
	assignMoveListAndSort(board,key,moveList,false);

	
	if (depth==1){
		return moveList[0];
	}
	
	
	for (int i=1; i<=moveList[0];i++){
		newKeyy=newKey(board,key,moveList[i]);
		newHashh=newHash(board,key,hash,moveList[i]);
		
		assignMakeMove(board,moveList[i]);
		
		count=perftBackground(board,newKeyy,newHashh,depth-1,atDepth+1);
		positions+=count;
		if (atDepth==0){
			cout << move2string(moveList[i])<<": "<<count<<"\n";
		}
		
		assignUndoMove(board,moveList[i]);
		
	}
	return positions;
}

void perft(int board[8][8], unsigned long long int key, int depth){
	long long int count;
	long long int hash=hashFunction(board,key);
	int asd1,asd2;
	asd1=clock();
	count=perftBackground(board,key, hash, depth, 0);
	asd2=clock();
	
	cout << "\nNodes: "<<count<<"\n";
	cout << "perft duration: "<<asd2-asd1<< "\n";
}

#endif