#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <iostream>
#include <iomanip>
#include <cassert>
#include <stack>
#include <vector>
#include "util_functions.h"

using namespace std;

template <class T>
class BPlusTree
{
public:

    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *(){
	    assert(it != NULL);
           assert(key_ptr < it->data_count);
		
		return it->data[key_ptr];
	
        }

        Iterator operator++(int un_used){
	
		Iterator hold = *this;

		if(key_ptr+1 == it->data_count){
			
			key_ptr = 0;	
			it = it->next;		
				
		}
		else
		++key_ptr;

		return hold;
        }

//works
        Iterator operator++(){

		if(key_ptr+1 == it->data_count){
			
			key_ptr = 0;	
			it = it->next;		
		}

		else
		++key_ptr;

		return *this;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){

		return (lhs.key_ptr == rhs.key_ptr && lhs.it == rhs.it);
        }


        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){

		return (lhs.key_ptr != rhs.key_ptr || lhs.it != rhs.it);
        
        }

        void print_Iterator(){

		cout<<	it.data[key_ptr];

        }

        bool is_null(){return !it;}

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other, vector<BPlusTree<T>* > &v);//shell to allow function
//    void copy_tree(const BPlusTree<T>& other, queue<BPlusTree<T>* > &q, int &link);//copy other

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                   //return a reference to entry in the tree, insert entry
    T& get_existing(const T& entry);          //returns a refrence but doesnt insert if not found 
    Iterator find(const T& entry);              //return an iterator to this key. NULL if not there.

    int size();                           //count the number of elements in the tree
    bool empty();			        //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();

    Iterator begin();
    Iterator end();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const
        {return child_count==0;}        //true if this is a leaf node

    T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
                                        // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void transfer_left(int i);          //transfer one element LEFT from child i
    void transfer_right(int i);         //transfer one element RIGHT from child i
    void rotate_left(int i);            //rotate left from BTree
    void rotate_right(int i);           //rotate right from BTree

    BPlusTree<T> *merge_with_next_subset(int i);    //merge subset i with subset i+1

};


//---------------------------------------------------------------------
//           P R I N T
//---------------------------------------------------------------------


template <typename T>
void BPlusTree<T>::print_tree(int level,
                    ostream &outs) const
{

bool debug = false;

	if(child_count > MINIMUM){

	subset[child_count-1]->print_tree(level+1,outs);

	}
	

	for(int i=data_count -1;i>=0; --i){

	outs <<std::setw(level *4)<<"";

		if(is_leaf()) outs<<"|";
	outs <<"["<<data[i]<<"]";
		if(is_leaf()) outs<<"|";
if(debug)
{
	if(next == NULL){
		outs <<"{N}"<<endl;
	} else {
		outs <<"{"<<next->data[0]<<"}"<<endl;
	}
}

	outs<<"\n";
		if(!is_leaf() ){
		subset[i]->print_tree(level+1,outs);
		}

	}
}



//---------------------------------------------------------------------
//            C T O R  /  B I G   T H R E E
//---------------------------------------------------------------------

template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
                                   child_count(0), next(NULL){
    //left emtpy
}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):data_count(0),
						child_count(0){
bool debug = false;

if(debug)cout<<"BPT( BPT other) fired\n";
//	clear_tree();

	vector<BPlusTree<T>* > v;
	copy_tree(other, v);
}

template <typename T>
BPlusTree<T>::~BPlusTree(){
//    if (debug) cout<<"** destructor fired!"<<endl;
	clear_tree();		

}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
bool debug = false;

if(debug)cout<<"BPT operator = fired\n";

//check yo self before you reck yo self
	if(this == &RHS)
		return *this;	

	clear_tree();

//	dups_ok = RHS.dups_ok;
	
	vector<BPlusTree<T>* > v;
	copy_tree(RHS, v);

	return *this;



}

//---------------------------------------------------------------------
//            B E G I N ( )   /   E N D ( )  / G E T _ S M _ N O D E
//---------------------------------------------------------------------


template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){

	if(is_leaf() && data_count == 0) {
		return NULL;
	}

	if( is_leaf() ){

		return this;
	}

	else {

		subset[0]->get_smallest_node();
	}


}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    return Iterator(get_smallest_node());
}

/*
template <typename T>
void BPlusTree<T>::begin(){
    this = Iterator(get_smallest_node());
}
*/
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    return Iterator(NULL);
}

