#ifndef CONSTANTS_H
#define CONSTANTS_H
//typedef unsigned short int us_int;


const int MAX_BUFFER = 1000;
const int MAX_ROWS = 40;
const int MAX_COLUMNS = 256;
const int ALPHA_STATE_START = 4;
const int NUM_STATE_START = 0;
const int SPACE_STATE_START= 7;
const int UNKNOWN_STATE_START= MAX_ROWS-2;
const int PUN_STATE_START= 8;

const char PUNCTUATION[] = ".!,?\'-;:";

//dbase
const int START= 0;
//any string, could represent fname, value, etc
const int ANY_STRING = 38; 

//select
const int SELECT= 1;
const int ASTERISK= 4;
const int QUOTE= 5;
const int COMMA= 6;
const int AND_OR= 7;
const int FROM= 8;
const int WHERE= 9;
const int OP= 10;

//load
const int LOAD = 21; 
//print
const int PRINT = 21; 
//exit
const int EXIT = 37; 

//create
const int CREATE = 25;
const int C_TABLE= 36;
const int C_COMMA= 35;
const int C_VALUE= 28;

//insert
const int INSERT   = 29;
const int INTO     = 30;
const int VALUES   = 32;
const int I_VALUE  = 32;
const int I_COMMA  = 34;

//success
const int TABLE_NAME = 4;
const int END_QUOTE = 10;

#endif 
