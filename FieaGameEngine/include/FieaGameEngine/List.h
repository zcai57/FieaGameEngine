#pragma once

namespace Fiea::Engine
{
    template <typename T>
    class List
    {
    public:
        class Iterator;
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using iterator = List::Iterator;

        // Special Member Functions
        List();

        List(const List& other);
        ~List();
        List& operator=(const List& other);

        // Initializer list constructor
        List(std::initializer_list<T> init_list);
        //Iterators
        Iterator begin();
        Iterator end();
        // Modifier
        void pop_back();
        void push_back(const value_type& item);
        void pop_front();
        void push_front(const value_type& item);

        // Accessors
        reference back();
        const_reference back() const;
        reference front();
        const_reference front() const;

        // Information
        size_type size() const;
        bool empty() const;

        // Other
        bool operator==(const List& other);

       /* bool operator!=(const List& other);*/

        class Iterator {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator();
            //Dereference
            reference operator*();
            const_reference operator*() const;
            pointer operator->();
            const_pointer operator->() const;

            //Navigation
            Iterator& operator++();
            Iterator operator++(int);
            Iterator& operator--();
            Iterator operator--(int);

            // Comparator
            friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
                return lhs._node == rhs._node;
            }
            friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
                return lhs._node != rhs._node;
            }
        private:
            struct Node* _node;
            struct Node* _beforeEnd;
            friend class List<T>;
        };
    private:
        struct Node
        {
            T value;
            Node* prev;
            Node* next;
        };

        Node* _head;
        Node* _tail;
        size_type _size;

        void clearList();
    };

}

#include "FieaGameEngine/List.inl"

