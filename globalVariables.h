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
int globalBestMoveCompletedMove[5];
float INF=10000000.*10000000000.*1000000000.*10000000000.;
int globalMaxDepth;

/***********************
** for counting calls **
***********************/

int countingStationaryEvalCalled;
int countingEvalCalled;
int countingMoveGenerationCalled;

/*****************************
** for avoiding repetitions **
*****************************/

int previous100PositionHashes[100]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

bool oracleStrongLegalMoves;
int moveOpponentToEdge;
long long int pieceSquareTable[6+1+6][8][8]={0};  // long long necessary to add it to newKeyy


#endif