#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class Record{
public:
	Record(){


		for(int i=0; i<MAX; ++i){
		     for(int j=0; j<MAX; ++j)
			record[i][j]= '\0';	
		}

	}


	Record(vector<string> field_names){

		for(int i=0; i<MAX; ++i)
                {
		     for(int j=0; j<MAX; ++j)
			record[i][j]= '\0';	
                }


		for(int i=0; i< field_names.size(); ++i)
		{
			strcpy(record[i], field_names[i].c_str() );
//cout << "record[" << i << "] = " << record[i] << endl;
		}

	}
	
	int get_record_size(){return sizeof(record); }

	string get_row_string(int row);
	long write(fstream& outs);
	long read(fstream& ins, long recno);


    friend ostream& operator<<(ostream& outs,
                               const Record& r);
private:
    static const int MAX = 20;
    char record[MAX][MAX];
};


string Record::get_row_string(int row){

	string str ="";	
	for(int i=0; record[row][i]!= '\0'; ++i)
	 str += record[row][i];	

	
	return str;

}

long Record::write(fstream &outs){
    //write to the end of the file.
    long pos = outs.tellp();

    //outs.write(&record[0], sizeof(record));
//cout << "SIZEOF record " << sizeof(record) << endl;
    outs.write(&record[0][0], sizeof(record));
    return pos;
}
long Record::read(fstream &ins, long recno){
    long pos= recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);


    ins.read(&record[0][0], sizeof(record));
    return ins.gcount();

}

//deprecated / janky
//use upper object (table.show() ) to
//print record
ostream& operator<<(ostream& outs,
                    const Record& r){
	int j;
	for(int i=0; i<r.MAX && r.record[i][0] != '\0'; ++i)
	{
		
		for(j=0;r.record[i][j] !='\0'; ++j)
		{
			
			cout << r.record[i][j];
		}

//		if(r.record[i][j] == '\0')
	
		cout<<endl;	
	}
    return outs;
}


#endif
