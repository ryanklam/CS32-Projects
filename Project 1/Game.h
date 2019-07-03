//
//  Game.h
//  CS32 Project 1
//
//  Created by Ryan Lam on 1/9/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#include "Arena.h"

#ifndef Game_h
#define Game_h

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};

int decodeDirection(char dir);

#endif

