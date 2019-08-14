#ifndef MMAP_H
#define MMAP_H
#include <iostream>
#include <vector>
#include <cassert>
#include "BPlusTree.h"
#include "util_functions.h"

using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;
    bool debug = false;
    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
	MPair(const K& k=K()): key(k), value_list(){
if(debug)cout<<"MPair:ctor 1\n";
	}

	MPair(const K& k, const V& v): key(k){
if(debug)cout<<"MPair:ctor 2\n";
		value_list.push_back(v);
//		value_list+=v;
	}
	MPair(const K& k, const vector<V>& vlist): key(k){
if(debug)cout<<"MPair:ctor 3\n";
		value_list = vlist;	
	}
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
	friend ostream& operator <<
		(ostream& outs, const MPair<K, V>& print_me)
	{

		outs<<"<"<<print_me.key<<"| ";
		outs<<print_me.value_list;
		outs<<">";
		return outs;
	
	}

    
	friend bool operator ==
		(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
	{
		return lhs.key == rhs.key;
	}

	friend bool operator < 
		(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
	{
		return lhs.key < rhs.key;
	}

	friend bool operator <= 
		(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
	{
		return lhs.key <= rhs.key;
	}

	friend bool operator >= 
		(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
	{
		return lhs.key >= rhs.key;
	}

	friend bool operator > 
		(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
	{
		return lhs.key > rhs.key;
	}

	friend MPair<K, V> operator + 
		(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
	{
		assert(lhs.key == rhs.key);
		vector<V> list_temp = lhs.value_list;
		
		for(int i =0; i<rhs.value_list.size(); ++i)
			list_temp.push_back(rhs.value_list[i]);	

		MPair<K, V> temp(lhs.key, list_temp);
		
		return temp;
	}
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it=NULL):_it(it){}

	Iterator operator ++(int unused)
	{
		Iterator hold = *this;
		++_it;
		return hold;

	}

        Iterator operator ++()
	{
		++_it;
		return *this;

	}

        MPair<K, V> operator *()
	{
		return MPair<K, V> ((*_it).key, (*_it).value_list );
	}

        friend bool operator ==
		(const Iterator& lhs, const Iterator& rhs)
	{
		return lhs._it == rhs._it;
	}

        friend bool operator !=
		(const Iterator& lhs, const Iterator& rhs)
	{
		return lhs._it != rhs._it;
	}

    private:
        typename map_base::Iterator _it;
    };

    MMap();

//  Iterators
	Iterator begin(){
		return mmap.begin();
	}
	Iterator end(){
		return mmap.end();
	}

//  Capacity
	int size() const
	{return mmap.size();}
	bool empty() const
	{return mmap.empty();}

//  Element Access
    	const vector<V>& operator[](const K& key) const;
    	vector<V>& operator[](const K& key);

//  Modifiers
    void insert(const K& k, const V& v);
    void insert(const MPair<K, V> p);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key);
    vector<V> &get(const K& key);

    Iterator find(const K& key);
    int count(const K& key);
    Iterator  lower_bound(const K& key);
    Iterator  upper_bound(const K& key);



    bool is_valid();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

//  ctor

template <typename K, typename V>
MMap<K, V>::MMap():mmap(true)
{
//left blank
}

//  Modifiers
template <typename K, typename V>
void MMap<K, V>::insert(const K& k, const V& v)
{
	MPair<K, V> temp(k, v);
	mmap.insert(temp);
}
template <typename K, typename V>
void MMap<K, V>::insert(const MPair<K, V> p)
{
	mmap.insert(p);
}

template <typename K, typename V>
void MMap<K, V>::erase(const K& key)
{
	MPair<K,V> temp(key);
	mmap.remove(key);
}

template <typename K, typename V>
void MMap<K, V>::clear()
{
	mmap.clear_tree();
}

//  Element Access

template <typename K, typename V>
vector<V>& MMap<K, V>::operator[]
		(const K& key) 
{
/*
	MPair<K, V> temp(key);	
	
		return mmap.get(temp).value_list;
*/


	MPair<K,V> temp(key);
	vector<V>& item = mmap.get(temp).value_list;
	return item;

}


template <typename K, typename V>
const vector<V>& MMap<K, V>::operator[]
		(const K& key) const
{
	MPair<K,V> temp(key);
	vector<V>& item = mmap.get(temp).value_list;
	return item;

}



//  Operations:

template <typename K, typename V>
bool MMap<K, V>::contains(const K& key)
{
	return mmap.contains(key);
}

template <typename K, typename V>
vector<V> &MMap<K, V>::get(const K& key)
{
	MPair<K, V> temp(key);	
	
	if(contains(temp)){
		return mmap.get(temp);
	}
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K& key)
{
	return mmap.find(key);
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K& key)
{
	MMap<K, V>::Iterator it= mmap.begin();	

	while(it != mmap.end() && (*it).key < key)
		++it;

	return  it;
}

/*
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K& key)
{

}
*/






#endif
