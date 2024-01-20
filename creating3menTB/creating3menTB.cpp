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
**** white will alawys be the side to move
**** the order of the hashtables is KvK, B, N, P, R, Q
**** KvK, KvKB and KvKN will all be mapped to the hash=0
**** General hash= positionWK + c1*positionBK + c2*positionPiece + c3*piecetype
**** For KPvK, we restrict the white King to the first 4 columns. 
**** --> We number the squares: a1=0 a2=1, a3=2,..., b1=8, ... h1=56, h8=63
**** For KRvK or KQvK, we restict the King to the A1,D1,D4 triangle.
**** --> We number the squares: a1=0, b1=0, b2=2, c1=3, c2=4, c3=5 ,..., d4=9. (therefore same order as squares)
**** The constants will be as small as possible
**** The square indices will also be constants will be as small as possible
**** The pieces indices will be numbered from the smallest piece to the highest
**** 
**** 1) [0,0] -> any KvK, KvKN or KvKB
**** 2) [1,32*64*48*2] = [1,196608] any KvKP
**** 3) 196608+[1, 10*64*64*4]=196608+[1, 163840]= [196609,360448]
**** so in total 360449 positions
**** 
**** the number 0 represents a draw
**** the number +n represents mate in n-1 plys for the current player
**** the number -n represents mate in n-1 plys for the opposite player
**** so -1 means currently checkmate
************/


void assignHash2board(int board[8][8], int hash){
	// clearing the board
	int posWhiteKing,posBlackKing,posPiece,pieceType;
	
	
	for (int col=0; col<8;col++){
		for (int row=0; row<8;row++){
			board[col][row]=0;
		}
	}
	
	if (hash==0){// draw by insufficient material
		board[0][0]=6;
		board[2][2]=-6;
		return;
	}
	
	if (1<=hash && hash<= 196608){// Pawn
		hash=hash-1;
		posWhiteKing=hash%32;
		posBlackKing=(hash/32)%64;
		posPiece=(hash/(32*64))%48;
		pieceType=(hash/(32*64*48))%2;
		
		

		board[posWhiteKing/8][posWhiteKing%8]=6;
		board[posBlackKing/8][posBlackKing%8]=-6;
		if (pieceType==0){board[posPiece/6][posPiece%6+1]=-1;}
		if (pieceType==1){board[posPiece/6][posPiece%6+1]=1;}
		return;
	}
	
	
	if (196609<=hash && hash<= 360448){// Rook or Queen
		hash=hash-196609;
		posWhiteKing=hash%10;
		posBlackKing=(hash/10)%64;
		posPiece=(hash/(10*64))%64;
		pieceType=(hash/(10*64*64))%4;
		

		if (posWhiteKing>=1){posWhiteKing+=7;}
		if (posWhiteKing>=10){posWhiteKing+=6;}
		if (posWhiteKing>=19){posWhiteKing+=5;}

		board[posWhiteKing/8][posWhiteKing%8]=6;
		board[posBlackKing/8][posBlackKing%8]=-6;
		if (pieceType==0){board[posPiece/8][posPiece%8]=-5;}
		if (pieceType==1){board[posPiece/8][posPiece%8]=-2;}
		if (pieceType==2){board[posPiece/8][posPiece%8]=2;}
		if (pieceType==3){board[posPiece/8][posPiece%8]=5;}

		return;
	}
	cout << "we should not be here!\n";
	return;
}




