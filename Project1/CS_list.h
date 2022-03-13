#ifndef CS_list_h_
#define CS_list_h_
#include <iostream>
#include "CS_allocator.h"


template<typename Iter> // requires Input_iterator<Iter>() (§19.3.3)
Iter high(Iter first, Iter last)
// return an iterator to the element in [first,last) that has the highest value
{
	Iter high = first;
	for (Iter p = first; p != last; ++p)
		if (*high < *p) high = p;
	return high;
}

// objects of class list are used for creation of iterators (to current/begin/end) and
// the iterators are passed to list to delete a variable
// list_node is not directly manipulated by the user.
// 
//template<typename int>
class list_node {
public:
	list_node* _head;
	list_node* _tail;
	int _val;

	list_node(int value) : _val{ value }, _head{ nullptr }, _tail{ nullptr } {}
	class iterator;				// member type: iterator (acts as prototype), list::iterator is implied where 'iterator' is mentioned
	list_node* _next() { return _tail; }
	list_node* _prev() { return _head; }
	int& operator*() { return _val; }
public:
class iterator {
public:
	list_node* curr; // current link
	iterator(list_node* p) : curr{ p } { }
	iterator& operator++() { curr = curr->_next(); return *this; } // forward, *this returns an object equivalent to this one.
	iterator& operator--() { curr = curr->_prev(); return *this; } // backward
	int& operator*() { return curr->_val; } // get value (dereference)
	bool operator==(const iterator& b) const { return curr == b.curr; }
	bool operator!= (const iterator& b) const { return curr != b.curr; }
	list_node* _next() { return curr->_tail; }
	list_node* _prev() { return curr->_head; }
	};
};

//template<typename int>
class list {
private:
	list_node* get_node(list_node::iterator p);
	list_node* front_ptr;
	int size{ 0 };
	cs_allocator<list_node> alloc;
public:
	list(int i) {
		front_ptr = alloc.allocate(1);
		alloc.construct(front_ptr, i);
		size = 1;
		front_ptr->_tail = alloc.allocate(1);
	}
	list() : size{ 0 } { front_ptr = alloc.allocate(1); }
	~list() {
		if (size > 0)
		{
			while (size > 0)
			{
				auto temp = front_ptr->_tail;
				alloc.destroy(front_ptr);
				alloc.deallocate(front_ptr, 1);
				front_ptr = temp;
				size--;
				if (size == 0)
				{
					alloc.deallocate(front_ptr, 1);
				}
			}
		}
		else {
			alloc.deallocate(front_ptr, 1);
		}
	} // use iterator to erase

	list_node::iterator begin() // iterator to first intent
	{ return list_node::iterator(front_ptr); }
	list_node::iterator end();
	list_node::iterator last();
	void push_back(int i);
	void push_front(int i);
	void pop_front() {erase(begin());}
	void pop_back() {erase(last());}
	void print();	// for testing
	list_node::iterator erase(list_node::iterator p); // used to handle all list deleting
	list_node::iterator insert(list_node::iterator p, const int& v); // insert node{v} as head/prev of p
	int& front()				// the first intent
	{	return **front_ptr;	}
	int& back()				// the last intent
	{	return  **(get_node(last()));	}

};


template<typename Iter> Iter high(Iter first, Iter last);
#include "CS_list_templates.h"
#endif