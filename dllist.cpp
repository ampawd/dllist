/**
 * @Author: Aram Gevorgyan
 * @Description: Doubly Linked List
 * @Creation Date: 2015-05-18
 * @version: 1.0
 */

#include <iostream>
#include <cassert>
#include <algorithm>

namespace dllist_0
{
	template <typename T>
	struct DLList_node
	{
		T _node_value;
		DLList_node<T> *_next, *_prev;
		DLList_node<T>(const T& = T(), DLList_node<T> * = NULL, DLList_node<T> * = NULL);	
	};

	template <typename T>
	class DLList;

	//iterators base class
	template<typename T>
	class DLList_base_iterator : 
		public std::iterator< std::bidirectional_iterator_tag, T, T*, T& >
	{
		public:		
			friend class DLList<T>;			
			typedef ptrdiff_t			difference_type;
			typedef const T&			const_reference;
			typedef const T*			const_pointer;
			typedef DLList_node<T>*		node_ptr;

			DLList_base_iterator() : ptr(NULL)
			{}

			DLList_base_iterator(node_ptr _ptr): ptr(_ptr)
			{}
			
			bool operator == (const DLList_base_iterator& other) const
			{
				return ptr == other.ptr;
			}
			
			bool operator != (const DLList_base_iterator& other) const
			{
				return ptr != other.ptr;
			}

			//pre incerement
			DLList_base_iterator& operator++()
			{
				assert(ptr != NULL && "list iterator not incrementable!");
				ptr = ptr->_next;
				return *this;
			}

			//post increment
			DLList_base_iterator operator++(int) 
			{
				assert(ptr != NULL && "list iterator not incrementable!");
				DLList_base_iterator tmp(*this);
				ptr = ptr->_next;
				return tmp; 
			}

			DLList_base_iterator& operator--() 
			{
				assert(ptr != NULL && "list iterator not decrementable!");
				ptr = ptr->_prev;
				return *this;
			}

			DLList_base_iterator operator--(int) 
			{
				assert(ptr != NULL && "list iterator not decrementable!");
				DLList_base_iterator tmp(*this);
				ptr = ptr->_prev;
				return tmp; 
			}

		protected:
			node_ptr ptr;
	};

	//const iterator
	template<typename T>
	class DLList_const_iterator;

	template<typename T>
	class DLList_iterator : public DLList_base_iterator<T>		 
	{
		public:
			//const iterator must have access to node pointer
			friend class DLList_const_iterator<T>;

			DLList_iterator() : DLList_base_iterator<T>(NULL)
			{}

			DLList_iterator(node_ptr _ptr) : DLList_base_iterator<T>(_ptr)
			{}
			
			reference operator*() const
			{				
				assert(ptr != NULL && "list iterator not dereferencable!");
				return ptr->_node_value;
			}

			pointer operator->() const
			{
				assert(ptr != NULL && "list iterator not dereferencable!");
				return &(ptr->_node_value);
			}
	};

	//const iterator
	template<typename T>
	class DLList_const_iterator : public DLList_base_iterator<T>
	{
		public:
			DLList_const_iterator() : DLList_base_iterator<T>(NULL)
			{}

			DLList_const_iterator(DLList_iterator<T>& _iter) : DLList_base_iterator<T>(_iter.ptr)
			{}

			DLList_const_iterator(node_ptr _ptr): DLList_base_iterator<T>(_ptr)
			{}

			const_reference operator*() const
			{
				assert(ptr != NULL && "list iterator not dereferencable!");
				return ptr->_node_value;
			}

			const_pointer operator->() const
			{
				assert(ptr != NULL && "list iterator not dereferencable!");
				return &(ptr->_node_value);
			}
	};

	template <typename T>
	class DLList
	{
		public:
			typedef DLList_iterator<T>			iterator;
			typedef DLList_const_iterator<T>	const_iterator;
			typedef DLList_node<T>*				node_ptr;			
			typedef DLList_node<T>				node;		

			DLList();
			DLList(const DLList<T> &);
			DLList &operator=(const DLList<T> &);
			DLList(iterator, iterator);
			~DLList();
		
			void push_back(const T &);
			void push_front(const T &);
			void pop_back();
			void pop_front();
			void reverse();			

			iterator insert(iterator, const T&);
			iterator erase(iterator);

			void remove(const T&);
			void unique();
		
			iterator begin() const;
			iterator end() const;

			const_iterator cbegin() const;
			const_iterator cend() const;
		
			T & front() const;
			T & back() const;
		
			size_t _size;		
			size_t size() const;
			bool empty() const;
			void clear();
		
		private:
			 node_ptr _begin, _tail;
	};
};


using dllist_0::DLList_node;
using dllist_0::DLList_iterator;
using dllist_0::DLList_const_iterator;
using dllist_0::DLList;


template <typename T>
DLList_node<T>::DLList_node(const T& _nvalue = T(), DLList_node<T> *_n = NULL, DLList_node<T> *_p = NULL):
_node_value(_nvalue), _next(_n), _prev(_p)
{}


template <typename T>
DLList<T>::DLList(): _begin(0), _tail(_begin), _size(0)
{}


template <typename T>
DLList<T>::DLList(iterator first, iterator last)
{
	if (first == end())	
	{
		_begin = new node();	
		_tail = _begin; _size = 0;
		return;
	}

	_begin = new node(*first);	
	_tail = _begin; _size = 0;
	for (DLList<T>::iterator i = first; i != last; ++i)
	{			
		push_back(*i);
	}
}