int board2Hash3men(int board[8][8],int turn){
	int whiteKingCol,whiteKingRow,blackKingCol,blackKingRow,pieceCol,pieceRow,pieceType,hv;
	
	pieceType=666;
	for (int col=0; col<8;col++){
		for (int row=0; row<8;row++){
			if (board[col][row]!=0 && abs(board[col][row])!=6){pieceType=board[col][row];pieceCol=col;pieceRow=row;}
			if (board[col][row]==+6){whiteKingCol=col;whiteKingRow=row;}
			if (board[col][row]==-6){blackKingCol=col;blackKingRow=row;}
		}
	}
	
	if (pieceType==666 || abs(pieceType)==3 || abs(pieceType)==4){return 0;}

	// switching side to play if necessary
	if (turn<0){
		// switching colors:
		pieceType=-pieceType;
		
		hv=whiteKingCol;
		whiteKingCol=blackKingCol;
		blackKingCol=hv;
	
		hv=whiteKingRow;
		whiteKingRow=blackKingRow;
		blackKingRow=hv;
	
		// flipping horizontally
		whiteKingRow=7-whiteKingRow;
		blackKingRow=7-blackKingRow;
		pieceRow=7-pieceRow;
	}
	
	// move the white King to columns a-d if necessary
	if (whiteKingCol>3){
		whiteKingCol=7-whiteKingCol;
		blackKingCol=7-blackKingCol;
		pieceCol=7-pieceCol;
	}
	
	
	// piece==Pawn:
	if (abs(pieceType)==1){
		if (pieceType==-1){return 1+(whiteKingRow+8*whiteKingCol)+32*(blackKingRow+8*blackKingCol)+64*32*((pieceRow-1)+6*pieceCol)+64*32*48*0;}
		if (pieceType==+1){return 1+(whiteKingRow+8*whiteKingCol)+32*(blackKingRow+8*blackKingCol)+64*32*((pieceRow-1)+6*pieceCol)+64*32*48*1;}
	}
	
	
	// flip horizontally if necessary:
	if (whiteKingRow>3){
		whiteKingRow=7-whiteKingRow;
		blackKingRow=7-blackKingRow;
		pieceRow=7-pieceRow;
	}
	
	
	// flip along main diagonal if necessary:
	if (whiteKingRow>whiteKingCol){
		hv=whiteKingCol;
		whiteKingCol=whiteKingRow;
		whiteKingRow=hv;
		
		hv=blackKingCol;
		blackKingCol=blackKingRow;
		blackKingRow=hv;
		
		hv=pieceCol;
		pieceCol=pieceRow;
		pieceRow=hv;
	}
	
	int whiteKingSquare=whiteKingRow+1*(whiteKingCol>=1)+2*(whiteKingCol>=2)+3*(whiteKingCol>=3);
	
	
	if (pieceType==-5){return 196609+whiteKingSquare+10*(blackKingRow+8*blackKingCol)+10*64*(pieceRow+8*pieceCol)+10*64*64*0;}
	if (pieceType==-2){return 196609+whiteKingSquare+10*(blackKingRow+8*blackKingCol)+10*64*(pieceRow+8*pieceCol)+10*64*64*1;}
	if (pieceType==+2){return 196609+whiteKingSquare+10*(blackKingRow+8*blackKingCol)+10*64*(pieceRow+8*pieceCol)+10*64*64*2;}
	if (pieceType==+5){return 196609+whiteKingSquare+10*(blackKingRow+8*blackKingCol)+10*64*(pieceRow+8*pieceCol)+10*64*64*3;}
	
	cout << "onooooo!\n";
	return 0;
}



bool allOk(int board[8][8]){
	bool whiteKing=false;
	bool blackKing=false;
	int whiteKingCol, whiteKingRow, blackKingCol, blackKingRow;
	
	for (int row=0; row<8;row++){
		for (int col=0; col<8;col++){
			if (board[col][row]==6){whiteKing=true; whiteKingCol=col; whiteKingRow=row;}
			if (board[col][row]==-6){blackKing=true; blackKingCol=col; blackKingRow=row;}
		}
	}
	
	if (!whiteKing){return false;}
	if (!blackKing){return false;}
	
	if (abs(whiteKingCol-blackKingCol)<=1 && abs(whiteKingRow-blackKingRow)<=1){return false;} 
	
	return true;
}

int nrPieces(int board[8][8]){
	int found=0;
	for (int row=0; row<8;row++){
		for (int col=0; col<8;col++){
			if (board[col][row]!=0){found++;}
		}
	}
	return found;
}


