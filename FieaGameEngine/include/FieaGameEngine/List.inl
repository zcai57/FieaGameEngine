#pragma once
#include "FieaGameEngine/List.h"
#include "List.h"

namespace Fiea::Engine
{
    // List default constructor
	template <typename T>
    List<T>::List() : _head(nullptr), _tail(nullptr), _size(0) {}
    // List copy constructor
    template <typename T>
    List<T>::List(const List& other) : _head(nullptr), _tail(nullptr), _size(0)
    {
        Node* curr = other._head;
        while (curr != nullptr) {
            push_back(curr->value);
            curr = curr->next;
        }
    }
    // Internal helper function
    // This delete lists
    template<typename T>
    inline void List<T>::clearList()
    {
        Node* curr = _head;
        while (curr != nullptr) {
            Node* temp = curr->next;
            delete curr;
            curr = temp;
        }
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    }
    // List destructor
    template <typename T>
    List<T>::~List() 
    {
        clearList();
    }
    // Initializer list
    template<typename T>
    inline List<T>::List(std::initializer_list<T> init_list) : _head(nullptr), _tail(nullptr), _size(0)
    {
        for (const T& value : init_list) {
            push_back(value); 
        }
    }
    // Assignment operator override
    template<typename T>
    inline List<T>& List<T>::operator=(const List<T>& other)
    {
        // If assign to self
        if (this == &other) {
            return *this;
        }
        // otherwise clear old self
        clearList();

        _head = nullptr;
        _tail = nullptr;
        _size = 0;
        // Assign current using other list
        Node* cur = other._head;
        while (cur != nullptr) {
            push_back(cur->value);
            cur = cur->next;
        }
        return *this;
    }
    // Iterator Constructor
    template<typename T>
    inline List<T>::Iterator::Iterator() : _node(nullptr), _beforeEnd(nullptr) {}
    // Begin function
    template<typename T>
    inline typename List<T>::Iterator List<T>::begin()
    {
        // use head as begin node
        Iterator curr;
        curr._node = _head;
        curr._beforeEnd = _tail;
        return curr;
    }
    // End function
    template<typename T>
    inline typename List<T>::Iterator List<T>::end()
    {
        // Use nullptr as end node
        Iterator curr;
        curr._node = nullptr;
        curr._beforeEnd = _tail;
        return curr;
    }

    // List pop back
    template <typename T>
    void List<T>::pop_back()
    {
        // if size is 0, exception
        /*if (_size == 0_z) {
            throw std::out_of_range("List is empty");
        }*/
        // if size is 1, clear current node
        if (_size == 1_z) {
            delete _tail;
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            // update tail
            Node* curr = _tail->prev;
            curr->next = nullptr;
            delete _tail;
            _tail = curr;
        }
        // update size
        _size -= 1;
    }
    // List push back
    template<typename T>
    inline void List<T>::push_back(const value_type& item)
    {
        // create a new node to push
        Node* curr = new Node();
        curr->value = item;
        // if list is empty, push in as head and tail
        if (_tail == nullptr) {
            _head = _tail = curr;
        }
        else {
            // Push in and update tail
            _tail->next = curr;
            curr->prev = _tail;
            _tail = curr;
        }
        _size += 1;
    }
    // List pop front
    template<typename T>
    inline void List<T>::pop_front()
    {
        // if size is 0, exception
        /*if (_size == 0_z) {
            throw std::out_of_range("List is empty");
        }*/
        // if size is 1, clear current node
        if (_size == 1_z) {
            delete _head;
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            // update head
            Node* curr = _head->next;
            curr->prev = nullptr;
            delete _head;
            _head = curr;
        }
        // update size
        _size -= 1;
    }
    // List push front
    template<typename T>
    inline void List<T>::push_front(const value_type& item)
    {
        // create new node to push
        Node* curr = new Node();
        curr->value = item;
        // If list is empty, push node as head and tail
        if (_head == nullptr) {
            _head = _tail = curr;
        }
        else {
            // if List is not empty, push and update head
            curr->next = _head;
            _head->prev = curr;
            _head = curr;
        }
        _size += 1;
    }
    // List back accessor
    template<typename T>
    inline T& List<T>::back()
    {
        // if empty list, exception
        /*if (_tail == nullptr) {
            throw std::out_of_range("List empty.");
        }*/
        // return tail value ref
        return _tail->value;
    }
    template<typename T>
    inline const T& List<T>::back() const
    {
        // if empt list, exception
        /*if (_tail == nullptr) {
           throw std::out_of_range("List empty.");
        }*/
        // return tail value cosnt ref
        return _tail->value;
    }
    template<typename T>
    inline T& List<T>::front()
    {
        // if list empty, exception
        /*if (_head == nullptr) {
            throw std::out_of_range("List empty.");
        }*/
        // return head value ref
        return _head->value;
    }

