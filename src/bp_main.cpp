#include <iostream>
#include <time.h>
#include "util_functions.h"
#include "Btree.h"
#include "BPlusTree.h"


using namespace std;


int get_rand(int min, int max);
template<typename T>
bool process(string &line, BPlusTree<T>& bp);
bool get_line(string &line);
void test_util();
void test_bt();

int main()
{


	srand(time(NULL));
	BPlusTree<int> btree;


	string line = "";
	while(get_line(line)){
	
	if((process(line, btree) == false))
	break;

	}
//	test_bt();
//	test_util();
		
	return 0;
}






template<typename T>
bool process(string &line, BPlusTree<T>& bp)
{

	int random;	
	int temp_int = 99;
	BPlusTree<int> bp2;

	BPlusTree<int>::Iterator it;
	

	BPlusTree<int>::Iterator it2;
	BPlusTree<int>::Iterator it3;
	
	

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
	cout<<"clear tree:\n";
	cout<<bp<<endl;
	break;

	case 't':
	it = bp2.begin();
	cout <<"iterate thru bp\n";
	for(int i=0; it!= bp2.end(); ++it, ++i){
//	cout <<i<<endl;
	cout << *it<<" " << endl;
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

void test_bt()
{
	BTree<int> bt;	
	int item =22; 
	bt.insert(item);
	bt.insert(24);
	bt.print_tree();
	bt.insert(26);
	bt.print_tree();

}

void test_util()
{
	int array1[20] = {1,4,5,8};
	int array2[20];
	int size =4;
	int size2 =0;

	cout<<"starting array"<<endl;
	print_array(array1, size, 0);

	ordered_insert(array1,size, 17);
	
	cout <<"size after ordered insert: "<<size<<endl;
	print_array(array1, size, 0);



	attach_item(array1, size, 10);
	cout <<"size after attach item: "<<size<<endl;
	print_array(array1, size, 0);
	
	int detach;
	detach_item(array1, size, detach);
	cout <<"size after detach item: "<<size<<endl;
	print_array(array1, size, 0);

	split(array1, size, array2, size2);
	cout <<"size1 after split: "<<size<<"|s2:"<<size2<<endl;
	print_array(array1, size, 0);
	print_array(array2, size2, 0);
	
	
	merge(array1, size, array2, size2);
	cout <<"size1 after merge: "<<size<<"|s2:"<<size2<<endl;
	print_array(array1, size, 0);
	print_array(array2, size2, 0);
}

int get_rand(int min, int max)
{
	int result = rand()%(max-min +1) + min;
	return result;

}
