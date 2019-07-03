//
//  Set.h
//  CS32 Project 2
//
//  Created by Ryan Lam on 1/24/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#ifndef Set_hpp
#define Set_hpp

#include <string>

using ItemType = std::string;

class Set
{
    public:
        Set();
        ~Set();
        Set(const Set& other);
        Set &operator=(const Set &other);
        bool empty() const;
        int size() const;
        bool insert(const ItemType& value);
        bool erase(const ItemType& value);
        bool contains(const ItemType& value) const;
        bool get(int pos, ItemType& value) const;
        void swap(Set& other);
        //void dump() const;
    private:
        int m_size;
        struct Node {
            ItemType value;
            Node* prev;
            Node* next;
        };
        Node* head;
        Node* tail;
    
};

void unite(const Set& s1, const Set& s2, Set& result);

void subtract(const Set& s1, const Set& s2, Set& result);


#endif /* Set_h */
