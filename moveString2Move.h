/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef moveString2MoveFILE
#define moveString2MoveFILE

/***************
*** Let's go ***
***************/


unsigned int moveString2Move (int board[8][8],std::string moveString){
	unsigned int move;
	int c1,c2,r1,r2,startPiece,endPiece;
	
	if (moveString.substr(0,1)=="a"){c1=0;}
	if (moveString.substr(0,1)=="b"){c1=1;}
	if (moveString.substr(0,1)=="c"){c1=2;}
	if (moveString.substr(0,1)=="d"){c1=3;}
	if (moveString.substr(0,1)=="e"){c1=4;}
	if (moveString.substr(0,1)=="f"){c1=5;}
	if (moveString.substr(0,1)=="g"){c1=6;}
	if (moveString.substr(0,1)=="h"){c1=7;}
	
	if (moveString.substr(1,1)=="1"){r1=0;}
	if (moveString.substr(1,1)=="2"){r1=1;}
	if (moveString.substr(1,1)=="3"){r1=2;}
	if (moveString.substr(1,1)=="4"){r1=3;}
	if (moveString.substr(1,1)=="5"){r1=4;}
	if (moveString.substr(1,1)=="6"){r1=5;}
	if (moveString.substr(1,1)=="7"){r1=6;}
	if (moveString.substr(1,1)=="8"){r1=7;}
		
	if (moveString.substr(2,1)=="a"){c2=0;}
	if (moveString.substr(2,1)=="b"){c2=1;}
	if (moveString.substr(2,1)=="c"){c2=2;}
	if (moveString.substr(2,1)=="d"){c2=3;}
	if (moveString.substr(2,1)=="e"){c2=4;}
	if (moveString.substr(2,1)=="f"){c2=5;}
	if (moveString.substr(2,1)=="g"){c2=6;}
	if (moveString.substr(2,1)=="h"){c2=7;}
	
	if (moveString.substr(3,1)=="1"){r2=0;}
	if (moveString.substr(3,1)=="2"){r2=1;}
	if (moveString.substr(3,1)=="3"){r2=2;}
	if (moveString.substr(3,1)=="4"){r2=3;}
	if (moveString.substr(3,1)=="5"){r2=4;}
	if (moveString.substr(3,1)=="6"){r2=5;}
	if (moveString.substr(3,1)=="7"){r2=6;}
	if (moveString.substr(3,1)=="8"){r2=7;}
	
	startPiece=board[c1][r1];
	endPiece=board[c2][r2];		
	
	
	move=c1+8*r1+64*c2+512*r2+32768*abs(endPiece);	
	
	if (startPiece==1 && r1==6){ // this was a promotion
		if (moveString.substr(4,1)=="r"){move+=4096*2;}
		if (moveString.substr(4,1)=="n"){move+=4096*3;}
		if (moveString.substr(4,1)=="b"){move+=4096*4;}
		if (moveString.substr(4,1)=="q"){move+=4096*5;}
	}		
	if (startPiece==-1 && r1==1){ // this was a promotion
		if (moveString.substr(4,1)=="r"){move+=4096*2;}
		if (moveString.substr(4,1)=="n"){move+=4096*3;}
		if (moveString.substr(4,1)=="b"){move+=4096*4;}
		if (moveString.substr(4,1)=="q"){move+=4096*5;}
	}

	if (abs(startPiece)==1 && c1!=c2 && endPiece==0){ // has to be an en passant move
		move+=262144;
	}
	
	
	return move;
}

#endif