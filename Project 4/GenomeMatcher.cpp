//
//  GenomeMatcher.cpp
//  CS32 Project 4
//
//  Created by Ryan Lam on 3/10/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;

private:
    int m_minSearchLength;
    vector<Genome> m_genomes;
    
    struct GenomeFrag
    {
        int genomeNum;
        int genomePos;
    };
    
    Trie<GenomeFrag> m_trie;
    
};

bool operator<(const GenomeMatch& a, const GenomeMatch& b)
{
    if(a.percentMatch < b.percentMatch)
        return true;
    return false;
}

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength) // RUNS IN CONSTANT TIME O(1)
{
    m_minSearchLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome) // runs in O(L*N), L = numsearchlength, N = length of sequence
{
    m_genomes.push_back(genome);
    int i = 0;
    string fragment;
    while(genome.extract(i, minimumSearchLength(), fragment))
    {
        GenomeFrag a;
        a.genomeNum = m_genomes.size();
        a.genomePos = i;
        m_trie.insert(fragment, a);
        i++;
    }
}


int GenomeMatcherImpl::minimumSearchLength() const  // RUNS IN CONSTANT TIME O(1)
{
    return m_minSearchLength;
}


// runs in O(H*F), H = num of distinct matches in genomes, F = length of fragment 
bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    if(fragment.size() < minimumLength || minimumLength < minimumSearchLength())
        return false;
    
    string newFragment = fragment.substr(0, minimumSearchLength()); // resize fragment to fit Trie design
    string restOfFrag = fragment.substr(minimumSearchLength());
     
    if(!exactMatchOnly)
    {
        vector<GenomeFrag> potential2 = m_trie.find(newFragment, false); // call find() on portion of fragment
    
        for(vector<GenomeFrag>::iterator it = potential2.begin(); it != potential2.end(); it++) // iterate thru potential matches
        {
            Genome cur = m_genomes[(*it).genomeNum - 1]; // get genome that the current sequence came from
            string restOfSeq2;
            cur.extract((*it).genomePos + minimumSearchLength(), cur.length() - (*it).genomePos - minimumSearchLength(), restOfSeq2); // extract rest of genome sequence
            int addtlMatch = 0;
            while(restOfFrag[addtlMatch] == restOfSeq2[addtlMatch] && restOfFrag.size() != 0)
            {
                addtlMatch++;
                if(addtlMatch >= restOfFrag.size() || addtlMatch >= restOfSeq2.size()) // if we reach end of fragment or sequence
                    break;
            }
            if(minimumSearchLength() + addtlMatch >= minimumLength) // if we have reached or exceeded minlength in matches
            {
                bool exists = false;
                for(int j = 0; j < matches.size(); j++)
                {
                    if(matches[j].genomeName == cur.name())
                    {
                        exists = true; // automatically ensures that only first genome of same length is entered
                        if(minimumSearchLength()+addtlMatch > matches[j].length) // check if cur is the longest match of genome
                        {
                            DNAMatch d;
                            d.genomeName = cur.name();
                            d.position = (*it).genomePos;
                            d.length = minimumSearchLength() + addtlMatch;
                            matches[j] = d;
                            break;
                        }
                        break;
                    }
                }
                if(!exists) // first instance of this genome, so add automatically
                {
                    DNAMatch d;
                    d.genomeName = cur.name();
                    d.position = (*it).genomePos;
                    d.length = minimumSearchLength() + addtlMatch;
                    matches.push_back(d);
                }
            }
        }
    }

    vector<GenomeFrag> potential = m_trie.find(newFragment, true); // call find() on portion of fragment
    for(vector<GenomeFrag>::iterator it = potential.begin(); it != potential.end(); it++) // iterate thru potential matches
    {
        Genome cur = m_genomes[(*it).genomeNum - 1]; // get genome that the current sequence came from
        string restOfSeq;
        cur.extract((*it).genomePos + minimumSearchLength(), cur.length() - (*it).genomePos - minimumSearchLength(), restOfSeq); // extract rest of genome sequence
        int addtlMatch = 0;
        while(restOfFrag[addtlMatch] == restOfSeq[addtlMatch] && restOfFrag.size() != 0)
        {
            addtlMatch++;
            if(addtlMatch >= restOfFrag.size() || addtlMatch >= restOfSeq.size()) // if we reach end of fragment or sequence
                break;
        }
        if(minimumSearchLength() + addtlMatch >= minimumLength) // if we have reached or exceeded minlength in matches
        {
            bool exists = false;
            for(int j = 0; j < matches.size(); j++)
            {
                if(matches[j].genomeName == cur.name())
                {
                    exists = true; // automatically ensures that only first genome of same length is entered
                    if(minimumSearchLength()+addtlMatch > matches[j].length) // check if cur is the longest match of genome
                    {
                        DNAMatch d;
                        d.genomeName = cur.name();
                        d.position = (*it).genomePos;
                        d.length = minimumSearchLength() + addtlMatch;
                        matches[j] = d;
                        break;
                    }
                    break;
                }
            }
            if(!exists) // first instance of this genome, so add automatically
            {
                DNAMatch d;
                d.genomeName = cur.name();
                d.position = (*it).genomePos;
                d.length = minimumSearchLength() + addtlMatch;
                matches.push_back(d);
            }
        }
        if(!exactMatchOnly) // we check for snips
        {
            bool usedmismatch = false;
            int addtlMatch = 0;
            while((restOfFrag[addtlMatch] == restOfSeq[addtlMatch] || (restOfFrag[addtlMatch] != restOfSeq[addtlMatch] && !usedmismatch)) && restOfFrag.size() != 0)
            {
                if(restOfFrag[addtlMatch] != restOfSeq[addtlMatch]) // we use our mismatch
                    usedmismatch = true;
                addtlMatch++;
                if(addtlMatch >= restOfFrag.size() || addtlMatch >= restOfSeq.size()) // if we reach end of fragment or sequence
                    break;
            }
            if(minimumSearchLength() + addtlMatch >= minimumLength) // if we have reached or exceeded minlength in matches
            {
                bool exists = false;
                for(int j = 0; j < matches.size(); j++)
                {
                    if(matches[j].genomeName == cur.name())
                    {
                        exists = true; // automatically ensures that only first genome of same length is entered
                        if(minimumSearchLength()+addtlMatch > matches[j].length) // check if cur is the longest match of genome
                        {
                            DNAMatch d;
                            d.genomeName = cur.name();
                            d.position = (*it).genomePos;
                            d.length = minimumSearchLength() + addtlMatch;
                            matches[j] = d;
                            break;
                        }
                        break;
                    }
                }
                if(!exists) // first instance of this genome, so add automatically
                {
                    DNAMatch d;
                    d.genomeName = cur.name();
                    d.position = (*it).genomePos;
                    d.length = minimumSearchLength() + addtlMatch;
                    matches.push_back(d);
                }
            }
        }
    }
    
    if(matches.empty())
        return false;
    return true;
}

