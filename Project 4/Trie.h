//
//  Trie.hpp
//  CS32 Project 4
//
//  Created by Ryan Lam on 3/10/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
using namespace std;

template<typename ValueType>
class Trie
{
public:
    Trie() // runs in O(1)
    {
        root = new Node;
    }
    
    ~Trie() // runs in O(N), N = num of nodes
    {
        deleteTrie(root);
        delete root;
    }
    
    void reset() // runs in O(N), N = num of nodes
    {
        ~Trie();
        root = new Node;
    }
    
    void insert(const string& key, const ValueType& value) // RUNS IN O(L*C), TIME L=LENGTH OF KEY, C=AVG NUM OF CHILDREN
    {
        string keyCopy = key;
        insertHelper(keyCopy, value, root);
    }
    
    // O(LC+V) if exactmatch true, O(L^2C^2+V) if false, L = length of key, C = avg num of children,  V = size of returned vector
    vector<ValueType> find(const string& key, bool exactMatchOnly) const
    {
        vector<ValueType> result;
        typename vector<Node*>::iterator it = root->children.begin();
        while(it != root->children.end())
        {
            if((*it)->label == key[0])
            {
                result = findHelper(key.substr(1), exactMatchOnly, (*it));
                break;
            }
            it++;
        }
        return result;
    }
    
    // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    
private:
    struct Node {
        char label;
        vector<Node*> children;
        vector<ValueType> values;
    };
    
    Node* root;
    
    void deleteTrie(Node* root) // runs in O(N), N = num of nodes
    {
        if(root->children.empty())
            return;
        typename vector<Node*>::iterator it;
        it = root->children.begin();
        while(it != root->children.end())
        {
            deleteTrie((*it));
            delete (*it);
            it = root->children.erase(it);
        }
    }
    
    
    void insertHelper(string key, ValueType value, Node* root) // RUNS IN O(L*C), L=LENGTH OF KEY, C=AVG NUM OF CHILDREN
    {
        if(key.size() == 0)
        {
            root->values.push_back(value);
            return;
        }
        typename vector<Node*>::iterator it;
        for(it = root->children.begin(); it != root->children.end(); it++)
        {
            if((*it)->label == key[0])
            {
                insertHelper(key.substr(1), value, (*it));
                break;
            }
        }
        if(it == root->children.end())
        {
            root->children.push_back(new Node);
            root->children.back()->label = key[0];
            insertHelper(key.substr(1), value, root->children.back());
        }
    }
    
    
    // O(LC+V) if exactmatch true, O(L^2C^2+V) if false, V = size of returned vector, L=LENGTH OF KEY, C=AVG NUM OF CHILDREN
    vector<ValueType> findHelper(string key, bool exactMatch, Node* root) const
    {
        vector<ValueType> result;
        string keyCopy = key;
        if(keyCopy.size() == 0) // We have reached the end of the string, so found exact match
        {
            result.insert(result.end(), root->values.begin(), root->values.end());
            return result;
        }
        typename vector<Node*>::iterator it;
        for(it = root->children.begin(); it != root->children.end(); it++)
        {
            if((*it)->label == key[0]) // If we find a matching node to our key
            {
                vector<ValueType> temp = findHelper(keyCopy.substr(1), exactMatch, (*it));
                result.insert(result.end(), temp.begin(), temp.end());
            }
            else if(!exactMatch) // If we find a snip, use our mismatch
            {
                vector<ValueType> temp = findHelper(keyCopy.substr(1), !exactMatch, (*it));
                result.insert(result.end(), temp.begin(), temp.end());
            }
        }
        return result;
    }
};

#endif // TRIE_INCLUDED
