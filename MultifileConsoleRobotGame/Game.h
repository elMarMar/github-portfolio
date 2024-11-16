#ifndef GAME_H
#define GAME_H

class Arena; // We only point to an arena so ONLY a forward declaration is required

class Game {
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

    // Mutators
    void play();

private:
    Arena* m_arena;
};

#endif