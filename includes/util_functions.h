#ifndef UTIL_FUNCIONS_H
#define UTIL_FUNCIONS_H
#include <iostream>
#include <vector>
#include <cassert>

//return the larger of the two items
template <class T>
T maximal(const T& a, const T& b);

//swap the two items
template <class T>
void swap(T& a, T& b);

//return index of the largest item in data
template <class T>
int index_of_maximal(T data[ ], int n);

//insert entry into the sorted array
//data with length n
template <class T>
void ordered_insert(T data[ ], int& n,const T& entry);

//return the first element in data that is                                                        
//not less than entry
template <class T>
int first_ge(const T data[ ], int n, const T& entry);

//return the first element in data that is
//not less than entry
template <class T>
int first_gt(const T data[ ], int n, const T& entry);   
                                                        
//append entry to the right of data
template <class T>
void attach_item(T data[ ], int& n, const T& entry);

//insert entry at index i in data
//call shift right
template <class T>
void insert_item(T data[ ], int i, int& n,const T& entry);

//remove the last element in data and place 
//it in entry
template <class T>
void detach_item(T data[ ], int& n, T& entry);

//delete item at index i and place it in entry                                                       
template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);

//append data2 to the right of data1
template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);   

//move n/2 elements from the right of data1 
//and move to data2
template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   
                                                        
//copy src[] into dest[]
template <class T>
void copy_array(T dest[], const T src[], int size);

//print array data
template <class T>
void print_array(const T data[], int n, int pos = -1);

template <class T>
void shift_right(T data[],int& n,int pos);

template <class T>
void shift_left(T data[],int& n,int pos);

//item > all data[i]
template <class T>
bool is_gt(const T data[], int n, const T& item);

//item <= all data[i]
template <class T>
bool is_le(const T data[], int n, const T& item);

template <class T, class R>
std::vector<T>& operator +=
		(std::vector<T>& list, const R& addme);
//-------------- Vector Extra operators: ---------------------

//list.push_back(addme)
template <class T, class R>
std::vector<T>& operator +=
		(std::vector<T>& list, const R& addme)
{
	list.push_back(addme);
	return list;
} 

//print vector list
template <class T>
std::ostream& operator <<
	(std::ostream& outs, const std::vector<T>& list)
{

	typename std::vector<T>::const_iterator it = list.begin();
	for(; it!= list.end(); ++it){
		outs<<*it<<" ";
	}


	return outs;
}





template <class T>
T maximal(const T& a, const T& b)
{
	if(a > b)
		return a;


	else return b;
}


template <class T>
void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
	
}
template <class T>
int index_of_maximal(T data[ ], int n)
{
	size_t max=0;	
	size_t i=0;	

	for(; i < n; ++i)
		if(data[i] > max)
		max = data[i];
		
	return max;

}


template <class T>
void ordered_insert(T data[ ], int& n,const T& entry)
{

	int i = first_ge(data,n,entry);
	insert_item(data, i, n, entry);
	
	

}

//insert entry at index i in data
//call shift right
template <class T>
void insert_item(T data[ ], int i, int& n,const T& entry)
{
	shift_right(data, n, i);
	data[i] = entry;

}

//return index of the first biggest number in array
template <class T>
int first_ge(const T data[ ], int n, const T& entry)
{

	int i=0;

	for(; i<n; ++i){

		if(data[i] >=entry)
			return i;
	}

//if nothing is bigger than entry, return total
//amount searched 
	return i;
	
}

//return the first element in data that is
//not less than entry
template <class T>
int first_gt(const T data[ ], int n, const T& entry)
{
	int i=0;

	for(; i<n; ++i){

	
		if(data[i] > entry)
			return i;

	}

//if nothing is bigger than entry, return total
//amount searched 
	return i;

}

template <class T>
void shift_right(T data[],int& n,int pos)
{
//cant shift if pos = n, must be bigger
//pos aka index cant be below 0
	//assert(pos > n && pos>=0);

//increase items in array by one
	++n;

	for(int i=n-1; i>pos; --i)
	
		data[i] = data[i-1];

}


template <class T>
void shift_left(T data[],int& n,int pos)
{

	for(int i=pos; i <n-1; ++i)
		
		data[i] = data[i+1];


	--n;
}

template <class T>
bool is_gt(const T data[], int n, const T& item)
{
	for(int i=0; i<n; ++i)
	{
		if(item <= data[i])
		return false;	
	}

	return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item)
{

	for(int i=0; i<n; ++i)
	{
		if(item > data[i])
		return false;	
	}

	return true;

}


template <class T>
void attach_item(T data[ ], int& n, const T& entry)
{
	data[n++] = entry;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry)
{
	n--;
	entry = data[n]; 
}


template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   
{
// spilt array in 2 and set amount of items
// equal to i
	int i = (n1+1)/2 ;
	int j = 0;
	for(;i<n1; ++i,j++){

		data2[j] = data1[i];

	}

//set size of n2 to j
	n2 = j;
//set size of n1 to n1 - n2;
	n1 = n1 - n2;


}

template <class T>
void copy_array(T dest[], const T src[], int size)
{

	for(int i=0; i < size; ++i)

		dest[i] = src[i];

}
template <class T>
void print_array(const T data[], int n, int pos)
{
//pos = start position
	for(int i=pos; i<n; ++i)

		std::cout<<"["<<data[i]<<"] ";

	std::cout<<"\n";
	

}

//append data2 to the right of data1
template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   
{

//start at beg of data2 array
	int d2_beg = 0;
//start at end of data1 array
	int i = n1;

	for(; d2_beg<n2; ++d2_beg, ++i){
//std::cout<<"in for loop\n";
		data1[i] = data2[d2_beg];
	
	}

//set size of new larger array [n1+n2]
	n1 = n1+n2;
//size for data2 is now 0, but trash is still inside
	n2 = 0;

}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry)
{
	entry = data[i];

	for(int j=i+1; j<n; ++j)
	data[j-1] = data[j];
//	shift_left(data, n, i);

//make sure size cant go negative
	if(n > 0)
	--n;

}
#endif 