// runs in O(Q*X), Q = length of query, X = big-O of findGenomesWithThisDNA
bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if(fragmentMatchLength < minimumSearchLength())
        return false;

    map<string, double> matchcounter; // map to record num of matches for each genome
    for(int j = 0; j < m_genomes.size(); j++)
        matchcounter.insert(pair<string, double>(m_genomes[j].name(), 0));
    
    int numSeq = query.length() / fragmentMatchLength; // number of sequences from query
    
    for(int i = 0; i < numSeq; i++)
    {
        string sequence;
        query.extract(i*fragmentMatchLength, fragmentMatchLength, sequence);
        vector<DNAMatch> matches;
        findGenomesWithThisDNA(sequence, fragmentMatchLength, exactMatchOnly, matches); // find genomes that match query sequence

        for(int k = 0; k < matches.size(); k++)
        {
            matchcounter[matches[k].genomeName]++;
        }
    }
    
    for(map<string, double>::iterator it = matchcounter.begin(); it != matchcounter.end(); it++)
    {
        double percentmatch = ((*it).second / numSeq)*100;
        if(percentmatch > matchPercentThreshold) // if genome matches more than percent threshold of sequences
        {
            GenomeMatch g;
            g.genomeName = (*it).first;
            g.percentMatch = percentmatch;
            results.push_back(g);
        }
    }
    
    if(results.empty())
        return false;
    sort(results.begin(), results.end()); // sort by ascending percentage order
    reverse(results.begin(), results.end()); // reverse to sort by descending percentage order
    for(int a = 0; a < results.size() - 1; a++) // break ties by sorting by string alphabetical order
    {
        if(results[a].percentMatch == results[a+1].percentMatch) // size 5, 0 1 2 3 4
        {
            string temp1 = "", temp2 = "";
            for(int ab = 0; ab < results[a].genomeName.size(); ab++)
                temp1 += tolower(results[a].genomeName[ab]);
            for(int ac = 0; ac < results[a+1].genomeName.size(); ac++)
                temp2 += tolower(results[a+1].genomeName[ac]);
            if(temp1.compare(temp2) > 0)
            {
                GenomeMatch temp = results[a];
                results[a] = results[a+1];
                results[a+1] = temp;
            }
        }
    }
    
    return true;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}

