#ifndef DPARSER_H
#define DPARSER_H

#include <iomanip>
#include <iostream>
#include <string.h>
#include <vector>
#include <cassert>
#include "Token.h"
#include "Map.h"
#include "Mmap.h"
#include "Constants.h"
#include "state_machine.h"
#include "STokenizer.h"

using namespace std;

class DParser {

public:
	DParser(const char str[]);
	DParser(string str);

	void process(const char str[]);

	bool parsed_tree(MMap<string, string>& pt);

private:
//	STokenizer _stk;
	Map<std::string, int> map_key_col; 
	Map<int, std::string> map_key_row; 
	vector<string> _inputV;


	void make_map();
	void make_table(int _table[][MAX_COLUMNS]);

	void string_to_upper(std::string &line);

	static int _table[MAX_ROWS][MAX_COLUMNS];


};

void DParser::string_to_upper(std::string &line)
{
	for(int i=0; i < line.size(); ++i)
		line[i] = toupper(line[i]);
}

int DParser::_table[MAX_ROWS][MAX_COLUMNS];

DParser::DParser(const char str[])
{
	process(str);
}

void DParser::process(const char str[])
{

bool debug = false;

	Token t;
	std::string temp_str="";

	_inputV.clear();

	STokenizer stk(str);
	while(stk.more()){

	t = Token();
	stk>>t;
	
	temp_str = t.token_str();
	string_to_upper(temp_str);

	if(temp_str != " ")	
	_inputV.push_back(temp_str);

	}

	make_map();
	make_table(_table);

if(debug)
{
for(int i=0; i<_inputV.size(); ++i)
	cout<<"_inputV["<<i<<"]="<<_inputV[i]<<endl;
}


}

void DParser::make_map()
{

//make map for column_numbers	
//select
	map_key_col["SELECT"] =  SELECT;
	map_key_col["CREATE"] =  CREATE;
	map_key_col["INSERT"] =  INSERT;
	map_key_col["*"]      =  ASTERISK;
	map_key_col["\""]     =  QUOTE;
	map_key_col["'"]      =  QUOTE;
	map_key_col[","]      =  COMMA;
	map_key_col["AND"]    =  AND_OR;
	map_key_col["OR"]     =  AND_OR;
	map_key_col["FROM"]   =  FROM;
	map_key_col["WHERE"]  =  WHERE;
	map_key_col["="]      =  OP;
//	map_key_col["<"]      =  OP;
//	map_key_col[">"]      =  OP;
//load
	map_key_col["LOAD"]   =  LOAD;
//print
	map_key_col["PRINT"]  =  PRINT;
//exit
	map_key_col["EXIT"]   =  EXIT;
//insert
	map_key_col["INSERT"] =  INSERT;
	map_key_col["INTO"]   =  INTO;
	map_key_col["VALUES"] =  VALUES;
	map_key_col["VALUE"]  =  I_VALUE;
//	map_key_col[","]      =  I_COMMA;
//create
	map_key_col["CREATE"] =  CREATE;
//	map_key_col[","]      =  C_COMMA;
	map_key_col["TABLE"]  =  C_TABLE;

//make map for row_strings 
	map_key_row[1]  = "COMMAND";
	map_key_row[2]  = "ASTERISK";
	map_key_row[13] = "FIELD_LIST";
	map_key_row[4]  = "TABLE_NAME";
	map_key_row[5]  = "WHERE";
	map_key_row[6]  = "FIELD_NAME";
	map_key_row[7]  = "OP";
	map_key_row[9]  = "VALUE";
	map_key_row[11] = "AND_OR";
//load
//print
	map_key_row[21]  = "COMMAND";
	map_key_row[22]  = "TABLE_NAME";
//exit
	map_key_row[37]  = "COMMAND";
//insert
	map_key_row[29]  = "COMMAND";
	map_key_row[31]  = "TABLE_NAME";
	map_key_row[33]  = "FIELD_LIST";
//	map_key_row[32]  = "VALUE";
//create
	map_key_row[25]  = "COMMAND";
	map_key_row[26]  = "TABLE_NAME";
	map_key_row[27]  = "FIELD_NAME";
//	map_key_row[28]  = "VALUE";


}