//---------------------------------------------------------------------
//            C O N T A I N S / F I N D / G E T / E T C .
//---------------------------------------------------------------------

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry){
	//return an iterator to this key. NULL if not there.

bool debug = false;

	int i =first_ge(data, data_count, entry);
	bool found = (i<data_count && data[i]==entry);



	if(found && is_leaf()){
if(debug)cout<<"\nfound && leaf\n";
		return Iterator(this, i);
	}
	else if(found && !is_leaf()){
if(debug)cout<<"\nfound && !leaf\n";
		subset[i+1]->find(entry);
	}
	else if(!found && is_leaf()){
if(debug)cout<<"\n!found && leaf\n";
		return Iterator(NULL);
	}
	else//(!found && !is_leaf())
	{
if(debug)cout<<"\n!found && !leaf\n";
		subset[i]->find(entry);
	}
}

#if 0
template <typename T>
T& BPlusTree<T>::get(const T& entry){
/*
 * ---------------------------------------------------------------
 * This routine explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 *
 *
 * ---------------------------------------------------------------
 
 *
 */
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found)
            return data[i];
        else{
            cout<<"get called with non-existent entry: "<<entry<<endl;
            insert(entry);
            return get(entry);

            //exit(-1);
        }
    if (found) //inner node
        return subset[i+1]->get(entry);
    else //not found yet...
        return subset[i]->get(entry);


}
#endif

template <typename T>
T& BPlusTree<T>::get(const T& entry) {
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    const bool debug = false;
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);
}

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry) {
    /*
 * ---------------------------------------------------------------
 * This routing explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 *
 * ---------------------------------------------------------------
 *
 */
    //assert that entry is not in the tree.
    //assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if (found) //inner node
        return subset[i+1]->get(entry);
        //or just return true?
    else //not found yet...
        return subset[i]->get(entry);


}

template <typename T>
bool BPlusTree<T>::contains(const T& entry){

	int i = first_ge(data, data_count, entry);
	bool found = (i<data_count && data[i]==entry);

	if(found && is_leaf())
		return true;
	
	else if(found && !is_leaf())
		subset[i+1]->contains(entry);
	
	else if(!found && is_leaf())
		return false;
	
	else//(!found && !is_leaf())
	
		subset[i]->contains(entry);
	

/*
	if(data[i] == entry)
		return true;

	else if(is_leaf() )
		return false;

	else
		return subset[i]->contains(entry);
*/
}

