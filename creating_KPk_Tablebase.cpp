#include <iostream>
#include <unistd.h>     //for sleep(1)
#include <string>       // for fen2Board
#include <stdlib.h>     // srand, rand
#include <time.h>       // time, clock_t, clock, CLOCKS_PER_SEC
#include <iomanip>      // for printing 2 floats
#include <time.h>       // clock_t, clock, CLOCKS_PER_SEC
#include <algorithm>    // std::max
#include <fstream>      // to write to a txt file

using namespace std;

#include "plotBoard.h"
#include "isKingInCheck.h"
#include "assignMakeMove.h"
#include "assignUndoMove.h"
#include "assignMoveList.h"
#include "printMoves.h"

/***********
**** this script creates a KPk tablebase
**** White is always the side with the extra pawn
**** This Pawn will always be on either of the columns a-d
**** We number the squares: a1=0 a2=1, a3=2,..., b1=8, ... h1=56, h8=63
**** This unusual numbering is necessary to ensure that Pawn position is on a square [0,31]
**** Pawn will always be on 
**** The encoding is K+64*k+64*64*P+64*64*32*[whiteTurn=true] using iverson notation
**** the value 0 represents a drawpoly
**** the number 900 represents blacks move, white just queened and black not in stalemate
**** the number 900-x represents, there is a queen in x plys
**** I expect there to be 64*64*32*2=262.144 positions
************/


void assignHash2board(int board[8][8], int hash){
	int position_K;
	int position_k;
	int position_P;

	int atPosition=-1;
	
	position_K=hash%64;
	position_k=(hash/64)%64;
	position_P=(hash/(64*64))%32;
	
	for (int col=0; col<8;col++){
		for (int row=0; row<8;row++){
			atPosition++;
			board[col][row]=0;
			if (atPosition==position_K){board[col][row]=6;}
			if (atPosition==position_P){board[col][row]=1;}
			if (atPosition==position_k){board[col][row]=-6;}	
		}
	}
	
	return;
}

int hash2turn(int hash){
	if (hash>=64*64*32){return 1;}
	return -1;
}

int board2Hash(int board[8][8],int turn){
	int position_K;
	int position_P;
	int position_k;
	
	int atPosition=-1;
	for (int col=0; col<8;col++){
		for (int row=0; row<8;row++){
			atPosition++;
			if (board[col][row]==6){position_K=atPosition;}
			if (board[col][row]==1){position_P=atPosition;}
			if (board[col][row]==-6){position_k=atPosition;}
		}
	}
	int hash=position_K+64*position_k+64*64*position_P;
	if (turn==1){hash+=64*64*32;}
	
	return hash;
}

int board2HashQueenAdjusted(int board[8][8],int turn){
	int position_K;
	int position_k;
	int position_P;

	
	int atPosition=-1;
	for (int col=0; col<8;col++){
		for (int row=0; row<8;row++){
			atPosition++;
			if (board[col][row]==6){position_K=atPosition;}
			if (board[col][row]==5){position_P=atPosition;}
			if (board[col][row]==-6){position_k=atPosition;}
		}
	}
	int hash=position_K+64*position_k+64*64*position_P;
	if (turn==1){hash+=64*64*32;}
	
	return hash;
}

bool fullMaterial(int board[8][8]){
	bool found_K=false;
	bool found_P=false;
	bool found_k=false;
	
	
	for (int row=0; row<8;row++){
		for (int col=0; col<8;col++){
			if (board[col][row]==6){found_K=true;}
			if (board[col][row]==1){found_P=true;}
			if (board[col][row]==-6){found_k=true;}
		}
	}

	
	return (found_K&& found_P&&found_k);
}

int nrPieces(int board[8][8]){
	int res=0;
	for (int row=0; row<8;row++){
		for (int col=0; col<8;col++){
			if (board[col][row]!=0){res++;}
		}
	}
	return (res);
}


