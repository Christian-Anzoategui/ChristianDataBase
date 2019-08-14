#ifndef FTOKENIZER_H
#define FTOKENIZER_H
#include <iostream>
#include <fstream>
#include "Constants.h"
#include "STokenizer.h"
#include "Token.h"

class FTokenizer
{

public:

    const int MAX_BLOCK = MAX_BUFFER;
    FTokenizer(char* fname);
    Token next_token();
    bool more();        //returns the current value of _more
//  int pos();          //returns the value of _pos
//  int block_pos();     //returns the value of _blockPos
    friend FTokenizer& operator >> (FTokenizer& f, Token& t)
	{
		t = f.next_token();
		return f;
	}

private:

    bool get_new_block(); //gets the new block from the file
    std::ifstream _f;   //file being tokenized
    STokenizer _stk; //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};


FTokenizer::FTokenizer(char* fname)
{
	_f.open(fname);
if(!_f){cerr<<"Error opening:"<<fname<<"\n"; return;}
//	_blockPos = 0;

	_more = get_new_block();

}

bool FTokenizer::get_new_block()
{
//check if _f is empty;	
	if(_f)
	{
//cout<<"---------------------New Block----------------------"<<endl;
	char buffer[MAX_BLOCK];
	_f.read(buffer, MAX_BLOCK-1);

	buffer[_f.gcount()] = '\0';
	
	_stk.set_string(buffer);

if(debug)cout << "FT|get_new_block: _f.gcount = "
              << _f.gcount() << endl;

		if(_f.gcount() > 0)	
		return true;
		else
		return false;

	}
	else {

if(debug)cout << "FT|get_new_block: _f is empty" << endl;
	return false;
	}

}

bool FTokenizer::more()
{
	return _more;
}


Token FTokenizer::next_token()
{
	Token t;
	if(_stk.done())
	{
		_more = get_new_block();
		if (_more == false)
			return t;
	}

	_stk >> t;

	return t;	

}

#endif
