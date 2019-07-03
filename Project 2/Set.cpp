//
//  Set.cpp
//  CS32 Project 2
//
//  Created by Ryan Lam on 1/24/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#include "Set.h"
#include <iostream>

Set::Set() // Construct empty set
{
    m_size = 0;
    head = nullptr;
    tail = nullptr;
}

Set::~Set() // Destruct set
{
    Node* n = head;
    while(n != nullptr)
    {
        Node* temp = n->next; // Prevents loss of pointers in n when n is deleted
        delete n;
        n = temp;
    }
}

Set::Set(const Set& other) // Copy constructor
{
    m_size = other.m_size;
    Node* temp = other.head;
    Node* current = head;
    while(temp != nullptr)
    {
        if(temp == other.head) // To construct head node of set
        {
            Node* a = new Node;
            head = a;
            head->next = nullptr;
            head->prev = nullptr;
            head->value = temp->value;
            current = a;
            temp = temp->next; // Increment temp
        }
        else // To construct all subsequent nodes
        {
            Node* b = new Node;
            b->value = temp->value;
            b->prev = current;
            b->prev->next = b;
            b->next = nullptr;
            current = b;
            temp = temp->next; // Increment temp
        }
    }
    tail = current; // Set tail to the last node in the set
}

Set& Set::operator=(const Set &other) // Assignment operator
{
    if(&other == this) // Check if assignment operators used on same set
        return *this;
    Node* current = head;
    while(current != nullptr)
    {
        Node* temp = current->next; // Clear this set
        delete current;
        current = temp;
    }
    m_size = other.size();
    head = nullptr;
    tail = nullptr;
    Node* temp = other.head;
    current = head;
    while(temp != nullptr) // To construct head node of set
    {
        if(temp == other.head)
        {
            Node* a = new Node;
            head = a;
            head->next = nullptr;
            head->prev = nullptr;
            head->value = temp->value;
            current = a;
            temp = temp->next;
        }
        else // To construct all subsequent nodes
        {
            Node* b = new Node;
            b->value = temp->value;   
            b->prev = current;
            b->prev->next = b;
            b->next = nullptr;
            current = b;
            temp = temp->next;
        }
    }
    tail = current; // Set tail to last node in set
    return *this; // Returns copied set
}

bool Set::empty() const // Checks if set is empty
{
    return m_size == 0;
}

int Set::size() const // Returns size of set
{
    return m_size;
}

bool Set::insert(const ItemType& value) // Insert node into set with specified value
{
    if(head == nullptr) // If set is empty
    {
        head = new Node;
        head->value = value;
        head->next = nullptr;
        head->prev = nullptr;
        tail = head;
        m_size++;
        return true;
    }
    Node* p = head;
    while(p != nullptr)
    {
        if(p->value == value) // Check if value is already in set
            return false;
        p = p->next;
    }
    Node* n = new Node; // Add node with value into set
    n->value = value;
    n->next = nullptr;
    tail->next = n;
    n->prev = tail;
    tail = n;
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value) // Erase node in set with specified value
{
    if(head == nullptr) // Check if set is empty
        return false;
    Node* p = head;
    while(p != nullptr)
    {
        if(p->value == value)
        {
            if(p == head) // Erase head node
            {
                Node* temp = head;
                head = p->next;
                delete temp;
                m_size--;
                return true;
            }
            else if(p == tail) // Erase tail node
            {
                Node* temp = tail;
                p->prev->next = nullptr;
                tail = p->prev;
                delete temp;
                m_size--;
                return true;
            }
            else // Erase any other node
            {
                Node* kill = p;
                p->prev->next = p->next;
                p->next->prev = p->prev;
                delete kill;
                m_size--;
                return true;
            }
        }
        p = p->next; // Increment p
    }
    return false;
}

bool Set::contains(const ItemType& value) const // Check if set contains specified value
{
    Node* p = head;
    while(p != nullptr)
    {
        if(p->value == value)
            return true;
        p = p->next; // Increment p
    }
    return false;
}

bool Set::get(int pos, ItemType& value) const // Get value in set greater than pos values
{
    if(pos < 0 or pos > m_size) // Check if pos is invalid
        return false;
    Node* current = head;
    Node* runner = head;
    int numGreaterThan = 0;
    for(int i = 0; i < m_size; i++)
    {
        while(runner != nullptr)
        {
            if(current->value > runner->value) // Check if current value is greater than runner value
                numGreaterThan++;
            runner = runner->next; // Increment runner
        }
        if(numGreaterThan == pos) // Check if current value is greater than pos values
        {
            value = current->value; // Set value to current value
            return true;
        }

        numGreaterThan = 0; // Reset numGreaterThan
        runner = head; // Reset runner pointer
        current = current->next; // Increment current
    }
    return false;
}

void Set::swap(Set& other) // Swap two sets
{
    Node *headTemp = head; // Swap head pointers
    head = other.head;
    other.head = headTemp;
    
    Node *tailTemp = tail; // Swap tail pointers
    tail = other.tail;
    other.tail = tailTemp;
    
    int sizeTemp = m_size; // Swap set sizes
    m_size = other.size();
    other.m_size = sizeTemp;
}

/*
void Set::dump() const
{
    std::cerr << "Size: " << m_size << std::endl;
    if(head == nullptr)
        std::cerr << "Empty" << std::endl;
    else
    {
        std::cerr <<  head->value << "  <- Head" << std::endl;
        Node* a = head->next;
        while(a != nullptr and a != tail)
        {
            std::cerr << a->value << std::endl;
            a = a->next;
        }
        std::cerr << tail->value << "  <- Tail" << std::endl;
        std::cerr << std::endl;
    }
}
 */


void unite(const Set& s1, const Set& s2, Set& result) // Result contains values in s1, s2, or both
{
    result = s1; // Set result to s1
    for(int i = 0; i < s2.size(); i++)
    {
        ItemType x;
        s2.get(i, x);
        if(!result.contains(x)) // Check if value in s2 is not contained in result
        {
            result.insert(x); // Add value into result
        }
    }
}

void subtract(const Set& s1, const Set& s2, Set& result) // Result contains values in s1 but not in s2
{
    result = s1; // Set result to s1
    for(int i = 0; i < s2.size(); i++)
    {
        ItemType x;
        s2.get(i, x);
        if(result.contains(x)) // Check if value in s2 is contained in result
        {
            result.erase(x); // Erase value from result
            i--; // Decrement for loop index to account for decreased result set size
        }
    }
}
