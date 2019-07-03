//
//  test.cpp
//  CS32 Project 2
//
//  Created by Ryan Lam on 1/24/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//


#include <stdio.h>
#include "Set.h"
#include <cassert>
#include <iostream>



int main() {

     // These tests were performed on sets of strings (ItemType = std::string)
     
     
     // Testing default constructor
    Set ucla;
     
    assert(ucla.empty()); // test empty
    assert(ucla.size() == 0); // test size
    assert(!ucla.erase("xyz")); // make sure nothing can be removed
     
    ucla.insert("ccc"); // test insert
    ucla.insert("aaa");
    assert(!ucla.insert("aaa")); // inserting duplicates should not work
    ucla.insert("bbb");
     
    assert(!ucla.empty()); // make sure set is no longer empty
    assert(ucla.size() == 3); // make sure size is correct
    assert(ucla.erase("ccc")); // test erase
    assert(!ucla.erase("abc")); // test erase with value not in set
    assert(ucla.size() == 2); // test size
     
    ucla.insert("zzz");
     
    ItemType x = "xxx";
    assert(!ucla.get(300, x)  &&  x == "xxx");  // test get with invalid parameter, shouldn't change x
    assert(!ucla.get(3, x)  &&  x == "xxx");  // test get with invalid parameter, shouldn't change x
    assert(ucla.get(1, x)  &&  x == "bbb"); // test get, should change x
    
    
    // Testing swap function
    Set a, b;

    a.insert("Hello");
    a.insert("There");
    a.insert("Buddy");
    a.insert("Boy");
    
    b.insert("bOy");
    b.insert("a");
    b.insert("b");
    b.insert("k");
    b.insert("");
    b.insert("uh huh honey");
    
    a.swap(b);
    assert(b.contains("Buddy") && a.contains("k")); // test swap
    assert(b.size() == 4 && a.size() == 6);
     
    Set rko;
    rko.insert("abra");
     
    Set edc;
    edc.swap(rko); // test swapping an empty set with another set
    assert(edc.size() == 1 && rko.size() == 0);
    
    Set are;
    Set you;
    you.swap(are); // test swapping two empty sets
    assert(are.size() == 0 && you.size() == 0);
     
    
    // Testing copy constructor and assignment operator
    Set ss2;
    ss2.insert("ccc");
    ss2.insert("aaa");
    ss2.insert("bbb");
    
    Set a2 = ss2; // test copy constructor
    assert(a2.contains("aaa") && ss2.contains("bbb") && a2.size() == 3);

    Set b2;
    
    b2.insert("bOy");
    b2.insert("a");
    b2.insert("b");
    b2.insert("k");
    b2.insert("");
    b2.insert("uh huh honey");
     
    a2 = b2; // test assignment operator
    assert(a2.contains("uh huh honey") && a2.size() == 6 && b2.contains("bOy") && b2.size() == 6);
    
    Set ark;
    ark.insert("a");
    ark = ark; // test aliasing in assignment operator
    assert(ark.size() == 1);
    
     
    // Testing unite and subtract functions
    Set j7, c3, d3;
    
    j7.insert("burrito");
    j7.insert("ham");
    j7.insert("cracker");
    
    c3.insert("ab");
    c3.insert("yu");
    c3.insert("er");
    c3.insert("io");
    c3.insert("ad");
    
    d3.insert("ad");
    d3.insert("ca");
    d3.insert("ny");
    d3.insert("ri");
    d3.insert("tx");
    
    // ASSUME THAT FOR EACH CALL OF SUBTRACT, THE SETS ARE AS CONSTRUCTED
    
    subtract(c3, d3, j7); // testing correct usage
    assert(!j7.contains("ad"));
    
    //subtract(j7, j7, j7); // testing aliasing
    //assert(j7.empty());
    
    //subtract(j7, d3, j7); // testing aliasing
    //assert(j7.contains("ham") && j7.size() == 3);
    
    //subtract(c3, j7, j7); // testing aliasing
    //assert(j7.empty());
    
    

    // ASSUME THAT FOR EACH CALL OF UNITE, THE SETS ARE AS CONSTRUCTED

    
    //unite(c3,d3,j7); // testing correct usage
    //assert(j7.size() == 9 && j7.contains("ad"));
    
    //unite(j7, j7, j7); // testing aliasing
    //assert(j7.size() == 3 && j7.contains("ham"));
    
    //unite(j7, d3, j7); // testing aliasing
    //assert(j7.contains("ca") && j7.size() == 8);
    
    //unite(c3, j7, j7); // testing aliasing
    //assert(j7.size() == 5 && j7.contains("io"));
    

     
    
    
    
    
    
/*
    
    
    // These tests were performed on sets of ints (ItemType = int)
    
    Set ss3;
    Set a3, b3;

    ss3.insert(4); // test insert
    ss3.insert(12);
    ss3.insert(6);
    
    assert(!ss3.empty()); // test empty
    assert(ss3.size() == 3); // test size
    assert(ss3.contains(12)); // test contains
    
    a3.insert(2);
    a3.insert(8);
    a3.insert(5);
    a3.insert(9);
    a3.insert(6);
    assert(!a3.empty());
    
    b3.insert(2);
    b3.insert(8);
    b3.insert(3);
    b3.insert(9);
    b3.insert(6);
    
    // ASSUME THAT FOR EACH CALL OF SUBTRACT, THE SETS ARE RECONSTRUCTED
    
    //subtract(a3, b3, ss3);
    //assert(ss3.contains(5));
    
    //subtract(ss3, ss3, ss3);
    //assert(ss3.empty());
    
    //subtract(ss3, b3, ss3);
    //assert(ss3.contains(4) && ss3.contains(12));
    
    //subtract(a3, ss3, ss3);
    //assert(ss3.empty());
    
    //unite(a3,ss3,ss3);
     
    */
    
 }







