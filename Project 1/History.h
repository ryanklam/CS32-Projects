//
//  History.hpp
//  CS32 Project 1
//
//  Created by Ryan Lam on 1/9/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

class Arena;
#include "globals.h"

#ifndef History_h
#define History_h

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_rows;
    int m_cols;
    char m_grid[MAXROWS][MAXCOLS];
};

#endif
