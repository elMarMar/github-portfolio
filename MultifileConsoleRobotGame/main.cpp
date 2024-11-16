#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Game.h"

using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    // Create a game
    // Use this instead to create a mini-game:  
    //Game g(3, 3, 5);
    Game g(15, 18, 80);
    // Play the game
    g.play();

}