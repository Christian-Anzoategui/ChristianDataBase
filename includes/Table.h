#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cassert>
#include <string>
#include <cstring>
#include <stdio.h>
#include "Record.h"
#include "Map.h"
#include "Mmap.h"

class Table
{
public:
	Table();
//open an exisiting table
//assert it exits
	Table(string name);

//creating new table "name" and adding the field_lists
	Table(std::vector<string> f_list, string name);

	void insert(vector<std::string> fields);

	Table select(vector<std::string> field_list,
		string fieldname, string op, string value);

	Table select(vector<std::string> field_list);

	void show();

//return vector<string> of all field_list
	vector<string> full_field_list();
	

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);
void save_list(Record list[], int count);
private:
	
	std::string _name;
	long _rec_count;

	Map<string, int> _fields_map; 
	MMap<string, int> _indices[10];	

};

Table::Table(): _rec_count(0) {}

//ctor : opening a table that already exists
Table::Table(string name): _rec_count(0)
{
bool debug = false;

	if(file_exists(name.c_str() )){
		_name = name;

		int length = 0;

		fstream f;
		open_fileRW(f, _name.c_str() );	
		
		f.seekg(0, f.end);
		length = f.tellg();
		f.seekg(0, f.beg);

		Record temp_rec;
		_rec_count = length/temp_rec.get_record_size();

if(debug)cout<<"_rec_count:"<<_rec_count<<endl;

	    f.close();
		
//open _name.txt which
//stores field list in non
//binary file and build private
//fields map
		ifstream ff;
		ff.open( (_name+".txt").c_str() );
		if(ff.is_open()){

		int i= 0;
		string temp_str = "";

		while(ff){
			getline(ff,temp_str);
			if(ff.eof()) break;
			Pair<string, int> temp(temp_str, i);
			++i;
			_fields_map.insert(temp); 
			}
		}
		
		ff.close();	
if(debug)cout<<"_fields_map|"<<_fields_map<<endl;

		
	fstream g;
	open_fileRW(g, _name.c_str() );	

	for(int i=0; i<_rec_count; ++i)
	{	
		Record temp_rec;
		temp_rec.read(g, i);
//		vector<string> temp;

			for(int j=0; j< _fields_map.size(); ++j){

//			  temp.push_back(temp_rec.get_row_string(j));
	           _indices[j].insert(temp_rec.get_row_string(j), i);
			}

	}

if(debug)show();

	}	


}


//ctor : opening a table that doesnt exist
Table::Table
(std::vector<string> f_list, string name):_name(name), _rec_count(0)
{
//clears file from string given (_name)
	remove(_name.c_str() );

	for(int i=0; i<f_list.size(); ++i){
		Pair<string, int> temp(f_list[i], i);
		_fields_map.insert(temp); 
	}	

//write field list to txt file	
	string temp_filename = _name;
	temp_filename += ".txt";
	ofstream o;	
	o.open( temp_filename.c_str() );

	if(o.is_open() ){

		for(int i=0; i<f_list.size();++i)
			o<<f_list[i]<<'\n';
	} else
	 cout<<"file crashed, couldnt write field_map\n";
	
	o.close();	
}

vector<string> Table::full_field_list()
{
	vector<string> vec;

	ifstream in;
	in.open( (_name+".txt").c_str() );

	string temp_str="";
	if(in.is_open() ){
			while(in){	
				getline(in, temp_str);	
				if(in.eof()) break;
				vec.push_back(temp_str);
			}

	} else
	cout<<"error opening file in full_field_list\n";

		
	in.close();	
	return vec;

}

