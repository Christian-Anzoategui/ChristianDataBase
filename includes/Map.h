#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <cassert>
#include "BPlusTree.h"

using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()): key(k), value(v){}

    friend ostream& operator <<
	(ostream& outs, const Pair<K, V>& pair)
	{
		outs<<"<"<<pair.key<<", "<<pair.value<<">";
		return outs;
	}

    friend bool operator ==
		(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		return lhs.key == rhs.key;
	}

    friend bool operator < 
		(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		return lhs.key < rhs.key;
	}
friend bool operator > 
		(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		return lhs.key > rhs.key;
	}
friend bool operator <= 
		(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		return lhs.key <= rhs.key;
	}
friend bool operator >= 
		(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
		return lhs.key >= rhs.key;
	}

    friend Pair<K, V> operator + 
		(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
	{
//should not be called or else dire straits 

		assert(lhs.key == rhs.key);	


//		return Pair<K, V>(rhs.key, rhs.value + lhs.value);
		return Pair<K, V>(rhs.key, lhs.value);
	}

};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;

        Iterator(typename map_base::Iterator it=NULL):_it(it)
	{
	//left blank
	}

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

        Pair<K, V> operator *()
	{
//		return *_it.value;
		return Pair<K, V>((*_it).key,(*_it).value);
	}

        friend bool operator ==
			(const Iterator& lhs, const Iterator& rhs)
	{
		lhs._it == rhs._it;
	}

        friend bool operator !=
			(const Iterator& lhs, const Iterator& rhs)
	{
		lhs._it != rhs._it;
	}

    private:
        typename map_base::Iterator _it;

    };

    Map();
//  Iterators
	Iterator begin(){
	return map.begin();
	}
	Iterator end(){
	return map.end();
	}

//  Capacity
	int size() 
	{return map.size();}
	 
	
	bool empty() const
	{return map.empty();}

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void insert(const Pair<K,V> p);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target);

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

//    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<
		(ostream& outs, const Map<K, V>& print_me)
    {
        outs<<print_me.map<<endl;
        return outs;
    }

private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};


template <typename K, typename V>
Map<K, V>::Map(): key_count(1)
{
// not a mucho
}


template <typename K, typename V>
void Map<K, V>::insert(const K& k, const V& v)
{
	Pair<K, V> temp(k, v);
	map.insert(temp);

}
template <typename K, typename V>
void Map<K, V>::insert(const Pair<K,V> p)
{
	map.insert(p);
}

template <typename K, typename V>
void Map<K, V>::erase(const K& key)
{
	Pair<K, V> temp(key, V());
	map.remove(temp);

}

template <typename K, typename V>
void Map<K, V>::clear()
{
	map.clear_tree();

}

//  Element Access
template <typename K, typename V>
V& Map<K, V>::operator[](const K& key)
{
	Pair<K, V> temp(key);
	V& item = map.get(temp).value;
	return item;
}

template <typename K, typename V>
V& Map<K, V>::at(const K& key)
{
//check to see if in map 
//only returns if true
	Pair<K, V> temp(key);
	if(contains(temp)){
	return map.get(temp).value;	
	}

}

template <typename K, typename V>
const V& Map<K, V>::at(const K& key) const
{
	Pair<K, V> temp(key);

	if(contains(temp)){
	return map.get(temp).value;	
	}
}


template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
	return map.find(key);
}

template <typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) 
{
	return map.contains(target);
}
#endif
