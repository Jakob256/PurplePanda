/**************************
*** function depends on ***
**************************/

// nothing

/************
*** Guard ***
************/

#ifndef fen2keyFILE
#define fen2keyFILE

/***************
*** Let's go ***
***************/


unsigned long long int fen2key (string fen){
	unsigned long long int key=0;
	unsigned long long int score=0,piecesOnBoard=0;
	bool firstWhiteSpaceFound=false;
	bool secondWhiteSpaceFound=false;
	
	for (int i=0; i<fen.length(); i++){
		if (fen.at(i)=='m'){break;} // this is the "m" from "moves"
		
		if (fen.at(i)=='p' && !firstWhiteSpaceFound){score-=1;piecesOnBoard++;}
		if (fen.at(i)=='n' && !firstWhiteSpaceFound){score-=3;piecesOnBoard++;}
		if (fen.at(i)=='b' && !firstWhiteSpaceFound){score-=3;piecesOnBoard++;}
		if (fen.at(i)=='r' && !firstWhiteSpaceFound){score-=5;piecesOnBoard++;}
		if (fen.at(i)=='q' && !firstWhiteSpaceFound){score-=9;piecesOnBoard++;}
		if (fen.at(i)=='k' && !firstWhiteSpaceFound){score-=20;piecesOnBoard++;}
		if (fen.at(i)=='P' && !firstWhiteSpaceFound){score+=1;piecesOnBoard++;}
		if (fen.at(i)=='N' && !firstWhiteSpaceFound){score+=3;piecesOnBoard++;}
		if (fen.at(i)=='B' && !firstWhiteSpaceFound){score+=3;piecesOnBoard++;}
		if (fen.at(i)=='R' && !firstWhiteSpaceFound){score+=5;piecesOnBoard++;}
		if (fen.at(i)=='Q' && !firstWhiteSpaceFound){score+=9;piecesOnBoard++;}
		if (fen.at(i)=='K' && !firstWhiteSpaceFound){score+=20;piecesOnBoard++;}
		
		if (fen.at(i)==' ' && firstWhiteSpaceFound){secondWhiteSpaceFound=true;}
		if (fen.at(i)==' '){firstWhiteSpaceFound=true;}
		
		if (fen.at(i)=='w' && firstWhiteSpaceFound){key++;} // set turn bit
		
		if (fen.at(i)=='K' && secondWhiteSpaceFound){key+=2;} // set casteling bits
		if (fen.at(i)=='Q' && secondWhiteSpaceFound){key+=4;}
		if (fen.at(i)=='k' && secondWhiteSpaceFound){key+=8;}
		if (fen.at(i)=='q' && secondWhiteSpaceFound){key+=16;}

		if (fen.at(i)=='a' && secondWhiteSpaceFound){key+=32;key+=64*0;} // set en pessant bits
		if (fen.at(i)=='b' && secondWhiteSpaceFound){key+=32;key+=64*1;}
		if (fen.at(i)=='c' && secondWhiteSpaceFound){key+=32;key+=64*2;}
		if (fen.at(i)=='d' && secondWhiteSpaceFound){key+=32;key+=64*3;}
		if (fen.at(i)=='e' && secondWhiteSpaceFound){key+=32;key+=64*4;}
		if (fen.at(i)=='f' && secondWhiteSpaceFound){key+=32;key+=64*5;}
		if (fen.at(i)=='g' && secondWhiteSpaceFound){key+=32;key+=64*6;}
		if (fen.at(i)=='h' && secondWhiteSpaceFound){key+=32;key+=64*7;}
	}
	
	key+=piecesOnBoard*512; // add pieces on board
	
	key+=(128+score)*32768;
	
	return key;
}

#endif