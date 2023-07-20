/**************************
*** function depends on ***
**************************/

#include "assignMakeMove.h"
#include "newKey.h"
#include "newHash.h"
#include "hashFunction.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "printMoves.h"
#include "printKey.h"

/************
*** Guard ***
************/

#ifndef perftFILE
#define perftFILE

/***************
*** Let's go ***
***************/

long int perftBackground(int board[8][8], unsigned long long int key, long long int hash, int depth, int atDepth){
	
	/*******************************************
	**** Debugging/Testing: key (nrOfPiece) ****
	*******************************************/
	
	int nrPieces1=0,nrPieces2;
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			if (board[i][j]!=0){
				nrPieces1+=1;
			}
		}
	}
	nrPieces2=((key>>9)&63);

	if (nrPieces1!=nrPieces2){
		cout << "nrPieces is wrong\n";
		cout << "according to counting: " << nrPieces1<< "\n";
		cout << "according to updated key: " << nrPieces2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	
	/*******************************************
	**** Debugging/Testing: key (Score) ****
	*******************************************/
	
	int score1=0,score2;
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			if (board[i][j]== 1){score1+=1;}
			if (board[i][j]== 2){score1+=5;}
			if (board[i][j]== 3){score1+=3;}
			if (board[i][j]== 4){score1+=3;}
			if (board[i][j]== 5){score1+=9;}
			if (board[i][j]==-1){score1-=1;}
			if (board[i][j]==-2){score1-=5;}
			if (board[i][j]==-3){score1-=3;}
			if (board[i][j]==-4){score1-=3;}
			if (board[i][j]==-5){score1-=9;}
		}
	}
	score2= int((key>>15)&255)-128;
	if (score1 != score2){
		cout << "score calculated is wrong\n";
		cout << "according to counting: " << score1<< "\n";
		cout << "according to updated key: " << score2 << "\n";
		plotBoard(board);
		printKey(key);
		cout << "\n\n";
	}
	
	
	
	
	/************************************************
	**** Debugging/Testing: key (bonus from PST) ****
	************************************************/

	int bonus1=0,bonus2;
	for (int c=0; c<8; c++){
		for (int r=0; r<8; r++){
			bonus1+= pieceSquareTable[6+board[c][r]][c][r];
		}
	}
	bonus2=(int((key>>23)&1023)-512);
	if (bonus1 != bonus2){
		cout << "bonus calculated is wrong\n";
		cout << "according to counting: " << bonus1<< "\n";
		cout << "according to updated key: " << bonus2 << "\n";
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
	
	
	int moveList[250*5];
	long int count;
	unsigned long long int newKeyy;
	long long int newHashh;
	
	
	assignMoveList(board,key,moveList);

	
	if (depth==1){
		return moveList[0];
	}
		
	int positions=0;
	for (int i=1; i<=moveList[0];i++){
		newKeyy=newKey(board,key,moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
		newHashh=newHash(board,key,hash,moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
		
		assignMakeMove(board,moveList[i*5-4],moveList[i*5-3],moveList[i*5-2],moveList[i*5-1],moveList[i*5]);
		
		count=perftBackground(board,newKeyy,newHashh,depth-1,atDepth+1);
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
	long long int hash=hashFunction(board,key);
	count=perftBackground(board,key, hash, depth, 0);
	cout << "\nNodes: "<<count<<"\n";
}

#endif