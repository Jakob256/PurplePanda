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

//UCI stuff:

bool UCI_OPTION_OPENINGBOOK=false;
bool openingBookRandomnessGenerated=false;
short int openingBookRandomNumbers[2346];

/*******************************
*** general global variables ***
*******************************/


const int INF=100000000;
bool exitSearch;
int endTime,NODELIMIT;

long long int PST_mg[6+1+6][8][8]={0};      // long long necessary to add it to newKeyy
long long int PST_eg[6+1+6][8][8]={0};      // long long necessary to add it to newKeyy
int PST_mg_vector[832],PST_eg_vector[832];  // currently only for moveordering


/******************
*** about moves ***
******************/

unsigned int rootBestMove;
unsigned int killerMoves[100];
int historyA[4096]; // how often a quiet move was tried
int historyB[4096]; // how often it was successful
int historyC[4096]; // how often it was tried since last pruning
int moveHistoryScore[8192];


/********************
** counting things **
********************/

int nodes,staticeval,movegen,hashstored,hashhits;

/*****************************
** for avoiding repetitions **
*****************************/

long long int previous100PositionHashes[100];
long long int branch_hashes[123];

/**********************
** hashes in general **
**********************/

// I want abs(hash)<2^63, so each number should be less than 2^63/6/(64+1)=2*10^16 to
// avaiod overflows, even though it might not even be a problem
const long long int hashSquareNumbers[8][8]={
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
	

const long long int hashTurnNumber=217122680661300;

/***************
** Hashtables **
***************/


const int ht_size=12700000;                // 8+2=10 Byte per entry
long long int ht_hash[12700000];           // the exact hash
unsigned short int ht_moveShort[12700000]; // the best move in that position (only the first 16 bits)



#endif