bool piecesNextToEachOther(int board[8][8], int piece1, int piece2){
	int row2;
	int col2;
	for (int row=0; row<8;row++){
		for (int col=0; col<8;col++){
			if (board[col][row]==piece1){ // piece1 found
			
				for (int dx=-1;dx<=1;dx++){
					for (int dy=-1;dy<=1;dy++){
						row2=row+dx;
						col2=col+dy;
						if (row2<0){continue;}
						if (row2>7){continue;}
						if (col2<0){continue;}
						if (col2>7){continue;}			
						if (board[col2][row2]==piece2){ // piece2 found
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

int main(){
	
	/***************************
	**** initializing stuff ****
	***************************/	
	
	int tbSize=64*64*32*2;
	short int tb[tbSize]; // faszinierender weise hängt sich das programm auf, wenn ich "int" statt "short int" benutze...
	for (int i=0;i<tbSize;i++){tb[i]=0;}
	
	int board[8][8]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int turn;
	int position_K;
	int position_k;
	int position_P;

	int pawnCol;
	int pawnRow;
	
	int winsFound=0;
	int tbSum=0;
	cout  <<"h1\n";
	bool is_K_nearby;	
	bool is_k_nearby;
	
	int moveList[1000];
	int scores[200];
	int bestScore;
	
	/**************************************************
	**** finding +1 evals (aka pawn just promoted) ****
	**************************************************/
	cout  <<"h1\n";
	
	for (int hash=0;hash<tbSize;hash++){
		assignHash2board(board,hash);
		turn=hash2turn(hash);
		
		// check if turn and pieces are correct;
		if (turn==1){continue;}
		if (!fullMaterial(board)){continue;}
		
		// check if pawn just promoted;
		position_K=hash%64;
		position_k=(hash/64)%64;
		position_P=(hash/(64*64))%32;
		
		if (position_P!=7 && position_P!=15 && position_P!=23 && position_P!=31){continue;}
		
		// check if K and k are next to one another
		
		if (piecesNextToEachOther(board,6,-6)){continue;}

		// check if k could capture Q
		
		if (piecesNextToEachOther(board,1,-6)){
			if (!piecesNextToEachOther(board,1,6)){
				continue;
			}
		}
		
		// check if k in stalemate ----- for assignMakeMove we have to convert the pawn into a queen for a moment;
		for (int row=0; row<8;row++){
			for (int col=0; col<8;col++){
				if (board[row][col]==1){board[row][col]=5;}
			}
		}
		if (!isKingInCheck(board,-1)){
			assignMoveList(board,-1,moveList,true);
			if (moveList[0]==0){
				continue;
			}
		}
		tb[hash]=900; // finally setting the tb entry
		// there should be the end of the loop, as the queen was not converted back to the pawn
		

	}

	int hash,newHash;
	
	
	cout << "finished finding Queen conversion\n";
	winsFound=0;
	tbSum=0;
	for (int i=0;i<tbSize;i++){tbSum+=tb[i];}
	for (int i=0;i<tbSize;i++){if (tb[i]>0){winsFound++;}}
	cout << "there are currently "<< winsFound<< " wins and tbSum " << tbSum <<"\n";	
	
	
	
	/********************************************
	**** finding the other winning positions ****
	********************************************/
	
	for (int GREATLOOP=0;GREATLOOP<22;GREATLOOP++){
		for (int hash=0;hash<tbSize;hash++){
			
			assignHash2board(board,hash);
			turn=hash2turn(hash);
			
			position_K=hash%64;
			position_k=(hash/64)%64;
			position_P=(hash/(64*64))%32;
			
			// check if this is a legal position where no promotion has occured:
			if (!fullMaterial(board)){continue;}
			if (position_P==7 || position_P==15 || position_P==23 || position_P==31){continue;}
			if (position_P==0 || position_P==8 || position_P==16 || position_P==24){continue;}

			if (piecesNextToEachOther(board,6,-6)){continue;}
			if (turn==1 && isKingInCheck(board,-1)){continue;}
			
			// generating the moveList and check for stalemate
			assignMoveList(board,turn,moveList,true);
			if (moveList[0]==0){continue;}
			
			
			
			
			// calculate all possible scores
			scores[0]=moveList[0];
			
			for (int i=1;i<=moveList[0];i++){
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
				if (fullMaterial(board)){ // there are 2 Kings and 1 Pawn
					newHash=board2Hash(board,-turn);
					scores[i]=tb[newHash];
				} else {
					if (nrPieces(board)==3){ // there are 2 Kings and 1 Queen
						newHash=board2HashQueenAdjusted(board,-turn);
						scores[i]=tb[newHash];
					} else { // there are only 2 Kings
						scores[i]=0;
					}
				}
				//if (hash==265248){cout << scores[i]<< "\n";}
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
			}
			
			// now, based on all the possible scores, we assign the new tb entry
			
			
			if (turn==1){ // white to move
				bestScore=0;
				for (int i=1;i<=moveList[0];i++){
					if (scores[i]-1>bestScore){
						bestScore=scores[i]-1;
					}
				}
			} else {
				bestScore=1000;
				for (int i=1;i<=moveList[0];i++){
					if (scores[i]-1<bestScore){
						bestScore=scores[i]-1;
					}
				}
				if (bestScore==-1){bestScore=0;}
			}
			
			tb[hash]=bestScore;
			
		}
		
		winsFound=0;
		tbSum=0;
		for (int i=0;i<tbSize;i++){tbSum+=tb[i];}
		for (int i=0;i<tbSize;i++){if (tb[i]>0){winsFound++;}}
		cout << "GREATLOOP: " << GREATLOOP<< "  ...  ";
		cout << "there are currently "<< winsFound<< " wins and tbSum " << tbSum <<"\n";	

	}
	
	
	/*******************************
	**** having fun with the tb ****
	*******************************/	
	for (int hash=0; hash<tbSize; hash=hash+10000){
		cout << hash<< "\n";
		assignHash2board(board,hash);
		turn=hash2turn(hash);
		
		plotBoard(board);
		cout << "Turn: "<<turn<<"\n";
		cout << "the score is "<< tb[hash] << "\n\n\n";
	}


	/********************************
	**** having fun with the tb2 ****
	********************************/	

	for (int hash=0; hash<tbSize; hash++){
		

		if (tb[hash]<0){cout << "lol there is a negative score \n";
			assignHash2board(board,hash);
			turn=hash2turn(hash);
			
			plotBoard(board);
			cout << "Turn: "<<turn<<"\n";
		}
		
	}


	/*************************
	**** exporting the db ****
	*************************/	
	
	
	ofstream MyFile("TABLEBASE_KPk.h");

	MyFile << "//This is a KPk Database";
	MyFile << "//White is always the side with the extra pawn\n";
	MyFile << "//Pawn will always be on a-d column\n";
	MyFile << "//We number the squares: a1=0 a2=1, a3=2,..., b1=8, ... , h8=63\n";
	MyFile << "//The encoding is K+64*k+64*64*P+64*64*32*[whiteTurn=true] using iverson notation\n";
	MyFile << "//the value 0 represents a draw\n";
	MyFile << "//the number 900 represents blacks move, white just queened and black not in stalemate\n";
	MyFile << "//the number 900-x represents, there is a queen in x plys\n";
	MyFile << "//Databse consists of 64*64*32*2=262.144 positions\n\n\n";

	
	/*
	MyFile << "short int TABLEBASE_KPk["<<tbSize<<"]; \n\n\n";
	
	for (int hash=0; hash<tbSize; hash++){
		if (hash%100000==0){cout << hash<<"\n";}
		MyFile << "TABLEBASE_KPk["<<hash<<"]="<< tb[hash]<<"; \n";
	}
	*/

	MyFile << "short int TABLEBASE_KPk["<<tbSize<<"]={";
	for (int hash=0; hash<tbSize; hash++){
		if (hash%100000==0){cout << hash<<"\n";}
		MyFile << tb[hash];
		if (hash < tbSize -1){MyFile << ",";}
		
	}
	MyFile << "};";
	
	
	
	
	MyFile.close();
	
	
	
	/**********************
	**** waiting @ end ****
	**********************/
	cout << "\nyou have reached the end\n";
	int asd;
	cin >> asd;
	return 1;
	
	
	
	
	
	
	
}