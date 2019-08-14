#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <iostream>
#include <string.h>
#include "Constants.h" 
#include "state_machine.h"
#include "Token.h" 

using namespace std;

class STokenizer
{

public:
    STokenizer();
    STokenizer(const char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens

    //---------------
    //extract one token (very similar to the way cin >> works)

friend STokenizer& operator >> (STokenizer& s, Token& t)
{
//start state goes to the begging of state machine
//which is 0
		int start_state = NUM_STATE_START;
		string temp_str = "";

	while(s.get_token(start_state, temp_str) == false)
	{


	if(start_state  < ALPHA_STATE_START && 
		_table[ALPHA_STATE_START][s._buffer[s._pos]] != -1)
	{
		start_state = ALPHA_STATE_START;
	}	

	else if(start_state  < SPACE_STATE_START && 
		_table[SPACE_STATE_START][s._buffer[s._pos]] != -1)
	{
		start_state = SPACE_STATE_START;
	}	

	else if(start_state  < PUN_STATE_START && 
		_table[PUN_STATE_START][s._buffer[s._pos]] != -1)
	{
		start_state = PUN_STATE_START;
	}	

	else
		start_state = UNKNOWN_STATE_START;

	}

		t = Token(temp_str, start_state);

		return s;

}

    //set a new string as the input string
    void set_string(const char str[]);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                    //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};



int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];


STokenizer::STokenizer()
{

	make_table(_table);
	_pos = 0;
}

STokenizer::STokenizer(const char str[])
{
	make_table(_table);
	_pos = 0;

	set_string(str);
}

void STokenizer::set_string(const char str[])
{
	_pos = 0;
	strcpy(_buffer, str);

}


void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
	init_table(_table);

//num
	mark_success(_table, 1);		
	mark_success(_table, 3);		
	mark_fail(_table, 0);
	mark_fail(_table, 2);
//alpha
	mark_success(_table, 4);
	mark_success(_table, 5);

//space
	mark_success(_table, 7);

//pun
//	mark_fail(_table, PUN_STATE_START);
	mark_success(_table, PUN_STATE_START);

//unknown
	mark_success(_table, UNKNOWN_STATE_START);


//0-3 NUMERIC state machine handles double
//state [0] --- DIGITS ---> [1]
mark_cells(NUM_STATE_START, _table, '0', '9', 1);    
//state [0] --- '.' ------> [2]
mark_cells(NUM_STATE_START, _table, '.', '.', 2);    
mark_cells(1, _table, '0', '9', 1);    //state [1] --- DIGITS ---> [1]
mark_cells(1, _table, '.', '.', 2);    //state [1] --- '.' ------> [2]
mark_cells(2, _table, '0', '9', 3);    //state [2] --- DIGITS ---> [3]
mark_cells(3, _table, '0', '9', 3);    //state [3] --- DIGITS ---> [3]

//4 ALPHA
mark_cells(ALPHA_STATE_START, _table, 'a', 'z', 5);  
mark_cells(ALPHA_STATE_START, _table, 'A', 'Z', 5);  

//5 ALPHA
mark_cells(ALPHA_STATE_START+1, _table, '0', '9', 5);  
mark_cells(ALPHA_STATE_START+1, _table, 'a', 'z', 5);  
mark_cells(ALPHA_STATE_START+1, _table, 'A', 'Z', 5);  

//7 SPACE
mark_cells(SPACE_STATE_START, _table, ' ', ' ', 7);  

//8 PUN (S STATE)
mark_cells(PUN_STATE_START, _table, PUNCTUATION, PUN_STATE_START);  

 
}

bool STokenizer::get_token(int start_state, string& token)
{
bool debug = false;

	string temp(_buffer);
	int index = _pos;
	int last_success = 0;

if(debug){
	 cout<<endl;
	 cout<<"ST|get_token BEG:index = "<<index<<endl; 
	 cout<<"ST|get_token BEG:_pos = "<<_pos<<endl;
	 cout<<"ST|get_token BEG:_buffer = "<<_buffer<<endl;
	 }


	if(start_state == UNKNOWN_STATE_START)
	{
		token = temp.substr(_pos, 1);
		++_pos;
if(debug)cout<<"ST|get_token: start_state=UNKNOWN, return true"<<endl;
		return true;
	}

while(_buffer[index] >=0 && _table[start_state][_buffer[index]] !=-1
	&& index <= temp.length() )
	{
	start_state = _table[start_state][_buffer[index]];
	++index;	

	if(is_success(_table, start_state)){
	last_success = index - _pos;
if(debug)cout<<"ST|get_token: last_success="<<last_success<<endl;
	}

	} 


	if(last_success > 0){
	token = temp.substr(_pos, last_success);
//cout <<"pos:"<<_pos<<", last_s:"<<last_success<<endl;
	_pos+=last_success;


if(debug){
	 cout<<endl;
	 cout<<"ST|get_token END:index = "<<index<<endl; 
	 cout<<"ST|get_token END:_pos = "<<_pos<<endl;
	 cout<<"ST|get_token END:_buffer = "<<_buffer<<endl;
	 cout<<"ST|get_token END:token = "<<token<<endl;
	 }

if(debug)cout<<"ST|get_token: return true"<<endl;
	return true;

	} else {

if(debug)cout<<"ST|get_token: last_success="<<last_success<<
	" _pos= "<<_pos<<" return false"<<endl;

	return false;
	}
	

}

bool STokenizer::done()
{
	if(_pos >= strlen(_buffer) )
		return true;

	return false;
}

bool STokenizer::more()
{
	if(_pos <= strlen(_buffer) )
		return true;

	return false;
	

}

#endif 