Table Table::select(vector<std::string> field_list,
		string fieldname, string op, string value)
{
	bool debug = false;

//clears temp file
	string te="temp";
	remove(te.c_str() );

	int index;
	vector<int> rec_pos;


	if( op == "="){
/**
ex
 field_list = {fname, avg}

 fieldname = "fname"
 op = "="
 value = "christian"

 index = 0
	   
 rec_pos = _indices[0]["christian"] 
 rec_pos = {1,2}

 open_fileRW(f, "ballplayers")

 for(i=0; i<2; i++)
	r.read(f,1);	

  for(j=0;j<2;j++)
	index 0 = _fields_map["fname"];	
	r.get_row(0) //"christian"
	temp.push_back("christian")	

	index 2 = _fields_map["age"]; 
	r.get_row(2) //".211"	
	temp.push_back(".211")
  t.insert(temp)
  
**/		
		index = _fields_map[fieldname];

		rec_pos = _indices[index][value];

		Table t = Table(field_list, "temp");	

		fstream f;
		open_fileRW(f,_name.c_str() );
		
if(debug)cout<<"field_list size:"<<field_list.size()<<"|\n";
		for(int i=0; i<rec_pos.size(); ++i){

			Record r;
			r.read(f,rec_pos[i]);
if(debug)cout<<"r:\n"<<r<<endl;
			vector<string> temp;
			for(int j=0; j< field_list.size(); ++j){

			index = _fields_map[field_list[j]];
if(debug)cout<<"r.g_str("<<j<<"): "<<r.get_row_string(index)<<"\n";
			temp.push_back(r.get_row_string(index));

			}
			t.insert(temp);
		}
if(debug)cout<<"t.show() inside of select\n";
if(debug)t.show();

		f.close();

if(debug)cout<<"Returning from select"<< endl;

		return t;
	}	


}

Table Table::select(vector<std::string> field_list)
{
bool debug = false;
//clears temp file
	string te="temp";
	remove(te.c_str() );

	int index;
	vector<int> rec_pos;

	Table t = Table(field_list, "temp");	

	MMap<string, int>::Iterator it;
	vector<string> temp;

        for(int i=0; i<field_list.size(); ++i){
	    index = _fields_map[field_list[i]];
if (debug)
    cout << "Index Key: " << i << " - " << _indices[index] << endl;
	    it = _indices[index].begin();
            for (; it != _indices[index].end(); ++it)
                 temp.push_back((*it).key);
	    t.insert(temp);
	    temp.clear();
        }

	return t;
}

void Table::insert(vector<std::string> fields)
{
//create temp record to write
	Record temp(fields);

//copy vector string into indices
	for(int i=0; i<fields.size(); ++i)
		_indices[i].insert(fields[i], _rec_count);

//write our data to file
	fstream f;
	open_fileW(f, _name.c_str() );
	temp.write(f);
	f.close();

	++_rec_count;	
}

void Table::show()
{
	bool debug = false;
if(debug)cout<<"show:";

	if(_rec_count == 0) {
	cout<<_name<<" is empty: nothing to show\n";
	return;
	}	

	fstream f;
	open_fileRW(f, _name.c_str() );	
	Record temp_rec;
	
	for(int i=0; i<_rec_count;cout<<endl, ++i)
	{	
		temp_rec.read(f, i);
		cout << temp_rec;
	}
	
	f.close();

//**DEBUG: print indices
if(debug)
 {
	cout<<"\n_indices\n";
if(debug)cout<<"_fields_map.size()="<<_fields_map.size()<<endl;
	for(int i=0; i<_fields_map.size(); i++)
	{
if(debug)cout<<" i="<<i<<endl;
		cout<<_indices[i]<<endl;
	}
 }


}

//-------------------------------------------------
//    PRIVATE HELPER FUNCTIONS
//-------------------------------------------------

bool Table::file_exists(const char filename[])
{
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
if (debug)
cout<<"file_exists: File does NOT exist: "<<filename<<endl;
        return false;
    }
if (debug)
cout<<"file_exists: File DOES exist: "<<filename<<endl;
    return true;
}

void Table::open_fileRW(fstream& f,
		const char filename[]) throw(char*)
{
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"
		<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
if (debug)
cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | 
		std::fstream::out| std::fstream::binary );

        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void Table::open_fileW(fstream& f, const char filename[])
{
    f.open (filename,
            std::fstream::out| 
		std::fstream::binary |std::ios_base::app);
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}
void Table::save_list(Record list[], int count)
{
    fstream f;
    open_fileW(f, "record_list.bin");
    for (int i= 0; i<count; i++){
        list[i].write(f);
    }
    f.close();

}


#endif 
