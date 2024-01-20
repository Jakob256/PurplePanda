/**************************
*** function depends on ***
**************************/

//nothing

/************
*** Guard ***
************/

#ifndef moveIDFILE
#define moveIDFILE

/***************
*** Let's go ***
***************/

short int moveID(int c1, int r1, int c2, int r2, int adInfo){ // 2 Byte
	return c1+8*r1+64*r2+512*c2+4096*(adInfo%16+adInfo/16+(adInfo>16)*5)-30000;
}

#endif