void DParser::make_table(int _table[][MAX_COLUMNS])
{




	init_table(_table);
//mark success & fails

	for(int i=0; i<MAX_ROWS; ++i)
		mark_fail(_table, i);
//select
	mark_success(_table, TABLE_NAME);
	mark_success(_table, END_QUOTE);

//mark cells
//		   ROW            COL     NEXT ROW to go to
//select
	mark_cell(START, _table, SELECT,       1); 
	mark_cell(1    , _table, ASTERISK,     2); 

	mark_cell(1    , _table, ANY_STRING,   13); 
	mark_cell(13   , _table, FROM,         3); 
	mark_cell(13   , _table, COMMA,        14); 
	mark_cell(14   , _table, ANY_STRING,   13); 

	mark_cell(2    , _table, FROM,         3); 
	mark_cell(3    , _table, ANY_STRING,   4); 
	mark_cell(4    , _table, WHERE,        5); 
	mark_cell(5    , _table, ANY_STRING,   6); 
	mark_cell(6    , _table, OP       ,    7); 
	mark_cell(7    , _table, OP       ,    7); 
	mark_cell(7    , _table, QUOTE    ,    8); 
	mark_cell(8    , _table, ANY_STRING,   9); 
	mark_cell(9    , _table, QUOTE    ,    10); 
	mark_cell(10   , _table, COMMA    ,    12); 
	mark_cell(10   , _table, AND_OR   ,    11); 
	mark_cell(12   , _table, AND_OR   ,    11); 
	mark_cell(11   , _table, ANY_STRING,   6); 


	mark_success(_table, 22);
//		   ROW            COL     NEXT ROW to go to
//load 
//print
	mark_cell(START, _table, LOAD,       21); 
	mark_cell(21   , _table, ANY_STRING, 22); 

	mark_success(_table, 37);
//exit
	mark_cell(START, _table, EXIT,       37); 


//insert
	mark_success(_table, 33);
//		   ROW            COL     NEXT ROW to go to
//
	mark_cell(START, _table, INSERT,       29); 
	mark_cell(29   , _table, INTO,         30); 
	mark_cell(30   , _table, ANY_STRING,   31); 
	mark_cell(31   , _table, I_VALUE,      32); 
	mark_cell(32   , _table, ANY_STRING,   33); 
	mark_cell(33   , _table, COMMA,        34); 
	mark_cell(34   , _table, ANY_STRING,   33); 

//create	
	mark_success(_table, 27);
//		   ROW            COL     NEXT ROW to go to
//
	mark_cell(START, _table, CREATE,       25); 
	mark_cell(25   , _table, C_TABLE,      36); 
	mark_cell(36   , _table, ANY_STRING,   26); 
	mark_cell(26   , _table, ANY_STRING,   27); 
	mark_cell(27   , _table, COMMA,        28); 
	mark_cell(28   , _table, ANY_STRING,   27); 
}

bool DParser::parsed_tree(MMap<string, string>& pt)
{

	bool debug = false;
if(debug)cout<<"parsed_tree:\n";

	int row = 0;
	int col = 0;
	int i = 0;
	string temp_str = "";

	while(i!=_inputV.size() )
	{
if(debug)cout<<"while:i=|"<<i<<"|";
		temp_str = _inputV[i];
	
if(debug)cout<<"temp_str=|"<<temp_str<<"|";
		if(map_key_col.contains(temp_str)){
			
		col = map_key_col[temp_str];

		} else col = ANY_STRING;
	

if(debug)cout<<"before_row=|"<<row<<"|";
if(debug)cout<<"col=|"<<col<<"|";
		row = _table[row][col];
if(debug)cout<<"after_row=|"<<row<<"|";

	if(row == -1)
		return false;

//Where case
	if(temp_str == "WHERE")
		temp_str = "YES";

//map rows for keys
//if i have it in map_key_row
//then i insert it to mmap 
		if(map_key_row.contains(row)){	
	MPair<string, string> temp_mpair(map_key_row[row],temp_str);
	pt.insert(temp_mpair);
		}

if(debug)cout<<endl;

		++i;
	}
	

//check if success state
	if( is_success(_table, row) )
		return true;

	else 
		return false;
	
}

#endif
