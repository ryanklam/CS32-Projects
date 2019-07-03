//
//  Genome.cpp
//  CS32 Project 4
//
//  Created by Ryan Lam on 3/10/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name;
    string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence) // runs in O(S), S = length of longer string
{
    m_name = nm;
    m_sequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) // runs in O(N), N = num characters in file
{
    genomes.clear();
    string name;
    string sequence;
    string s;
    bool firstline = true;
    while(getline(genomeSource, s))
    {
        if(s[0] != '>' && firstline)
            return false;
        if(s.empty())
            return false;
        
        for(int i = 0; i < s.size(); i++)
        {
            if(s[i] == '>')
            {
                if(!firstline)
                {
                    if(sequence.empty())
                        return false;
                    Genome g(name, sequence);
                    genomes.push_back(g);
                    sequence = "";
                }
                name = s.substr(1);
                if(name.empty())
                    return false;
                firstline = false;
                break;
            }
            
            if(!firstline)
            {
                char nt = toupper(s[i]);
                if(nt != 'A' && nt != 'C' && nt != 'T' && nt != 'G' && nt != 'N')
                    return false;
                sequence += nt;
            }
        }
    }
    getline(genomeSource, name);
    getline(genomeSource, sequence);
    Genome g(name, sequence);
    genomes.push_back(g);
    return true;
}

int GenomeImpl::length() const // RUNS IN CONSTANT TIME O(1)
{
    return m_sequence.size();
}

string GenomeImpl::name() const // runs in O(S), S = length of name
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const  // RUNS IN LINEAR TIME O(S)
{
    if(position + length > m_sequence.size())
        return false;
    fragment = m_sequence.substr(position, length);
    return true; 
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