template <typename T>
DLList<T>::DLList(const DLList<T> &other)
{
	if (other.size() < 1)	
	{
		_begin = new node();
		_tail = _begin; _size = 0;	
	}
	else
	{
		_begin = new node(other.front());
		_tail = _begin; _size = 0;
		for (DLList<T>::iterator i = other.begin(); 
			i != other.end(); ++i)
		{			
			push_back(*i);
		}
	}
}


template <typename T>
DLList<T> & DLList<T>::operator=(const DLList<T> &other)
{	
	if (this != &other)
	{
		clear();
		for (DLList<T>::iterator i = other.begin(); 
			i != other.end(); ++i)
		{			
			push_back(*i);
		}
	}
	
	return *this;
}


template <typename T>
DLList<T>::~DLList()
{
	clear();
}


template <typename T>
void DLList<T>::clear()
{	
	while (_size)
	{
		pop_front();
	}	

	if (_tail != NULL)
		_begin = _tail->_next;
}


template <typename T>
void DLList<T>::push_back(const T &_nvalue)
{
	if (_size == 0)
	{
		_begin = new node(_nvalue);	
		_tail = _begin;		
	} 
	else
	{
		_tail->_next = new node(_nvalue, NULL, _tail);
		_tail = _tail->_next;
	}
	
	_size++;	
}


template <typename T>
void DLList<T>::push_front(const T& _nvalue)
{
	if (_size == 0)
	{
		_begin = new node(_nvalue);	
		_tail = _begin;
	} 
	else
	{
		_begin->_prev = new node(_nvalue, _begin, NULL);
		_begin = _begin->_prev;
	}
	
	_size++;	
}


template <typename T>
void DLList<T>::pop_back()
{
	if (_size < 1) return;
	
	--_size;
	node_ptr tmp = _tail;
	_tail = _tail->_prev;
	//_tail->_next = NULL;
	delete tmp;
}


template <typename T>
void DLList<T>::pop_front()
{
	if (_size < 1) return;		
	
	--_size;		
	node_ptr tmp = _begin;
	_begin = _begin->_next;	
	delete tmp;
}


template <typename T>
void DLList<T>::reverse()
{
	if (empty())
		return;
	node_ptr curr = _begin, tmp = NULL;
	_tail = _begin;
	while (_begin)
	{		
		curr = _begin;		
		_begin = _begin->_next;
		tmp = curr->_next;
		curr->_next = curr->_prev;
		curr->_prev = tmp;
	}

	_begin = curr;
}


template <typename T>
DLList_iterator<T> DLList<T>::insert(iterator _before, const T& _nvalue)
{
	iterator ret;
	if (_size == 0)	
	{
		push_back(_nvalue);
		ret = begin();
	}
	else
	{
		if (_before == end())	
		{
			push_back(_nvalue);		
			ret = iterator(_tail);
		}
		else if (_before == begin())
		{
			push_front(_nvalue);	
			ret = _before;
		}
		else
		{			
			_before.ptr->_prev = new node(_nvalue, _before.ptr, _before.ptr->_prev);
			ret = _before.ptr->_prev;
			_before.ptr->_prev->_prev->_next = _before.ptr->_prev;
			++_size;
		}
	}
	
	return ret;	
}


template <typename T>
DLList_iterator<T> DLList<T>::erase(iterator _where)
{	
	//assert(_where != end());
	iterator ret;
	if (_size < 1)
		return end();
	if (_where.ptr == _begin)
	{
		pop_front(); 
		ret = begin();
	}
	else if (_where.ptr == _tail)
	{
		pop_back();
		ret = _tail;
	}
	else
	{
		--_size;
		ret = _where.ptr->_prev;
		node_ptr tmp = _where.ptr;		
		_where.ptr->_prev->_next = _where.ptr->_next;
		_where.ptr->_next->_prev = _where.ptr->_prev;
		delete tmp;
	}

	if (_size == 0)
		return end();
	//assert(_size != 0);
	return ret;
}


template <typename T>
void DLList<T>::remove(const T& _nvalue)
{
	if (_size < 1)
		return;	
	iterator i = begin();
	while (i != end())
	{
		while (*i == _nvalue)
		{	
			i = erase(i);
			if (i == end())
			{
				_begin = _tail = NULL;
				return;
			}
		}
		++i;		
	}	
}


template <typename T>
void DLList<T>::unique()
{
	iterator i = begin(), j;
	while (i != end())
	{
		for (j = i; j != end(); ++j)
		{
			if (*i == *j && i != j)
			{				
				j = erase(j);
				if (j == end())
					break;
			}
		}
		++i;
	}
}


template <typename T>
DLList_iterator<T> DLList<T>::begin() const
{	
	//assert(_begin != NULL);
	return iterator(_begin);
}


template <typename T>
DLList_iterator<T> DLList<T>::end() const
{
	if (_tail)
		return iterator(_tail->_next);
	return iterator();
}


template <typename T>
DLList_const_iterator<T> DLList<T>:: cbegin() const
{
	return const_iterator(_begin);
}


template <typename T>
DLList_const_iterator<T> DLList<T>:: cend() const
{
	if (_tail)
		return const_iterator(_tail->_next);
	return const_iterator();
}


template <typename T>
T & DLList<T>::front() const
{
	//assert(_begin != NULL);
	return _begin->_node_value;
}


template <typename T>
T & DLList<T>::back() const
{		
	//assert(_tail != NULL);
	return _tail->_node_value;	
}


template<typename T>
size_t DLList<T>::size() const
{
	return _size;
}


template<typename T>
bool DLList<T>::empty() const
{
	return _size == 0;
}