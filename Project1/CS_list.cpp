#include "CS_list.h"

list_node* list::get_node(list_node::iterator p) {
	list_node* temp = front_ptr;
	list_node::iterator temp_iter = list_node::iterator(temp);
	int error_check{ 0 };
	while (temp_iter != p)
	{
		temp = temp->_next();
		++temp_iter;
		++error_check;
		if (error_check > size)
			throw std::exception("Invalid iterator passed to list.erase(..)");
	}
	return temp;
}

void list::push_back(int i) {
	// Case 1: if empty
	if (size == 0)
	{
		alloc.construct(front_ptr, i);
		front_ptr->_tail = alloc.allocate(1);
		size++;
		return;
	}
	else {
	// Case 2: if populated
		// extra space is pointed to by end(), this will be constructed in and a new end will be allocated
		auto last_constructed_node = get_node(last());
		alloc.construct(last_constructed_node->_tail, list_node(i));
		last_constructed_node->_tail->_head = last_constructed_node;
		last_constructed_node->_tail->_tail = alloc.allocate(1);
		size++;
		return;
	}
}

void list::push_front(int i) {
	// Case 1: if empty
	if (size == 0) {
		push_back(i);
		return;
	}
	// Case 2: if populated
	// extra space is pointed to by end(), this will be constructed in and a new end will be allocated
	auto last_allocated_node = get_node(end());
	
	alloc.construct(last_allocated_node, i);
	auto last_node = get_node(last());
	front_ptr->_head = last_allocated_node;
	last_allocated_node->_tail = front_ptr;
	front_ptr = last_allocated_node;

	last_node->_tail = alloc.allocate(1);	// replace allocated object at end
	size++;
	return;
}

void list::print() { // for testing
	// creates an iterator and outputs each val on a newline
	if (size == 0)
	{
		std::cout << "empty list" << std::endl;
		return;
	}
	auto it = list_node::iterator(front_ptr);
	std::cout << *it << std::endl;
	int count{ 0 };
	while (count < size) {
		++it;
		++count;
		if (count < size)
			std::cout << *it << std::endl;
	}
}

list_node::iterator list::insert(list_node::iterator p, const int& v) {
	// Case 1: no entries - return nullptr
	if (size == 0)
		return nullptr;
	// Case 2: insert at front
	if (p == list_node::iterator(front_ptr)) {
		push_front(v);
		return list_node::iterator(front_ptr);
	}
	// Case 3: insert between two entries
	if (p!=end()) {
		list_node* allocated_end = get_node(end());
		alloc.construct(allocated_end, list_node(v));	// allocated end is written into

		list_node* node_after_insertion = get_node(p);	// allocated + constructed end is reconnected where desired
		list_node* node_before_insertion = node_after_insertion->_head;
		allocated_end->_head = node_before_insertion;
		allocated_end->_tail = node_after_insertion;
		node_before_insertion->_tail = allocated_end;
		node_after_insertion->_head = allocated_end;
		
		list_node* last_constructed_node = get_node(last()); // allocated end is replaced
		last_constructed_node->_tail = alloc.allocate(1);
		size++;
		return list_node::iterator(allocated_end);	// newly constructed and connected object is returned
	}
	// Case 4: end
	else if(p==end()) {
		push_back(v);
		return list_node::iterator(last());
	}
	throw std::exception("Uncaught insert iterator");
}

list_node::iterator list::erase(list_node::iterator p) {		// used to handle all list deleting
	// Case 1: no entries
	if (size == 0)
		return nullptr;
	// Case 2: only entry
	else if (size == 1)
	{
		list_node* allocated_end = front_ptr->_tail;
		alloc.destroy(front_ptr);
		alloc.deallocate(front_ptr, 1);
		front_ptr = allocated_end;
		size--;
		return nullptr;
	}
	// Case 3: first in list
	else if (list_node::iterator(front_ptr) == p) {
		list_node* temp = front_ptr->_next();
		front_ptr = front_ptr->_next();
		alloc.destroy(temp);
		alloc.deallocate(temp, 1);
		size--;
		return list_node::iterator(front_ptr);
	}
	// Case 4: from second to last
	else {
		int count{ 0 };
		list_node* temp{ front_ptr };
		while (list_node::iterator(temp) != p && count < size)
		{
			++temp;
			++count;
			if (list_node::iterator(temp) == p){
				list_node* ret_val = temp->_head;

				(temp->_head)->_tail = temp->_tail;
				(temp->_tail)->_head = temp->_head;
				alloc.destroy(temp);
				alloc.deallocate(temp, 1);
				--size;
				return ret_val;
			}
		}
	}
	// Case 5: last
		// connect second last to allocated
	list_node* to_del = get_node(last());
	list_node* ret_val = to_del->_head;
	(to_del->_head)->_tail = to_del->_tail;
	(to_del->_tail)->_head = to_del->_head;
		// delete last
	alloc.destroy(to_del);
	alloc.deallocate(to_del, 1);
	size--;
	return ret_val;
}

list_node::iterator list::last()
{
	list_node::iterator temp{ front_ptr };
	int count = 0;
	while (count < size - 1)
	{
		++temp;
		++count;
	}
	return temp;
}

list_node::iterator list::end()
{
	list_node::iterator it(front_ptr);
	int count = 0;
	while (count < size)
	{
		++count;
		++it;
	}
	return it;
}