//------------------------------------------------
//          R E M O V E
//------------------------------------------------
template <typename T>
void BPlusTree<T>::remove(const T& entry){
    /*
     * ---------------------------------------------------------------------------------
     * Same as BTree:
     * Loose_remove the entry from this tree.
     * Shrink if you have to
     * ---------------------------------------------------------------------------------
     * once you return from loose_remove, the root (this object) may have no data and
     * only a single subset:
     * now, the tree must shrink:
     *
     * point a temporary pointer (shrink_ptr) and point it to this root's only subset
     * copy all the data and subsets of this subset into the root (through shrink_ptr)
     * now, the root contains all the data and poiners of it's old child.
     * now, simply delete shrink_ptr, and the tree has shrunk by one level.
     * Note, the root node of the tree will always be the same, it's the
     * child node we delete
     *
     *
     */

bool debug = false;
if(debug)cout<<"remove:";

	loose_remove(entry);

	if(data_count == 0 && child_count == 1){

if(debug)cout<<"r: d_c==0, c_c==1; shrink-1:";

		BPlusTree<T>* temp = subset[0];

		copy_array(data, temp->data, temp->data_count);
		data_count = temp->data_count;

		
		copy_array(subset, temp->subset, temp->child_count);
		child_count = temp->child_count;

		temp->child_count = 0;
		
		delete temp;
		
	}
}
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry){
     /* four cases:
           leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
           non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

             |   !found               |   found                 |
       ------|------------------------|-------------------------|-------
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|-------
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|-------


     */
bool debug = false;
if(debug)cout<<"l_r("<<entry<<"):";

	int where_should_go = first_ge(data, data_count, entry);
	int i = where_should_go;
	bool found = i <data_count && data[i]==entry;


    T found_entry;

//leaf
    if (is_leaf()){

if(debug)cout<<"is_leaf:";
        if(!found){
if(debug)cout<<"!found:do nothing\n";
            //[a.] nothing to do
            //entry not in this tree
		return;
        }
        else{
if(debug)cout<<"found:delete_item\n";
            //[b.] just delete the item
            //   my parent will take care of my shortage
		delete_item(data, i, data_count, found_entry);
        }

    }


//not leaf
    else{
if(debug)cout<<"!is_leaf:";
        // ---- 000 B_PLUS_TREE: no more remove_biggest
        if (!found){
if(debug)cout<<"!found:sub["<<i<<"]->l_r\n";
            //[c.] Not here: subset[i]->loose_remove
            //not found yet. search in the next level:
		subset[i]->loose_remove(entry);

		if(subset[i]->data_count < MINIMUM)
			fix_shortage(i);

        }
//not leaf &found
        else{
if(debug)cout<<"found:sub["<<i<<"+1]->l_r\n";
            //[d.] found key in an inner node:subset[i+1]->loose_remove
            assert(i < child_count-1);

            /*
             * Here's The Story:
             * data[i] is the same as the item that we have deleted.
             * so, now, it needs to be replaced by the current smallest key
             *      in subset[i+1]
             * Problem: After fix_shortage, data[i] could have moved to a different index(?)
             *      or it could have sunk down to a lower level as a result of merge
             *      we need to find this item and replace it.
             *
             *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
             *      before fix_excess because this smallest key could be the very entry
             *      we are removing. So, we'd be replacing data[i] with entry. and this will
             *      leave the deleted key in the inner nodes.
             *
             *      After fix_shortage: We have no way of knowing where data[i] key (which is
             *      same as entry) will end up. It could move down one level by the time
             *      fix_shortage returns.
             *
             *      Therefore it seems like we have no choice but to search for it in
             *      data[ ] AND subset[i+1]->data[ ]
             * Current Solution: Kind of messy:
             *      After fix_shortage(i+1):
             *      Search for entry in data[ ] and if found, replace it with smallest.
             *      otherwise, search for it in subset[i+1]->data[ ]
             *          and replace it.
             *
             *
             */
            subset[i+1]->loose_remove(entry);

		if(subset[i+1]->data_count< MINIMUM)
			fix_shortage(i+1);


            //remember. it's possible that data[i] will be gone by the time
            //      fix_shortage returns.
            //key was removed from subset[i+1]:
            //  1. shortage: find entry in data or subset[i+1]->data 
            //              and replace it with subset[i+1]->smallest
            //  2. no shortage: replace data[i] with subset[i+1]->smallest


//check self after to
//see if deleted key is still
//here

	 where_should_go = first_ge(data, data_count, entry);
	 i = where_should_go;
	 found = i <data_count && data[i]==entry;

	if(found){

	delete_item(data, i, data_count, found_entry);
	subset[i+1]->get_smallest(found_entry);

	ordered_insert(data, data_count, found_entry);

//if not in self so might be in i
//which is your childs subset

	} else {

	int j = first_ge(subset[i]->data, subset[i]->data_count, entry);
	bool kid_found = i < subset[i]->data_count && subset[i]->data[i] == entry;
//**sometimes you wont have a
//loose key and wont find it
//in kid 
	if(kid_found){
	delete_item(subset[i]->data, j, subset[i]->data_count, found_entry);
	subset[i]->subset[j+1]->get_smallest(found_entry);

	ordered_insert(subset[i]->data, subset[i]->data_count, found_entry);
	}

	}





        }
        // --------------------------------------------------------
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM,
     *          (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM,
     *          (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling,
     *          merge with prev child: merge(i-1)
     * else
     *          merge with next (right) child: merge(i)
     *
     *
     * returns a pointer to the fixed_subset
     */
const bool debug = false;
if(debug)cout<<"f_s("<<i<<"):";

//if(subset[i]->is_leaf() )
{


//transfer left 

	if( (i <child_count-1) && (subset[i+1]->data_count > MINIMUM))
	{
	//borrow from right subtree if you can

	transfer_left(i);
        return subset[i];

	}

//transfer right
	else if ((i > 0) && (subset[i-1]->data_count > MINIMUM))
	{

	transfer_right(i);
        return subset[i];
        }

//merge left 
	else if (i < child_count -1 )
	{

	merge_with_next_subset(i);
        return subset[i];

        }
//merge right 
	else //((i > 0) )
	{

	merge_with_next_subset(i-1);
        return subset[i-1];

        }

}

//if(debug)cout<<"f_s:!is_leaf:";




}


template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    //replace entry with the left_most leaf of this tree
    // Keep looking in the first subtree (recursive)
    //  until you get to a leaf.
    // Then, replace entry with data[0] of the leaf node
    const bool debug = false;

	if( is_leaf() ){

		entry = data[0];
	}

	else {

		subset[0]->get_smallest(entry);
	}


}

template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    /*
     * ----------------------------------------------------------------------
     *  Merge subset[i] with subset [i+1] REMOVE data[i];
     *  non leaf: same as BTree
     *  leaves  : delete but do not bring down data[i]
     * ----------------------------------------------------------------------
     *
     *   1. remove data[i] from this object
     *   2. if not a leaf, append it to child[i]->data:
     *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to
     *          right of subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. if a leaf, point subset[i]->next to temp_ptr->next
     *   6. delete temp ptr
     *
     *
     * non-leaf nodes: (same as BTree)
     * ------------------
     *  i = 0:
     *             [50  100]
     *          /      |     \
     *  [  ]         [75]       ....
     *    |         /    \
     *   [a]      [b]    [c]
     *
     *  bring down data[i], merge it with subset[i] and subset[i+1]:
     *      then, get rid of subset[i+1]
     *             [100]
     *               |
     *            [50|75]       ....
     *            /  |  \
     *         [a]  [b]  [c]
     *
     *
     *
     * leaf node:
     * ----------------
     * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
     *
     *  i = 0 : merge 5 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [()]->  [10]
     *
     *  Delete data[i] (7), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [10]
     *      /      \
     *  [5]->      [10]
     *
     *
     * i = 1 merge 7 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [7]->  [()]
     *
     *  Delete data[i] (10), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [7]
     *      /     \
     *  [5]->      [7]
     *
     *
     *
     *
     */
const bool debug = false;

if(debug)cout<<"merge_w("<<i<<")\n";

if(subset[i]->is_leaf() ){
//delete root data at i
	T item;
	delete_item(data, i, data_count, item);

//merge i with i+1
	merge(subset[i]->data, subset[i]->data_count,
		subset[i+1]->data, subset[i+1]->data_count);

//set subset btree to i+1 next;
	subset[i]->next = subset[i+1]->next;	

//delete empty bp_ptr at i+1
	BPlusTree<T>* bp_ptr;
	delete_item(subset, i+1, child_count, bp_ptr);
	delete bp_ptr;


	return subset[i];
//merging with non leaf
} else {

	T temp;	
	delete_item(data, i, data_count, temp);
	attach_item(subset[i]->data, subset[i]->data_count, temp);
merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

	BPlusTree<T>* temp_ptr;
	delete_item(subset, i+1, child_count, temp_ptr);
	delete temp_ptr;



}

}
template <typename T>
void BPlusTree<T>::transfer_left(int i){
    /*
     * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     *
     *
     *
     */

const bool debug = false;

if(debug)cout<<"t_left("<<i<<")\n";

if(subset[i]->is_leaf()){
//delete key from parents data
	T item;
	delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);

//insert to empty subset(i);
	ordered_insert(subset[i]->data, subset[i]->data_count, item);

//first delete key then
//insert subset[i+1] data 0+1 into own data
//too put new key
	delete_item(data, i, data_count, item);
	ordered_insert(data, data_count, subset[i+1]->data[0] );
} else {

	rotate_left(i);

}
	
}

