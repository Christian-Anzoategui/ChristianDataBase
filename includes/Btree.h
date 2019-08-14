#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <iomanip>

bool debug = true;

using namespace std;

class level
{
public:

level(int l, int t=4): _level(l), _tab(t) {}

friend std::ostream& operator <<
	(std::ostream& outs, const level& print_me)
 {
  outs <<"[L: "<<print_me._level<<"]"
       <<std::setw(print_me._tab*print_me._level);

	return outs;

 }

private:
	int _level;
	int _tab;

};

/**

  |----|----|----|
  |    |    |    | data[MAXIMUM +1]
__|____|____|____|___
|____|____|____|____| subset[MAXIMUM +2]
   /    |    |    \
  /     |    |     \
 /      |    |      \


**/


template <class T>
class BTree
{
public:
    BTree(bool dups = false);
    //big three:
    BTree(const BTree<T>& other);
//    ~BTree();
    BTree<T>& operator =(const BTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count;                                //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const {return child_count==0;}   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
    void rotate_left(int i);                        //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1

};

template <typename T>
BTree<T>::BTree(bool dups)
{
	data_count = 0;
	child_count = 0;

//duplicates ok
	dups_ok = dups;
	
}

//---------------------------------------------------------------
//      P R I N T  E T C.
//---------------------------------------------------------------
template <typename T>
void BTree<T>::print_tree(int level, ostream& outs) const{
    //1. print the last child (if any)
    //2. print all the rest of the data and children

	if(child_count > MINIMUM){
		subset[child_count-1]->print_tree(level+1,outs);

	}

	for(int i=data_count -1;i>=0; --i){

	outs << std::setw(level*4) << ""; // Print the indentation.
	outs <<"["<< data[i]<<"]" <<endl;

		if(!is_leaf()){
		subset[i]->print_tree(level+1,outs);
		}
	

	}
/*
	const int EXTRA_INDENTATION = 4;
	size_t i;

	outs << std::setw(level) << ""; // Print the indentation.
	// Print the data from the root.
	for (i = 0; i < data_count; ++i)
	outs <<"["<< data[i]<<"]" << " ";
	outs << std::endl;

	// Print the subtrees.
	for (i = 0; i < child_count; ++i)
	subset[i]->print_tree(level+EXTRA_INDENTATION);
*/
}


template <typename T>
void BTree<T>::insert(const T& entry)                //insert entry into the tree
{
/*check for duplicates
	if(contains(entry) && !dups_ok )
		return;
*/

//insert entry at the bottom of the BTree
	loose_insert(entry);

//as parent check child if they have
//too much data	
	if(data_count > MAXIMUM){

//create new BTree temp and copy
//data and subset over to temp
//and clear parent, except for having
//one child point to temp

if(debug)cout<<"insert: d_c:"<<data_count;
if(debug)cout<<" |MAX:"<<MAXIMUM<<endl;

	BTree<T>* temp = new BTree();
	copy_array(temp->data, data, data_count);
	temp->data_count = data_count;

	copy_array(temp->subset, subset, child_count);
	temp->child_count = child_count;
	
	data_count = 0;
	child_count = 1;
	subset[0] = temp;

//call fix excess on newly created child
	fix_excess(0);

	}

}

template <typename T>
void BTree<T>::loose_insert(const T& entry)              //allows MAXIMUM+1 data elements in the root
{
    /* 
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         a. found: deal with duplicates
         ! found:
         b. leaf : insert entry in data at position i 
         c. !leaf: subset[i]->loose_insert(entry)
                   fix_excess(i) if there is a need    
            |   found     |   !found        |
      ------|-------------|-----------------|-------
      leaf  |  a. Deal    | b: insert entry |
            |     with    |    at data[i]   |
      ------|  duplicates |-----------------|-------
            |             | d: subset[i]->  |
      !leaf |             |    loose_insert |
            |             |    fix_excess(i)|
      ------|-------------|-----------------|-------
    */  

if(debug)cout<<"loose_insert\n";

	int where_should_go =first_ge(data, data_count, entry);
	int i = where_should_go; 

bool found = (i < data_count) && (data[i] == entry);

	if(found){
		//duplicates ok
		if(dups_ok){
		ordered_insert(data, data_count, entry); 
		}
				
		else {
			data[i] = entry;
			return;
		}
	}

	if(is_leaf())
	{
if(debug)cout<<"loose_insert:is_leaf, entry=";
if(debug)cout<<entry<<endl;
		ordered_insert(data, data_count, entry); 
	}

//if not leaf then look for 
//position where entry should
//go and call loose insert on
//subtree

	else{
if(debug)cout<<"loose_insert:!is_leaf \n";
		subset[where_should_go]->loose_insert(entry);
if(debug)cout<<"loose_insert:!is_leaf where_should_go=";
if(debug)cout<<where_should_go<<"\n";
//when back in parent above after loose
//insert call fix excess if childs data count is over max		
		if(subset[where_should_go]->data_count > MAXIMUM)
		fix_excess(where_should_go); 
	}
}

template <typename T>
void BTree<T>::remove(const T& entry){

    //Loose_remove the entry from this tree.
    //once you return from loose_remove, the root (this object) may have no data and only a single subset
    //now, the tree must shrink:
    //  point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the child node we delete


	loose_remove(entry);

	if(data_count < MINIMUM){



	}


}

template <typename T>
void BTree<T>::loose_remove(const T& entry)              //allows MINIMUM-1 data elements in the root
{
     /* four cases:
          a. leaf && not found target: there is nothing to do
          b. leaf && found target: just remove the target
          c. not leaf and not found target: recursive call to loose_remove
          d. not leaf and found: replace target with largest child of subset[i]

             |   !found    |   found       |
       ------|-------------|---------------|-------
       leaf  |  a: nothing | b: delete     |
             |     to do   |    target     |
       ------|-------------|---------------|-------
       !leaf | c: loose_   | d: replace    |
             |    remove   |    w/ biggest |
       ------|-------------|---------------|-------


     */

	int where_should_go =first_ge(data, data_count, entry);
	int i = where_should_go; 
	T temp;

bool found = i < data_count && entry == data[i];

	if(is_leaf() && found){
if(debug)cout<<"l_r: leaf & found| delete_item\n";
	// delete item
		delete_item(data, i, data_count, temp);
	//decrement child count from parent?
	--child_count;

	}
	
	else if(is_leaf() && !found){
if(debug)cout<<"l_r: leaf & !found| do nothing\n";
	// do nothing
	return;
	}

	else if(!is_leaf() && found){
if(debug)cout<<"l_r: !leaf & found| rm_big then f_s\n";
	// remove biggest, fix shortage

//temp? instead of entry
	subset[where_should_go]->remove_biggest(temp);	
	fix_shortage(where_should_go);


	}

	// !is_leaf && !found
	else {
if(debug)cout<<"l_r: !leaf & !found| l_r then f_s\n";
	// loose remove, fix shortage
	subset[where_should_go]->loose_remove(temp);
	fix_shortage(where_should_go);


	}

}

template <typename T>
void BTree<T>::fix_shortage(int i)
{
    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM, rotate left
     * elif child i-1 has more than MINIMUM, rotate right
     * elif there is a right child, merge child i with next child
     * else merge child i with left child
     */

//rotate left
	//check if anything is to the right
	//and right data count is bigger than min

//?notsure if right
	if(i > child_count+1 && subset[i-1]->data_count > MAXIMUM){
		
		rotate_right(i);

	}

	else if(i < child_count-1 && subset[i+1]->data_count > MINIMUM){

		rotate_left(i);
	}

}
template <typename T>
void BTree<T>::remove_biggest(T& entry){
    // Keep looking in the last subtree (recursive)
    //  until you get to a leaf.
    // Then, detach the last (biggest) data item
    //
    // after the recursive call, fix shortage.


	if(is_leaf())
		detach_item(data, data_count, entry);
	else {
		subset[child_count-1]->remove_biggest(entry);

		if(subset[child_count-1]->data_count < MINIMUM)
			fix_shortage(child_count-1);
	}
}
template <typename T>
void BTree<T>::merge_with_next_subset(int i){
    /*
     *  Merge subset[i] with subset [i+1] with data[i] in the middle
     *
     *   1. remove data[i] from this object
     *   2. append it to child[i]->data
     *   3. Move all data items from subset[i+1]->data to subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. delete temp ptr
     */

}
template <typename T>
void BTree<T>::rotate_left(int i){
    /*
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50 100]
     *  [  ]        [65 75]       ....
     *            [a]  [b]  [c]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *              [65 100]
     *  [50]         [ 75 ]       ....
     *     [a]      [b]  [c]
     *
     *
     *
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     */
    // If necessary, shift first subset of subset[i] to end of subset[i-1]

	T item;
	delete_item(subset[i+1]->data,0, subset[i+1]->data_count, item); 
	ordered_insert(data, data_count, item);

	//now bring down data[0] to subset
	delete_item(data, i, data_count, item);
	ordered_insert(subset[i]->data, subset[i]->data_count, item);
	
	//if subset[i+1] has children we have 
	//to attach them to the place you gave them too
	if(!subset[i+1]->is_leaf() ){

	BTree* bt_ptr;
	delete_item(subset[i+1]->subset,0, subset[i+1]->child_count, bt_ptr); 
	attach_item(subset[i]->subset, subset[i]->child_count, bt_ptr);
	
	}

}

template <typename T>
void BTree<T>::rotate_right(int i){
    /* (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
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
     *
     *
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     */
    // If necessary, shift last subset of subset[i] to front of subset[i+1]

}

template <typename T>
void BTree<T>::fix_excess(int i)                         //fix excess of data elements in child i
{


  //this node's child i has one too many items: 3 steps: 
  //1. add a new subset at location i+1 of this node 
  //2. split subset[i] (both the subset array and the data array) and move half into 
  // subset[i+1] (this is the subset we created in step 1.) 
  //3. detach the last data item of subset[i] and bring it and insert it into this node's data[] 
  // //Note that this last step may cause this node to have too many items. This is OK. This will be 
  // dealt with at the higher recursive level. (my parent will fix it!)  




//split childs data to temp
//and copy the split data and subset 
	BTree<T>* temp = new BTree();
	split(subset[i]->data, subset[i]->data_count,
		temp->data, temp->data_count);

	split(subset[i]->subset, subset[i]->child_count,
		temp->subset, temp->child_count);

if(debug){cout<<"fix_excess, print temp after split\n";
	print_array(subset[i]->data, subset[i]->data_count, 0);
	print_array(subset[i]->subset, subset[i]->child_count, 0);
	}
if(debug)cout<<"f_e, childcount:"<<child_count<<endl;

//insert to roots subset i+1
	insert_item(subset, i+1, child_count, temp);

	T item;
//detach item from child and attach it to root

if(debug)cout<<"f_e, d_c before detach:"<<data_count<<endl;

	detach_item(subset[i]->data, subset[i]->data_count, item);
	ordered_insert(data, data_count, item);

if(debug)cout<<"f_e, d_c after attach:"<<data_count<<endl;

	//ordered_insert(data, data_count, item);


	
	

}

//search entire tree and return a bool
//if entry is in tree
template <typename T>
bool BTree<T>::contains(const T& entry)
{
	int i = first_ge(data,data_count, entry);

	if(data[i] == entry){
//if(debug)cout<<"contains if, i:"<<i<<endl;
		return true;
	}
	else if(is_leaf()){
//if(debug)cout<<"contains e/if, i:"<<i<<endl;
		return false;
	}
//if first two conditions were not met
//call contains on subet and try again
	else {
//if(debug)cout<<"contains else, i:"<<i<<endl;
		return subset[i]->contains(entry);

	}
}


template <typename T>
T* BTree<T>::find(const T& entry) 
{

	int i = first_ge(data,data_count, entry);
	
	if(data[i] == entry)
		return &data[i];

	else if(is_leaf())
		return NULL;

	else
		return subset[i]->contains(entry);
}
/** not working

for future project, searches
B-Tree for item if not found
insert item and the return 
that item

**/
template <typename T>
T& BTree<T>::get(const T& entry){
    //If entry is not in the tree, CRASH
    //assert(contains(entry));

    int i = first_ge(data, data_count, entry);
//    bool found = (i<data_count && data[i]==entry);
//cout<<"found: "<<found<<endl;    
/*
if (found)
        return data[i];
    if (is_leaf()){
        cout<<"get called with non-existent entry: "<<entry<<endl;
        insert(entry);
        return get(entry);

        //exit(-1);
    }
    if (!found)  //yet...
        return subset[i]->get(entry);
*/
}














#endif