    template<typename T>
    inline const T& List<T>::front() const
    {
        // if list empty, exception
        /*if (_head == nullptr) {
            throw std::out_of_range("List empty.");
        }*/
        // return head value const ref
        return _head->value;
    }
    // List size getter
    template<typename T>
    inline size_t List<T>::size() const
    {
        return _size;
    }
    // List empty function
    template<typename T>
    inline bool List<T>::empty() const
    {
        // return if size is 0
        return this->_size == 0_z;
    }
    // Override List double equal operator
    template<typename T>
    inline bool List<T>::operator==(const List& other)
    {
        // if size not same, return false
        if (other.size() != _size) {
            return false;
        }
        // if size same, compare each node
        Node* cur = _head;
        Node* cur_o = other._head;
        while (cur != nullptr) {
            if (cur->value != cur_o->value) {
                return false;
            }
            cur = cur->next;
            cur_o = cur_o->next;
        }
        return true;
    }
    
   /* template<typename T>
    inline bool List<T>::operator!=(const List& other)
    {
        return false;
    }*/
    
    // Iterator * operater return node value reference
    template<typename T>
    inline T& List<T>::Iterator::operator*()
    {
        return this->_node->value;
    }
    // Iterator const * operater return node value reference
    template<typename T>
    inline const T& List<T>::Iterator::operator*() const
    {
        return this->_node->value;
    }
    // Iterator -> operator return address of node value
    template<typename T>
    inline T* List<T>::Iterator::operator->()
    {
        return &(_node->value);
    }
    // Iterator const -> operator return address of node value
    template<typename T>
    inline const T* List<T>::Iterator::operator->() const
    {
        return &(_node->value);
    }
    // Iterator pre-increment
    template<typename T>
    inline typename List<T>::Iterator& List<T>::Iterator::operator++()
    {
        // if current node is not null, increment
        if (_node != nullptr) {
            _node = _node->next;
        }
        // return ref of this
        return *this;
    }
    // Iterator post-increment
    template<typename T>
    inline typename List<T>::Iterator List<T>::Iterator::operator++(int)
    {
        // make a copy of current iterator and later return
        Iterator cp = *this;
        // increment node if not null
        if (_node != nullptr) {
            _node = _node->next;
        }
        return cp;
    }
    // Iterator pre-decrement
   template<typename T>
    inline typename List<T>::Iterator& List<T>::Iterator::operator--()
    {
        // if node is null and before end node has value, update node to before end
        if (_node == nullptr) {
            if (_beforeEnd != nullptr) {
                _node = _beforeEnd;
            }
            /*else {
                throw std::out_of_range("Cannot decrement from an invalid end iterator");
            }*/
        } else if (_node->prev != nullptr) {
            _node = _node->prev;
        }
        return *this;
    }
    // Iterator post decrement
    template<typename T>
    inline typename List<T>::Iterator List<T>::Iterator::operator--(int)
    {
        Iterator cp = *this;
        // if node is null and before end node has value, update node to before end
        if (_node == nullptr) {
            if (_beforeEnd != nullptr) {
                _node = _beforeEnd;
            }
           /* else {
                throw std::out_of_range("Cannot decrement from an invalid end iterator");
            }*/
        } else if (_node->prev != nullptr) {
            _node = _node->prev;
        }
        return cp;
    }

}