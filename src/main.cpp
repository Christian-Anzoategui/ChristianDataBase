/*
 * Christian DataBase
 * created by Christian Molina
 *
 * https://github.com/illegalaccount/
 *
 *
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include "Map.h"
#include "Mmap.h"
#include "Btree.h"
#include "BPlusTree.h"
#include "util_functions.h"
#include "Constants.h"
#include "DParser.h"
#include "Token.h"
#include "FTokenizer.h"
#include "STokenizer.h"
#include "Record.h"
#include "Table.h"
#include "Cdb.h"
#include "state_machine.h"

using namespace std;

void bpt();
int get_rand(int min, int max);
template<typename T>
bool process(string &line, BPlusTree<T>& bp, BPlusTree<T>& bp2);
bool get_line(string &line);

int main()
{


	Cdb d_base;
	d_base.run();
	
	return 0;
}



int get_rand(int min, int max);
template<typename T>
bool process(string &line, BPlusTree<T>& bp);
bool get_line(string &line);

void bpt()
{


	srand(time(NULL));
	BPlusTree<int> bp;
	BPlusTree<int> bp2;


	string line = "";
	while(get_line(line)){
	
	if((process(line, bp, bp2) == false))
	break;

	}

		
}






template<typename T>
bool process(string &line, BPlusTree<T>& bp, BPlusTree<T>& bp2)
{

	int random;	
	int temp_int = 99;


	BPlusTree<int>::Iterator it;
	BPlusTree<int>::Iterator it2;
	BPlusTree<int>::Iterator it3;
	
	it2 = bp2.begin();
	it = bp.begin();
	

	switch(line[0])
	{

	case 'r':
	random = get_rand(0, 100);
	cout<<"random insert: "<<random<<endl;
	bp.insert(random);
//	cout<<"size:"<<bp.size()<<endl;
	cout<<endl<<bp<<endl;
	break;

	case 'x':
	cout<<"bye!\n";
	return false;
	
	case 'i':
	line.erase(0,1);
	temp_int = stol(line.c_str());	
	cout<<"insert: "<<temp_int<<endl;
	bp.insert(temp_int);
//	cout<<"size:"<<bp.size()<<endl;
	cout<<endl<<bp<<endl;
	break;

	case 'f':
	line.erase(0,1);
	temp_int = stol(line.c_str());	
	cout<<endl<<"find: "<<temp_int;
	it3 = bp.find(temp_int);

	cout<<"*it="<<*it3<<endl;

	break;

	case 'd':
	line.erase(0,1);
	temp_int = stol(line.c_str());	
	cout<<"delete: "<<temp_int<<endl;
	bp.remove(temp_int);
//	cout<<"size:"<<bp.size()<<endl;
	cout<<endl<<bp<<endl;
	break;

	case 'e':
	bp.insert(5);
	bp.insert(10);
	bp.insert(7);
	bp.insert(8);
	cout<<endl<<"bp:"<<endl;
	cout<<bp<<endl;

	//bp2.copy_tree(bp);
	bp2 = bp;
	
	cout<<"bp copied to bp2:"<<endl;
	//cout<<"size:"<<bp2.size()<<endl;
	cout<<bp2<<endl;

	break;

	case 'c':
	bp.clear_tree();
	bp2.clear_tree();
	cout<<"clear trees:\n";
	cout<<bp<<endl;
	cout<<bp2<<endl;
	break;

	case 't':


	switch(line[1])
	{

		case '1':
		cout <<"iterate thru bp"<<line[1]<<"\n";
		cout <<"size: "<<bp.size()<<endl;
			for(int i=0; it!= bp.end(); ++it, ++i)
			cout <<*it<<" " << endl;
			break;
		
		case '2':
		cout <<"iterate thru bp"<<line[1]<<"\n";
		cout <<"size: "<<bp2.size()<<endl;
			for(int i=0; it2!= bp2.end(); ++it2, ++i)
			cout <<*it2<<" " << endl;
			break;
	}

	cout<<endl;
	break;

	default:
	break;
	
	}
	return true;

}

bool get_line(string &line)
{
	string prompt ="copi[e]d i[t]erator [r]andom [i]nsert [c]lear [f]ind [d]elete e[x]it";
	cout<<prompt<<":\n";
	getline(cin, line);
	return !line.empty();

}

int get_rand(int min, int max)
{
	int result = rand()%(max-min +1) + min;
	return result;

}

