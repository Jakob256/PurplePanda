/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef assignFen2BoardFILE
#define assignFen2BoardFILE

/***************
*** Let's go ***
***************/


void assignFen2Board (string fen ,int board[8][8]){
	for (int i=0; i<8; i++){
		for (int j=0; j<8; j++){
			board[i][j]=0;
		}
	}
	
	
	int pointerRow=7;
	int pointerCol=0;
	
	for (int i=0; i<80; i++){
		if (fen.at(i)=='P'){board[pointerCol][pointerRow]= 1;pointerCol++;continue;}
		if (fen.at(i)=='R'){board[pointerCol][pointerRow]= 2;pointerCol++;continue;}
		if (fen.at(i)=='N'){board[pointerCol][pointerRow]= 3;pointerCol++;continue;}
		if (fen.at(i)=='B'){board[pointerCol][pointerRow]= 4;pointerCol++;continue;}
		if (fen.at(i)=='Q'){board[pointerCol][pointerRow]= 5;pointerCol++;continue;}
		if (fen.at(i)=='K'){board[pointerCol][pointerRow]= 6;pointerCol++;continue;}
		if (fen.at(i)=='p'){board[pointerCol][pointerRow]=-1;pointerCol++;continue;}
		if (fen.at(i)=='r'){board[pointerCol][pointerRow]=-2;pointerCol++;continue;}
		if (fen.at(i)=='n'){board[pointerCol][pointerRow]=-3;pointerCol++;continue;}
		if (fen.at(i)=='b'){board[pointerCol][pointerRow]=-4;pointerCol++;continue;}
		if (fen.at(i)=='q'){board[pointerCol][pointerRow]=-5;pointerCol++;continue;}
		if (fen.at(i)=='k'){board[pointerCol][pointerRow]=-6;pointerCol++;continue;}
		
		if (fen.at(i)=='1'){pointerCol+=1;continue;}
		if (fen.at(i)=='2'){pointerCol+=2;continue;}
		if (fen.at(i)=='3'){pointerCol+=3;continue;}
		if (fen.at(i)=='4'){pointerCol+=4;continue;}
		if (fen.at(i)=='5'){pointerCol+=5;continue;}
		if (fen.at(i)=='6'){pointerCol+=6;continue;}
		if (fen.at(i)=='7'){pointerCol+=7;continue;}
		if (fen.at(i)=='8'){pointerCol+=8;continue;}

		if (fen.at(i)=='/'){pointerCol=0;pointerRow--;continue;}
				
		if (fen.at(i)==' '){return;}
	}
	return;
}

#endif