template <typename T>
void BPlusTree<T>::transfer_right(int i){
    /*
     * --------------------------------------------------------------
     * transfer right from subset[i] to subset[i+1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave a 'trace':
     *              data[i] is replaced with subset[i+1]->data[0]
     *              after you borrowed the key,
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_right
     * ---------------
     * (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
     *  subset[i+1] is the one being fixed.
     *        [7 | 10]
     *      /    |     \
     * [5|6]->[7 | (8)]=>[()]
     *
     * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
     *        [7 | (10)]
     *      /    |     \
     *   [5|6]->[7]-> [8]
     *
     * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
     *
     *        [7 | 8]
     *      /    |    \
     *   [5|6]->[7]-> [8]
     *
     *
     */

const bool debug = false;

if(debug)cout<<"t_right("<<i<<")\n";

	if(subset[i]->is_leaf() ){
//detach from subset[i-1]->data
	T item;
	detach_item(subset[i-1]->data, subset[i-1]->data_count, item);

//insert to empty subset(i);
	ordered_insert(subset[i]->data, subset[i]->data_count, item);

//first delete key then
//insert item in data, and subset[i]->data
	T trash;
	delete_item(data, i, data_count, trash);

	ordered_insert(data, data_count, item); 
	} else {


	rotate_right(i);

	}

}

