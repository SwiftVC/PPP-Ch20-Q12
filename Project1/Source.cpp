/*
*	Author: CS
*	Date:	2022_03_13
*
*	This is worked from a copy of Ch20 Q13, along with the CS_Allocator header (name changed) from Ch19 Q09
*	
*	The list class's implementation is rewritten from the next question (this is Q12, next is Q13) to use an allocator,
*	and to keep one list node allocated, as to terminate the list.
*	In doing so, it complicates the checking through the list, as the end of the list is determined via the list size integer
*	throughout the program as we can no longer check for termination with the null pointer.
*	It isn't generalized for the sake of readability.
* 
*	The std::cout output was commented out to verify that there is no heap memory leak.
* 
*	Criticisms:
*	-ve		Inconsistent approach to connecting list_nodes objects -sometimes unnecessary temporary variables are added.
*	-ve		Testing of each function was not stored for convienient proofing and final validation
*/

// Changes from Ch20 Q13:
// alloc<list_node> private object
// list() includes alloc, list(int) includes alloc					(passes memory check for allocation and deallocation)
// push_back() uses allocator method
// push_front() uses allocator method
// erase() uses allocator method
// last() uses allocator method
// insert() uses allocator method


#include <iostream>
#include "CS_list.h"

int main() {
	try{
		
		{
			list newlist(5);	// initializes a list with the number 5 as the first element
			newlist.erase(newlist.begin());
			newlist.push_front(8);
			newlist.push_front(80);
			newlist.back() = 50;
			newlist.insert(newlist.end(), 650);
			newlist.push_back(200);
			newlist.push_back(900);
			newlist.push_back(50);

			std::cout << "The list is as follows:\n";
			newlist.print();
			std::cout << "Testing the list with a function of STL style:\n";
			std::cout << "The highest is:\t" << *high(newlist.begin(), newlist.end()) << std::endl;
			std::cout << "Iterators with the list object work as expected.\n";
			
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}