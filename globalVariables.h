/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef globalVariablesFILE
#define globalVariablesFILE

/***************
*** Let's go ***
***************/

int globalBestMove[5];
float INF=10000000.*10000000000.*1000000000.*10000000000.;
int globalMaxDepth;
int randomness=1;


/**************************
*** for move generation ***
**************************/

// there are 4 directions: N,E, NW, NW in that order. Here in the list are the pieces marked that can go that way (i.e. rook, rook, bishop, bishop)
const int moveGenerationHelper[4]={2,2,4,4};

// directions in that order N,S,E,W,NW,SE,NE,SW,8 knight directions
const int dx_LIST[16]={ 0, 0, 1,-1,-1, 1, 1,-1,-2,-2,-1,-1,+1,+1,+2,+2};
const int dy_LIST[16]={ 1,-1, 0, 0, 1,-1, 1,-1,-1,+1,-2,+2,-2,+2,-1,+1};

/***********************
** for counting calls **
***********************/

int countingStationaryEvalCalled;
int countingEvalCalled;
int countingMoveGenerationCalled;
int countingHashesStored;

/*****************************
** for avoiding repetitions **
*****************************/

long long int previous100PositionHashes[100]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/******************************
** for preSearchCalculations **
******************************/

int globalNrWhitePieces=0;
int globalNrBlackPieces=0;
int globalNrWhitePawns=0;
int globalNrBlackPawns=0;
int globalNrWhiteRooks=0;
int globalNrBlackRooks=0;
int globalNrWhiteKnights=0;
int globalNrBlackKnights=0;
int globalNrWhiteBishops=0;
int globalNrBlackBishops=0;
int globalNrWhiteQueens=0;
int globalNrBlackQueens=0;

int globalPieceScoreWhite=0;
int globalPieceScoreBlack=0;
int globalPiecesOnHomeSquareWhite=0;
int globalPiecesOnHomeSquareBlack=0;

int globalSide2Play=0;


/***************
** for oracle **
***************/

int moveOpponentToEdge;
long long int pieceSquareTable[6+1+6][8][8]={0};  // long long necessary to add it to newKeyy


/**********************
** hashes in general **
**********************/

// I want abs(hash)<2^63, so each number should be less than 2^63/6/(64+1)=2*10^16 to
// avaiod overflows, even though it might not even be a problem
long long int hashSquareNumbers[8][8]={
	314159265358979,//a1
	323846264338327,//a2
	950288419716939,//a3
	937510582097494,//a4
	459230781640628,//a5
	620899862803482,//a6
	534211706798214,//a7
	808651328230664,//a8
	709384460955058,//b1
	223172535940812,//b2
	848111745028410,//b3
	270193852110555,//b4
	964462294895493,//b5
	 38196442881097,//b6
	566593344612847,//b7
	564823378678316,//b8
	527120190914564,//c1
	856692346034861,//c2
	 45432664821339,//c3
	360726024914127,//c4
	372458700660631,//c5
	558817488152092,//c6
	 96282925409171,//c7
	536436789259036,//c8
	  1133053054882,//d1
	 46652138414695,//d2
	194151160943305,//d3
	727036575959195,//d4
	309218611738193,//d5
	261179310511854,//d6
	807446237996274,//d7
	956735188575272,//d8
	489122793818301,//e1
	194912983367336,//e2
	244065664308602,//e3
	139494639522473,//e4
	719070217986094,//e5
	370277053921717,//e6
	629317675238467,//e7
	481846766940513,//e8
	200056812714526,//f1
	356082778577134,//f2
	275778960917363,//f3
	717872146844090,//f4
	122495343014654,//f5
	958537105079227,//f6
	968925892354201,//f7
	995611212902196,//f8
	 86403441815981,//g1
	362977477130996,//g2
	 51870721134999,//g3
	999837297804995,//g4
	105973173281609,//g5
	631859502445945,//g6
	534690830264252,//g7
	230825334468503,//g8
	526193118817101,//h1
	   313783875288,//h2
	658753320838142,//h3
	 61717766914730,//h4
	359825349042875,//h5
	546873115956286,//h6
	388235378759375,//h7
	195778185778053};//h8
	

long long int hashTurnNumber=217122680661300;

/***************************
** for hash move ordering **
***************************/


int hashMoveOrderingTableSize=100*1000*1000;
long int hashMoveOrderingTable[100*1000*1000][2]={0};  // must be the same number as above
// first column to store the exact hash, to detect hash collisions
// second column to store the id of a move:
// ID: move[0]+move[1]*20 + move[2]*20^2+ ... +move[5]*20^4



#endif