//------------------------------------------------
//          I N S E R T
//------------------------------------------------
template <typename T>
void BPlusTree<T>::insert(const T& entry){
    //in order for this class to be able to keep track of the number of the keys,
    //      this function (and the functions it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values)
    //  then the success code must distinguish between inserting a new key, or
    //  adding a new key to the existing key. (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns,
    //  all the subtrees are valid
    //  btree subtrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])

const bool debug = false;
if(debug)cout<<"i:";


	loose_insert(entry);


	if(data_count > MAXIMUM){

if(debug)cout<<"i: d_c>"<<MAXIMUM<<"; grow+1";


	BPlusTree<T>* temp = new BPlusTree(dups_ok);

	copy_array(temp->data, data, data_count);
	temp->data_count = data_count;

	copy_array(temp->subset, subset, child_count);
	temp->child_count= child_count;

	data_count = 0;
	child_count= 1;

	subset[0] = temp;

	fix_excess(0);

	}

}

template <typename T>
void BPlusTree<T>::loose_insert(const T& entry){
    /*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert(i+1)|  loose_insert(i) |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */
const bool debug = false;
if(debug)cout<<"l_i("<<entry<<"):";



       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

	if(found && is_leaf() )
	{
if(debug)cout<<"found&&leaf: d["<<i<<"]=entry\n";
		if(dups_ok){
//MPairs + overloard is called with this new condition
		data[i] = data[i] + entry;
		} else {
		data[i] = entry;
		}
	}

	else if(found && !is_leaf() )
	{
if(debug)cout<<"found&&!leaf: s["<<i<<"+1]->l_i\n";
		subset[i+1]->loose_insert(entry);

		if(subset[i+1]->data_count >MAXIMUM)
			fix_excess(i+1);

//check self after to
//see if deleted key is still
//here
        i = first_ge(data, data_count, entry);
        found = (i<data_count && data[i] == entry);


		

	}

	else if(!found && is_leaf() )
	{

if(debug)cout<<"!found&&leaf: ordered_i\n";

		ordered_insert(data, data_count, entry);

	}

	else//!found && !is_leaf()
	{

if(debug)cout<<"!found&&!leaf: s["<<i<<"]->l_i\n";
		subset[i]->loose_insert(entry);

		if(subset[i]->data_count >MAXIMUM)
		fix_excess(i);

	}

}

template <typename T>
void BPlusTree<T>::fix_excess(int i){
    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    //      subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert
    //      it into this node's data[]
    //
    //Note that this last step may cause this node to have too many itesm.
    //  This is OK. This will be
    //  dealt with at the higher recursive level. (my parent will fix it!)
    //
    //NOTE: ODD/EVENNESS
    // when there is an excess, data_count will always be odd
    //  and child_count will always be even.
    //  so, splitting is always the same.

    //  000 B_PLUS_TREE
    //  if (subset[i]->is_leaf())
    //  transfer the middle entry to the right and...
    //  Deal with next pointers. just like linked list insert

	bool debug = false;
if(debug)cout<<"f_e:";

	T temp;

if(subset[i]->is_leaf()){
if(debug)cout<<"s["<<i<<"]->leaf\n";

	insert_item(subset, i+1, child_count, new BPlusTree(dups_ok) );
	split(subset[i]->data, subset[i]->data_count,
		subset[i+1]->data, subset[i+1]->data_count);

	detach_item(subset[i]->data, subset[i]->data_count, temp);

//insert temp into root
//and subset[i+1]'s data	

	ordered_insert(subset[i+1]->data, subset[i+1]->data_count, temp);
	ordered_insert(data, data_count, temp);

//must set sub i next to subi+1 next to not
//lose what its pointing too
	subset[i+1]->next = subset[i]->next;
//pointing at next bpt object
	subset[i]->next = subset[i+1];
} else{

if(debug)cout<<"s["<<i<<"]->!leaf\n";

//split childs data to temp
//and copy the split data and subset 
BPlusTree<T>* temp = new BPlusTree(dups_ok);
split(subset[i]->data, subset[i]->data_count,
	temp->data, temp->data_count);

split(subset[i]->subset, subset[i]->child_count,
	temp->subset, temp->child_count);


//insert to roots subset i+1
insert_item(subset, i+1, child_count, temp);

T item;
//detach item from child and attach it to root


detach_item(subset[i]->data, subset[i]->data_count, item);
ordered_insert(data, data_count, item);


}


}

