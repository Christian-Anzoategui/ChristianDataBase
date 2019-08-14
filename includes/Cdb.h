#ifndef CDB_H
#define CDB_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include "Table.h"
#include "Map.h"
#include "Mmap.h"
#include "DParser.h"

class Cdb {

public:
	Cdb();
	~Cdb();
	void run();


private:

	Map<std::string, Table*> _tables;
	vector<Table> test;
	void string_to_lower(std::string& line);
	void vec_to_lower(vector<string>& vec);
	bool file_exist(const char filename[]);


};

Cdb::Cdb()
{

	std::cout<<"\n\n\n\n";
	std::cout<<"Loading Christian's Database...\n\n";
	_tables.clear();
}

Cdb::~Cdb()
{
	_tables.clear();
}

void Cdb::run()
{
	bool debug = false;
	bool process = true;

while(process)
 {
	std::string user_input ="";
	std::cout<<">:";	
	getline(cin, user_input);

	DParser parser(user_input.c_str() ); 

	MMap<std::string,std::string>
				_input;

 if(parser.parsed_tree(_input) == true)
  {

if(debug)std::cout<<"\nTREE _input\n"<<_input;
	

	vector<std::string> vec = _input["COMMAND"];
	std::string str = vec[0];

if(debug)std::cout<<"str|"<<str<<std::endl;	

	if(str == "LOAD"){
if(debug)std::cout<<"\nLOAD\n";

	
	vec.clear();
	vec = _input["TABLE_NAME"];
	std::string tbl_name = vec[0]; 

//check to see if file exist in
//system if not quit load function
	if(!file_exist(tbl_name.c_str() ))
		continue;
		
	Table* tbl_ptr = new Table(tbl_name.c_str());
	_tables[tbl_name] = tbl_ptr;


	

	}

	if(str == "PRINT"){
if(debug)std::cout<<"\nPRINT\n";

	vec.clear();
	vec = _input["TABLE_NAME"];
	std::string tbl_name = vec[0]; 

//check to see if file
//has been loaded in _tables	
	Table* t=NULL;
	Pair<std::string, Table*> search(tbl_name, t);
	if(!_tables.contains(search) ){
		cout<<"\nTable not loaded\n";
		continue;
	}

	_tables[tbl_name]->show();


	}
	
	if(str == "EXIT"){
if(debug)std::cout<<"\nEXIT\n";

	process = false;	
	}

	if(str == "CREATE"){
if(debug)std::cout<<"\nCREATE\n";

	vec.clear();
	std::string tbl_name = _input["TABLE_NAME"][0]; 

	vector<std::string> field_list = _input["FIELD_NAME"];

//bring field_list vectors back to lower case
	vec_to_lower(field_list);
	
	
	Table* tbl_ptr = new Table(field_list, tbl_name);
	_tables[tbl_name] = tbl_ptr;

	}

	if(str == "INSERT"){
if(debug)std::cout<<"\nINSERT\n";

	vec.clear();
	std::string tbl_name = _input["TABLE_NAME"][0]; 

//check to see if file
//has been loaded in _tables	
	Table* t=NULL;
	Pair<std::string, Table*> search(tbl_name, t);
	if(!_tables.contains(search) )
		continue;

	vector<std::string> fields =  _input["FIELD_LIST"];

//bring fields vectors back to lower case
	vec_to_lower(fields);

	_tables[tbl_name]->insert(fields);
	}

	if(str == "SELECT"){
if(debug)std::cout<<"\nSELECT\n";

	vec.clear();
	std::string tbl_name = _input["TABLE_NAME"][0]; 

//check to see if file
//has been loaded in _tables	
	Table* t=NULL;

	Pair<std::string, Table*> search(tbl_name, t);
	if(!_tables.contains(search) ){
	std::cout<<"\ntable is not loaded, select incorrect\n";
		continue;
	}



/*yes WHERE */
	if(_input.contains("WHERE")){
if(debug)cout<<"\nwhere is here\n";
if(debug)cout<<_input;

	std::string op       = _input["OP"][0];
	string_to_lower(op);
	std::string val      = _input["VALUE"][0];
	string_to_lower(val);
	std::string fieldname= _input["FIELD_NAME"][0];
	string_to_lower(fieldname);

		if(_input.contains("ASTERISK")){
		vec = _tables[tbl_name]->full_field_list();
if(debug)cout<<"vec\n"<<vec;
	
		Table t_temp = _tables[tbl_name]->select
			(vec, fieldname, op, val);
				
if(debug)cout<<"print t_temp\n";
	
		t_temp.show();
		}

		else {

		vec = _input["FIELD_LIST"];
		vec_to_lower(vec);

		Table t_temp = _tables[tbl_name]->select
			(vec, fieldname, op, val);

		t_temp.show();
		}

	} else /*no WHERE*/{
if(debug)cout<<"\nno where here\n";

		if(_input.contains("ASTERISK")){

		_tables[tbl_name]->show();

		} else {

		vec = _input["FIELD_LIST"];
		vec_to_lower(vec);

		Table t_temp = _tables[tbl_name]->select
			(vec);

		t_temp.show();

		}
	}

     }

	
   } else
		std::cout<<"Error ID10T: Bad user input\n";
 }
}


void Cdb::string_to_lower(std::string& line)
{
	for(int i=0; i<line.size(); ++i)
		line[i] = tolower(line[i]);

}

void Cdb::vec_to_lower(vector<string>& vec)
{
	for(int i=0; i< vec.size(); ++i)
		string_to_lower(vec[i]);

}

bool Cdb::file_exist(const char filename[])
{
    fstream f;
    f.open (filename,
        std::fstream::in | std::fstream::binary );
    if (f.fail()){
if (debug)
cout<<"file_exists: File does NOT exist: "<<filename<<
endl<<endl<<endl;
        return false;
    }
if (debug)
cout<<"file_exists: File DOES exist: "<<filename<<
endl<<endl<<endl;
    return true;

}

#endif
