//
//  test.cpp
//  CS32 Project 4
//
//  Created by Ryan Lam on 3/13/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//



 
 #include <stdio.h>
 #include <iostream>
 #include <fstream>
 #include "Trie.h"
 #include "Genome.cpp"
 #include "GenomeMatcher.cpp"
 
 using namespace std;
 
 int main()
 {
 
    /*
     Trie<int> trie;
     trie.insert("GATTACA", 42);        //    GATTACA    à {42}
     trie.insert("GATTACA", 17);        //    GATTACA    à {42,    17}
     trie.insert("GATTACA", 42);        //    GATTACA    à {42,    17,    42}
     trie.insert("GCTTACA", 30);        // GCTTACA à {30}
     trie.insert("KCTTACA", 7);        // GCTTACA à {30}
     trie.insert("GCTTAC", 2);        // GCTTACA à {30}
     trie.insert("GCTTCCA", 3);        // GCTTACA à {30}
     trie.insert("GATTOCA", 100);        // GCTTACA à {30}
     trie.insert("ACTGC", 21);        // GCTTACA à {30}
     trie.insert("ACTGCC", 90);        // GCTTACA à {30}
     
     vector<int> result1 = trie.find("GATTACA", false);
     vector<int>::iterator it = result1.begin();
     cout << "Vector values: ";
     while(it != result1.end())
     {
     cout << (*it) << " ";
     it++;
     }
     cout << endl;
     
     cout << "Vector size: " << result1.size() << endl;
     
     
     */
     
     /*
     string filename = "/Users/ryanlam/Downloads/Gee-nomics/data/Halorubrum_californiense.txt";
     ifstream strm(filename);
     if (!strm)
     cout << "Cannot open " << filename << endl;
     
     vector<Genome> vg;
     bool success = Genome::load(strm, vg);
     if (success)
     {
     cout << "Loaded " << vg.size() << " genomes successfully:" << endl;
     for (int k = 0; k != vg.size(); k++)
     {
     cout << vg[k].name() << endl;
     }
     }
     else
     cout << "Error loading genome data" << endl;
     
     */
     
     
     
     /*
     Genome g("oryx",
     "GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
     
     string f1, f2, f3 = "ashwin";
     cout << g.extract(0, 5, f1) << f1 << endl;     //    result1    =    true,    f1    =    “GCTCG”;
     cout << g.extract(74, 6, f2) << f2 << endl; //    result2 =    true,    f2 =    “CTGGGA”;
     cout << g.extract(74, 7, f3) << f3 << endl; //    result3 =    false,    f3 is    unchanged
     
     
     GenomeMatcherImpl g(3);
     Genome g1("g1", "ACTG");
     Genome g2("g2", "TCGACT");
     Genome g3("g3", "TCTCG");
     g.addGenome(g1);
     g.addGenome(g2);
     g.addGenome(g3);
     
     vector<string> a = g.m_trie.find("CTG", true);
     vector<string>::iterator it = a.begin();
     cout << "Vector values: ";
     while(it != a.end())
     {
     cout << (*it) << " ";
     it++;
     }
     cout << endl;
     
     cout << "Vector size: " << a.size() << endl;
     */
     
     
     ///*
     GenomeMatcherImpl g(4);
     Genome g1("g1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
     Genome g2("g2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
     Genome g3("g3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");
     //               01234567890123456789012345678901234567890123456789012345678901234567890123456789
     //                         1         2         3         4         5         6         7
     
     g.addGenome(g1);
     g.addGenome(g2);
     g.addGenome(g3);
     
     vector<DNAMatch> matches;
     
     //cout << g.findGenomesWithThisDNA("GAAG", 4, true, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAATAC", 4, true, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAATAC", 6, true, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAATAC", 6, false, matches) << endl; // works
     cout << g.findGenomesWithThisDNA("GTATAT", 6, false, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAATACG", 6, false, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAAGGGTT", 5, false, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAAGGGTT", 6, false, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCC", 12, false, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCG", 12, false, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAAG", 5, true, matches) << endl; // works
     //cout << g.findGenomesWithThisDNA("GAAG", 3, true, matches) << endl; // works
     
     for(vector<DNAMatch>::iterator it = matches.begin(); it != matches.end(); it++)
     {
         cout << (*it).genomeName << " " << (*it).length << " " << (*it).position << endl;
     }
      //*/
     
     /*
    GenomeMatcherImpl g(4);
     Genome g1("sas", "GGGGTTTTAAAACCCCACGTACGTACGTNANANANA");
     g.addGenome(g1);
     Genome g2("a", "AAATCCCTGGGGTTTTNANA");
     vector<GenomeMatch> results;
     g.findRelatedGenomes(g2, 8, false, 20.0, results);
     for(int i = 0; i < results.size(); i++)
         cout << results[i].percentMatch;
      */
      
 }
 