//---------------------------------------------------------------
//      S I Z E  /  E M P T Y
//---------------------------------------------------------------
template <typename T>
int BPlusTree<T>::size() {
//this function counts the number of keys in the btree

Iterator it = begin();
int count=0;

for(;it!=end(); ++it){
	++count;
//cout<<"count:"<<count<<endl;
}

return count;
}

template <typename T>
bool BPlusTree<T>::empty() {
return (size() == 0);
}

//---------------------------------------------------------------
//      C L E A R   /  C O P Y
//---------------------------------------------------------------

template <typename T>
void BPlusTree<T>::clear_tree(){
bool debug = false;
if(debug)cout<<"clear_tree: ";	

if(!is_leaf() ){

	for(int i=0; i<child_count; ++i){
if(debug)cout<<"for loop i:"<<i<<"\n";	

	subset[i]->clear_tree();
	delete subset[i];
	subset[i] = NULL;
	}
next = NULL;
data_count = 0;
child_count= 0;
}


}

#if 0
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){

bool debug = true;
if(debug)cout<<"copy_tree:\n";	
//if self is not empty, clear 
if(!empty() )
clear_tree();
int links_to_connect = 0;

stack<BPlusTree<T>* > s;

copy_tree(other, s, links_to_connect);

BPlusTree<T>* p2me = NULL;




for (;! s.empty(); s.pop()){
if(debug)cout<<"*(s.top()):"<<*(s.top());
	s.top()->next = p2me;
	p2me = s.top();
if(debug)cout<<" |p2me:"<<p2me<<endl;
}	
		
if(debug)cout<<"links:"<<links_to_connect<<endl;

}
#endif

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other,
vector<BPlusTree<T>*> &v)
{

bool debug = false;
if(debug)cout<<"c_t:";

copy_array(data, other.data, other.data_count);
data_count = other.data_count;
child_count = other.child_count;


if(other.is_leaf() && !v.empty() ){
if(debug)cout<<" other.leaf & !v.empty:";
		 next = v[0];
if(debug)cout<<" pop|"<<next<<"|";
	//	v.pop_back();
		v.clear();
}


if(other.is_leaf()){
if(debug)cout<<" pusback|"<<this<<"|\n";
	v.push_back(this);
}


if(!other.is_leaf() )
{
	
	for(int i=other.child_count-1; i>=0; --i)
	{
if(debug)cout<<": !leaf i["<<i<<"] ";
if(debug)cout<<"childcount:"<<other.child_count<<endl;
//		BPlusTree<T>* temp = new BPlusTree(dups_ok);
//		subset[i] = temp;
		
		subset[i] = new BPlusTree(other.dups_ok);

		subset[i]->copy_tree(*other.subset[i], v);

	}

} 


}

//---------------------------------------------------------------
//     R O T A T E
//---------------------------------------------------------------

template <typename T>
void BPlusTree<T>::rotate_left(int i){

bool debug = false;
if(debug)cout<<"rotate_left("<<i<<")\n";

T item;
delete_item(subset[i+1]->data,i, subset[i+1]->data_count, item); 
ordered_insert(data, data_count, item);

//now bring down data[0] to subset
delete_item(data, i, data_count, item);
ordered_insert(subset[i]->data, subset[i]->data_count, item);

//if subset[i+1] has children we have 
//to attach them to the place you gave them too
if(!subset[i+1]->is_leaf() ){

	BPlusTree* bt_ptr;
	delete_item(subset[i+1]->subset,i, subset[i+1]->child_count, bt_ptr); 
	attach_item(subset[i]->subset, subset[i]->child_count, bt_ptr);
	
	}

}

template <typename T>
void BPlusTree<T>::rotate_right(int i){

bool debug = false;
if(debug)cout<<"rotate_right("<<i<<")\n";

	T item;
	delete_item(subset[i-1]->data,i, subset[i-1]->data_count, item); 
	ordered_insert(data, data_count, item);

	//now bring down data[0] to subset
	delete_item(data, i, data_count, item);
	ordered_insert(subset[i]->data, subset[i]->data_count, item);
	
	//if subset[i-1] has children we have 
	//to attach them to the place you gave them too
	if(!subset[i-1]->is_leaf() ){

	BPlusTree* bt_ptr;
	delete_item(subset[i-1]->subset,i, subset[i-1]->child_count, bt_ptr); 
	attach_item(subset[i]->subset, subset[i]->child_count, bt_ptr);
	}
	
}


#endif
