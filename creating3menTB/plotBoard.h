/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef plotBoardFILE
#define plotBoardFILE

/***************
*** Let's go ***
***************/


void printInt2Letter (int i){
	switch ( i ){
		case 1:
			cout <<"P";
			break;
		case 2:
			cout <<"R";
			break;
		case 3:
			cout <<"N";
			break;
		case 4:
			cout <<"B";
			break;
		case 5:
			cout <<"Q";
			break;
		case 6:
			cout <<"K";
			break;
		case -1:
			cout <<"p";
			break;
		case -2:
			cout <<"r";
			break;
		case -3:
			cout <<"n";
			break;
		case -4:
			cout <<"b";
			break;
		case -5:
			cout <<"q";
			break;
		case -6:
			cout <<"k";
			break;
	 default:
		cout << " ";
    }
}


void plotBoard(int board[8][8]) {
	for (int row = 7; row >= 0; row--) {
		cout << "+---+---+---+---+---+---+---+---+\n|";
		for (int column = 0; column < 8; column++) {
			cout << " ";
			printInt2Letter(board[column][row]);
			cout <<" |";
		}
		cout <<"\n";
	}
	cout << "+---+---+---+---+---+---+---+---+\n\n";
}

#endif