int main(){
	
	/***************************
	**** initializing stuff ****
	***************************/	
	
	int tbSize=1+32*64*48*2+10*64*64*4;
	short int tb[tbSize]; // faszinierender weise hängt sich das programm auf, wenn ich "int" statt "short int" benutze...
	for (int i=0;i<tbSize;i++){tb[i]=0;}
	
	int board[8][8];	
	int winsFound=0;
	int tbSum=0;	
	int moveList[1000];
	int scores[200];
	int bestScore;
	
	
	
	/***********************************************
	**** finding +-1 evals someone in checkmate ****
	***********************************************/

	
	for (int hash=0;hash<tbSize;hash++){
		assignHash2board(board,hash);
		if (!allOk(board)){continue;}
		if (nrPieces(board)!=3 && hash!=0){continue;}
		
		assignMoveList(board,1,moveList); // not very sure about that line
		
		if (moveList[0]==0){ // there are no legal moves
			if (isKingInCheck(board,1,0,0)!=0){ // key not accurate probably
				tb[hash]=-1;
				plotBoard(board);
			}
		}
	}
	
	int hash,newHash;
	
	
	cout << "all mates found :)\n";
	
	winsFound=0;
	tbSum=0;
	for (int i=0;i<tbSize;i++){tbSum+=tb[i];}
	for (int i=0;i<tbSize;i++){if (tb[i]!=0){winsFound++;}}
	cout << "there are currently "<< winsFound<< " wins and tbSum " << tbSum <<"\n";	
	

	

	/****************
	**** testing ****
	****************/
	
	/*int aswwwsd;
	for (int hash=1;hash<tbSize;hash++){ // without hash=0
		
		assignHash2board(board,hash);
		if (nrPieces(board)!=3){continue;}
		if (!allOk(board)){continue;}
		if (board2Hash3men(board,1)==hash){
			cout << "yaay\n";
		} else {
			plotBoard(board);
			cout << hash << " vs "<< board2Hash3men(board,1)<< "\n";
			cin >> aswwwsd;
		}
	}*/
	
	/********************************************
	**** finding the other winning positions ****
	********************************************/
	
	
	
	for (int GREATLOOP=0;GREATLOOP<35;GREATLOOP++){
		for (int hash=0;hash<tbSize;hash++){	
			assignHash2board(board,hash);

			if (!allOk(board)){continue;}
			if (abs(tb[hash])==1){continue;} // leaf the leafes alone
			
			// generating the moveList and check for stalemate
			assignMoveList(board,1,moveList);
			if (isKingInCheck(board,-1,0,0)!=0){continue;} // if its white's turn, black can't be in check
			if (moveList[0]==0){continue;}
			
			
			
			
			// calculate all possible scores
			scores[0]=moveList[0];
			
			for (int i=1;i<=moveList[0];i++){
				assignMakeMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
				
				newHash=board2Hash3men(board,-1);
				scores[i]=-tb[newHash];
				assignUndoMove(board,moveList[5*i-4],moveList[5*i-3],moveList[5*i-2],moveList[5*i-1],moveList[5*i-0]);
			}
			// now, based on all the possible scores, we assign the new tb entry
			
			
			
			bestScore=-1; // the worst score is a mate in one
			
			// finding the optimal score:
			for (int i=1;i<=moveList[0];i++){
				if (scores[i]==0){
					if (bestScore<0){
						bestScore=0;
					}
				} else if (scores[i]>0){
					if (bestScore<=0 || bestScore>scores[i]){
						bestScore=scores[i]+1;
					}
				} else if (scores[i]<0){
					if (bestScore<0 && bestScore>=scores[i]){
						bestScore=scores[i]-1;
					}
				}
			}
			
			
			tb[hash]=bestScore;
			
		}
		
		winsFound=0;
		tbSum=0;
		for (int i=0;i<tbSize;i++){tbSum+=tb[i];}
		for (int i=0;i<tbSize;i++){if (abs(tb[i])>0){winsFound++;}}
		cout << "GREATLOOP: " << GREATLOOP<< "  ...  ";
		cout << "there are currently "<< winsFound<< " wins and tbSum " << tbSum <<"\n";	
		
		for (int hash=0;hash<tbSize;hash++){
			if (tb[hash]!=-1 && tb[hash]!=0){
				//cout <<tb[hash]<< "\n";
				//assignHash2board(board,hash);
				//plotBoard(board);
			}
		}
		
	}
	

	/*******************************
	**** having fun with the tb ****
	*******************************/
	
	for (int hash=0; hash<tbSize; hash=hash+10000){
		cout << hash<< "\n";
		assignHash2board(board,hash);
		
		plotBoard(board);
		cout << "Turn: white as always\n";
		cout << "the score is "<< tb[hash] << "\n\n\n";
	}

	for (int hash=0; hash<tbSize; hash++){
		assignHash2board(board,hash);
		if (abs(tb[hash])<55){continue;}
		plotBoard(board);
		
		cout << "the score is "<< tb[hash] << "\n\n\n";
	}

	/*************************
	**** exporting the db ****
	*************************/	
	
	
	ofstream MyFile("TABLEBASE_3men.h");
	
	MyFile << "/**************************\n";
	MyFile << "*** function depends on ***\n";
	MyFile << "**************************/\n";
	MyFile << "\n";
	MyFile << "//nothing\n";
	MyFile << "\n";
	MyFile << "/************\n";
	MyFile << "*** Guard ***\n";
	MyFile << "************/\n";
	MyFile << "\n";
	MyFile << "#ifndef TABKEBASE_3menFILE\n";
	MyFile << "#define TABKEBASE_3menFILE\n";
	MyFile << "\n";
	MyFile << "/***************\n";
	MyFile << "*** Let's go ***\n";
	MyFile << "***************/\n\n\n";
	MyFile << "// This is a 3men distance-to-mate (DTM) Tablebase\n";
	MyFile << "// White is always the side to move\n";
	MyFile << "// the order of the hashtables is KvK, KBvK, KNvK, KPvK, KRvK, KQvK\n";
	MyFile << "// General hash= positionWK + c1*positionBK + c2*positionPiece + c3*piecetype\n";
	MyFile << "// For KPvK, we restrict the white King to the first 4 columns. \n";
	MyFile << "// --> We number the squares: a1=0 a2=1, a3=2,..., b1=8, ... h1=56, h8=63\n";
	MyFile << "// For KRvK or KQvK, we restict the King to the A1,D1,D4 triangle.\n";
	MyFile << "// --> We number the squares: a1=0, b1=0, b2=2, c1=3, c2=4, c3=5 ,..., d4=9. (therefore same order as squares)\n";
	MyFile << "// The constants will be as small as possible\n";
	MyFile << "// The square indices will also be constants will be as small as possible\n";
	MyFile << "// The pieces indices will be numbered from the smallest piece to the highest\n";
	MyFile << "\n";
	MyFile << "// 1) [0,0] -> any KvK, KNvK or KBvK\n";
	MyFile << "// 2) [1,32*64*48*2] = [1,196608] any KPvK\n";
	MyFile << "// 3) 196608+[1, 10*64*64*4]=196608+[1, 163840]= [196609,360448]\n";
	MyFile << "// so in total 360449 positions\n\n";
	MyFile << "// the number 0 represents a draw\n";
	MyFile << "// the number +n represents mate in n-1 plys for the current player\n";
	MyFile << "// the number -n represents mate in n-1 plys for the opposite player\n";
	MyFile << "// so -1 means currently checkmate\n\n\n";

	MyFile << "// insert here the function 'board2Hash3men'\n\n\n";

	MyFile << "short int TABLEBASE_3men["<<tbSize<<"]={";
	for (int hash=0; hash<tbSize; hash++){
		if (hash%100000==0){cout << hash<<"\n";}
		MyFile << tb[hash];
		if (hash < tbSize -1){MyFile << ",";}	
	}
	MyFile << "};";
	
	MyFile << "\n\n\n#endif";

	
	
	MyFile.close();
	
	
	
	/**********************
	**** waiting @ end ****
	**********************/
	
	cout << "\nyou have reached the end\n";
	int asd;
	cin >> asd;
	return 1;
}