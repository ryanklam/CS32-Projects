//
//  History.cpp
//  CS32 Project 1
//
//  Created by Ryan Lam on 1/9/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#include "History.h"

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            m_grid[r][c] = '.';
}



bool History::record(int r, int c)
{
    if(r > m_rows or r < 1)
        return false;
    else if(c > m_cols or c < 1)
        return false;
    char& gridChar = m_grid[r-1][c-1];
    switch(gridChar)
    {
        case '.':
            gridChar = 'A';
            break;
        case 'Z':
            break;
        default:
            gridChar++;
            break;
    }
    return true;
}

void History::display() const
{
    clearScreen();
    int r, c;
    
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << m_grid[r][c];
        cout << endl;
    }
    cout << endl;
}
