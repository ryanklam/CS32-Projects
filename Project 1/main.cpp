//
//  main.cpp
//  CS32 Project 1
//
//  Created by Ryan Lam on 1/9/19.
//  Copyright © 2019 Ryan Lam. All rights reserved.
//

#include "Game.h"

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(4, 4, 4);
    
    // Play the game
    g.play();
}


