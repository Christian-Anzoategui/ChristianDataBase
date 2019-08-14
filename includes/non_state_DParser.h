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
	DParser(char str[]);
	MMap<string, int> parsed_tree();
	void process
	(vector<string>& fields_vect, string& table_name, 
		MMap<string, string>& conditions_mmap);
	void commands();

private:
//	STokenizer _stk;
	Map<std::string, int> map_start; 
	vector<string> _inputV;

	void select_parse();
	vector<string> _field_list;
	std::string _table_name;

	void insert_parse();
	

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

DParser::DParser(char str[])
{
	make_map();

bool debug = true;

	Token t;
	std::string temp_str="";

	STokenizer stk(str);
	while(stk.more()){

	t = Token();
	stk>>t;
	
	temp_str = t.token_str();
	string_to_upper(temp_str);
	
//dont push "," " " "\"" "" "'"
	if(temp_str != "," && temp_str != " " &&
		 temp_str!="\"" && temp_str!=""&& temp_str!="'")
	_inputV.push_back(temp_str);

	}

//	make_table(_table);

if(debug)
{
for(int i=0; i<_inputV.size(); ++i)
	cout<<"_inputV["<<i<<"]="<<_inputV[i]<<endl;
}


	if(_inputV[0] == "SELECT")
		select_parse();

	else if(_inputV[0] == "INSERT")
		insert_parse();

	else//unknown
		cout<<"input is toilet level, retry"<<endl;


}

void DParser::make_map()
{

//create map for column_numbers	
	map_start["SELECT"] = SELECT_START;

	map_start["FNAME"] = FIELDS_START;
	map_start["LNAME"] = FIELDS_START;
	map_start["*"] = ALL_FIELDS_START;

	map_start["FROM"] =   FROM_START;

	map_start["TABLE"] =  TABLE_START;
}

void DParser::make_table(int _table[][MAX_COLUMNS])
{




	init_table(_table);
//mark success & fails

	mark_fail(_table, 0);
	mark_fail(_table, 1);
	mark_fail(_table, 2);
	mark_success(_table, 3);

//mark cells

	mark_cells(SELECT_START, _table, "SELECT", FIELDS_START); 

	mark_cells(FIELDS_START, _table, "*", FROM_START); 

	mark_cells(FROM_START, _table, "FROM", TABLE_START); 
	mark_cells(TABLE_START, _table, "STUDENT", TABLE_START); 

}

MMap<string, int> DParser::parsed_tree()
{

	//while(_table[start_state][] && !_inputQ.empty() )
	{


	}

}


void DParser::select_parse()
{
bool debug = true;

//check for from

	bool has_from = false;
	int from_pos = -1;

	for(int i=1; i<_inputV.size(); ++i)
		if(_inputV[i] == "FROM" ){
			has_from = true; 
			from_pos = i;

		}
	
	if(has_from == false){

		cout<<"Missing from, bad input"<<endl;
		return;
	}
		

		
	string temp_str = "";
	for(int i=1;!_inputV.empty() && _inputV[i]!="FROM"; ++i){
	
//		temp_str = _inputV[i];
//		_field_list.push_back(temp_str);
		_field_list += _inputV[i];
	}

if(debug)
{
for(int i=0; i<_field_list.size(); ++i)
	cout<<"_field_list["<<i<<"]="<<_field_list[i]<<endl;
}

//check for name of table and store it
//in private variable 
	if(_inputV.size() > from_pos)
//		_n



}

void DParser::insert_parse()
{

}

void DParser::process
	(vector<string>& fields_vect, string& table_name, 
		MMap<string, string>& conditions_mmap)
{

}

#endif
