/***********************
*** loading packages ***
***********************/

#include <iostream>     // for input/output
#include <bitset>       // for printing the binary representation of the "key"

#include <string>       // for fen2Board
#include <stdlib.h>     // for random openings
#include <time.h>       // clock_t, clock, CLOCKS_PER_SEC,time
#include <math.h>       // for abs for some reason

using namespace std;

/**************************
*** function depends on ***
**************************/

#include "globalVariables.h"
#include "printingStuff.h"
#include "assignMoveListAndSort.h"
#include "newKey.h"
#include "assignFen2Board.h"
#include "moveString2Move.h"
#include "perft.h"
#include "fen2key.h"
#include "positionFeatures.h"
#include "oracle.h"
#include "setBonusOfKey.h"
#include "engine.h"
#include "stationaryEval.h"


/***************
*** Let's go ***
***************/



int main() {
	srand(time(NULL)); // initialize random seed
	
	int resetBoard[8][8],board[8][8];
	assignFen2Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq",board);
	assignFen2Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq",resetBoard);
	
	unsigned long long int resetKey=0b1000000000000100000000000001100010000000100000000011111;
	unsigned long long int key=     0b1000000000000100000000000001100010000000100000000011111;
	
	for (int i=0;i<100;i++){previous100PositionHashes[i]=i;}
	
	oracle(board,key);
	
	int maxMillisecondsTime2Think=30000;   //  <<<---- this parameter could be changed!!!
	
	
	int millisecondsTime2Think,perftDepth,hv,depth;
	bool movesFound=false; //for fen ... moves ...
	std::string input, fenString, moveString,word1,word2,word3,word4,word5,word6,word7,word8,word9;
	int lastWhitespace,wtime,btime,winc,binc;
	unsigned int move;
	
	
	while (true) {

		std::getline(std::cin, input);

		if (input=="uci"){
			cout << "id name Purple Panda 20\nid author J. Steininger\n\noption name OpeningBook type check default false\nuciok\n";
			
		} else if (input=="setoption name OpeningBook value true"){
			UCI_OPTION_OPENINGBOOK=true;
		
		} else if (input=="setoption name OpeningBook value false"){
			UCI_OPTION_OPENINGBOOK=false;
		
		} else if (input=="d"){
			plotBoard(board);
			cout << "fen: ";
			printFen(board,key);
			cout << "\n";
			printKey(key);

			oracle(board,key);
			key=setBonusOfKey(board,key);
			cout << "Stationary eval: "<< stationaryEval(board,key)<< " cp\n";
			
		} else if (input=="clear"){
			for (int i=0; i<ht_size;i++){
				ht_hash[i]=0;
				ht_moveShort[i]=0;
			}
			cout << "hashtable cleared\n";
			
		} else if (input=="hashmove"){
			long long int hash=hashFunction(board,key);
			int hash4moveOrder=int(abs(hash%ht_size));
			long long int hashOfTable=ht_hash[hash4moveOrder];
			int moveShort=0;
			
			if (hashOfTable==hash){// we have the exact same hash stored!
				moveShort=ht_moveShort[hash4moveOrder];	
			}
			
			unsigned int moveList[250];
			assignMoveListAndSort(board,key,moveList,true,moveShort);
			if (moveList[1]%65536==moveShort){
				cout << "hashmove: "<<move2string(moveList[1])<<"\n";
			} else {
				cout << "no hashmove found\n";
			}
		
		} else if (input=="pst"){
			oracle(board,key);
			int phase24=int((key>>23)&63);
			for (int row=7; row>=0; row--){
				for (int col=0; col<8; col++){
					int piece=board[col][row];
					hv=(PST_mg[piece+6][col][row]*phase24+PST_eg[piece+6][col][row]*(24-phase24))/24;
					if      (hv<=-100)          {cout << hv << " ";}
					else if (hv<=-10 || hv>=100){cout << " "<< hv << " ";}
					else if (hv<0 || hv >=10)   {cout << "  " << hv << " ";}
					else                        {cout << "   " << hv << " ";}
				}
				cout <<"\n";
			}
			cout << "\nStationary Eval: "<< stationaryEval(board,key)<<"\n\n";
			
		} else if (input=="moveorder"){
			unsigned int moveList[250];
			assignMoveListAndSort(board,key,moveList,true);
			printMoveList(moveList);
			
		} else if (input=="uciready" || input=="isready"){
			cout << "readyok\n";
			
		} else if (input=="quit" || input=="exit" || input=="q"){
			break;	
			
		} else if (input.substr(0,17)=="position startpos"){
			for (int i=0;i<100;i++){previous100PositionHashes[i]=i;}  // clear previous positions
			
			for (int i=0;i<8;i++){
				for (int j=0;j<8;j++){
					board[i][j]=resetBoard[i][j];
				}
			}
			key=resetKey;
			oracle(board,key);
			
			// now check if there are extra moves given
			if (input.substr(0,23)=="position startpos moves"){
				
				input.append(1,' ');
		
				for (int i=23; i<=input.length(); i++){
					if (input.substr(i,1)==" "){
						for (int j=i+1; j<=input.length(); j++){
							if (input.substr(j,1)==" "){
							
								moveString= input.substr(i+1,j-i);
								move=moveString2Move(board,moveString);
								
								key=newKey(board,key,move);
								assignMakeMove(board,move);
								
								for (int k=98;k>=0;k--){previous100PositionHashes[k+1]=previous100PositionHashes[k];}  // move stack of previous 100 position
								previous100PositionHashes[0]=hashFunction(board,key); // save new Hash position
								
								break;
								
							}
						}
					}
				}
			
				oracle(board,key);
				key=setBonusOfKey(board,key);
			}
			
		} else if (input.substr(0,12)=="position fen"){
			for (int i=0;i<100;i++){previous100PositionHashes[i]=i;}  // clear previous positions
			
			fenString=input.substr(13,input.length()-13);
			assignFen2Board(fenString,board);
			key=fen2key(fenString);
			
			oracle(board,key);
			key=setBonusOfKey(board,key);
			
			movesFound=false;
			for (int i=1; i<input.length()-5; i++){
				if (input.substr(i,5)=="moves"){
					movesFound=true;
					hv=i+4;
				}
			}
			

			if (movesFound==true){
				input.append(1,' ');
				for (int i=hv; i<=input.length(); i++){
					if (input.substr(i,1)==" "){
						for (int j=i+1; j<=input.length(); j++){
							if (input.substr(j,1)==" "){
								
								moveString= input.substr(i+1,j-i);
								move=moveString2Move(board,moveString);
								
								key=newKey(board,key,move);
								assignMakeMove(board,move);
								
								for (int k=98;k>=0;k--){previous100PositionHashes[k+1]=previous100PositionHashes[k];}  // move stack of previous 100 position
								previous100PositionHashes[0]=hashFunction(board,key); // save new Hash position
								
								break;
							}
						}
					}
				}	
			}
			

			
		} else if (input.substr(0,8)=="go perft"){
			perftDepth=stoi(input.substr(9,input.length()-9));
			perft(board,key,perftDepth);
			
		/***********************************
		*** now starting the calculation ***
		***********************************/
		
		} else if (input=="go depth 0" || input=="go movetime 0"){
			unsigned int moveList[250];
			assignMoveListAndSort(board,key,moveList,true,0);
			cout << "bestmove "<<move2string(moveList[1])<< "\n";
		
		} else if (input=="go movetime 10000"){ 	// so, the first comand on Lichess is movetime 10000 when your time is not yet ticking
													// however, I don't want to keep the player waiting for 10 seconds. So I will reduce the
													// thinking time to 0.5 sek, if it seems that we are communicating with lichess in the starting position.
			int nrUntouchedSquares=0;
			for (int col=0;col<8;col++){
				for (int row=0;row<8;row++){
					if (resetBoard[col][row]==board[col][row]){
						nrUntouchedSquares++;
					}
				}
			}
			if (nrUntouchedSquares>=64-2){
				engine(board, key, 500,1000,-1);
			} else {
				engine(board, key, 10000,1000,-1);
			}
			
		} else if (input.substr(0,11)=="go movetime"){			// this is for constant movetime
			millisecondsTime2Think=stoi(input.substr(12,input.length()-12));
			// okay... as far as I can tell, Arena does not have the option to set a constant movetime that is not a multiple of one second.
			// therefore, the millisecond time is always divisible by 1000.
			// however, I want to allow movetimes less than a second.
			// so my hack is, if the movetime in milliseconds is of the form 789xxx000, then we only use the xxx milliseconds.
			
			if (millisecondsTime2Think-millisecondsTime2Think%1000000==789000000 && millisecondsTime2Think%1000==0){
				millisecondsTime2Think=(millisecondsTime2Think%1000000)/1000;
			}
			
			engine(board, key, millisecondsTime2Think,1000,-1);
			
		} else if (input.substr(0,8)=="go depth"){
			depth=stoi(input.substr(9,input.length()-9));
			engine(board, key, 1000000000,depth,-1);			
		
		} else if (input.substr(0,8)=="go nodes"){
			hv=stoi(input.substr(9,input.length()-9));
			engine(board, key, 1000000000,1000,hv);
		
		} else if (input=="go infinite"){ 						// used for analysis: as there is no interrupt, analysis is bounded to 10 seconds
			engine(board, key, 10000,1000,-1);
			
		} else if (input.substr(0,2)=="go"){					// this is for normal gameplay, where there might be additional increment
			// expected input like: go wtime 120703 btime 120464 winc 1000 binc 1000
			// 					or: go wtime 120703 btime 120464
			// 					or: go   (for debugging stuff)
			
			input.append(1,' '); // append ws to the end
			
			word1.assign("empty");
			word2.assign("empty");
			word3.assign("empty");
			word4.assign("empty");
			word5.assign("empty");
			word6.assign("empty");
			word7.assign("empty");
			word8.assign("empty");
			word9.assign("empty");
			
			lastWhitespace=-1;
			wtime=0;btime=0;winc=0;binc=0;
			
			for (int i=0; i<input.length(); i++){
				if (input.at(i)==' '){
					if (word9=="empty" && word8!="empty"){word9.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word8=="empty" && word7!="empty"){word8.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word7=="empty" && word6!="empty"){word7.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word6=="empty" && word5!="empty"){word6.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word5=="empty" && word4!="empty"){word5.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word4=="empty" && word3!="empty"){word4.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word3=="empty" && word2!="empty"){word3.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word2=="empty" && word1!="empty"){word2.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));}
					if (word1=="empty")                  {word1.assign(input.substr(lastWhitespace+1,i-lastWhitespace-1));} 
					lastWhitespace=i;
				}
			}
			
			if (word2=="wtime"){wtime=stoi(word3);}
			if (word4=="wtime"){wtime=stoi(word5);}
			if (word6=="wtime"){wtime=stoi(word7);}
			if (word8=="wtime"){wtime=stoi(word9);}
			
			if (word2=="btime"){btime=stoi(word3);}
			if (word4=="btime"){btime=stoi(word5);}
			if (word6=="btime"){btime=stoi(word7);}
			if (word8=="btime"){btime=stoi(word9);}

			if (word2=="winc"){winc=stoi(word3);}
			if (word4=="winc"){winc=stoi(word5);}
			if (word6=="winc"){winc=stoi(word7);}
			if (word8=="winc"){winc=stoi(word9);}
			
			if (word2=="binc"){binc=stoi(word3);}
			if (word4=="binc"){binc=stoi(word5);}
			if (word6=="binc"){binc=stoi(word7);}
			if (word8=="binc"){binc=stoi(word9);}

			if (word2=="empty"){ // only "go" found
				engine(board, key, 1000,1000,-1); // thinks for 1 second
				
			} else { // there is at least wtime and btime
				if (key%2==1){ // white's turn					
					millisecondsTime2Think=winc+wtime/40;
					if (millisecondsTime2Think>wtime*95/100){ // getting increment only after the move
						millisecondsTime2Think=wtime*95/100;
					}
				} else {
					millisecondsTime2Think=binc+btime/40;
					if (millisecondsTime2Think>btime*95/100){ // getting increment only after the move
						millisecondsTime2Think=btime*95/100;
					}
				}
				
				if (millisecondsTime2Think>maxMillisecondsTime2Think){millisecondsTime2Think=maxMillisecondsTime2Think;}
				engine(board, key, millisecondsTime2Think,1000,-1);
			}
			
		
		}

	}
	return 0;
}

