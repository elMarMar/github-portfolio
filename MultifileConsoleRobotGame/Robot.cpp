#include "Robot.h"

#include <iostream> 
#include "Arena.h" 
#include "globals.h"    
using namespace std;

Robot::Robot(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A robot must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "***** Robot created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;

    health = 2;

}

int Robot::row() const
{
    return m_row;
}

int Robot::col() const
{
    return m_col;
}

void Robot::move()
{
    // Attempt to move in a random direction; if we can't move, don't move
    switch (rand() % 4)
    {
    case UP:
        if (m_row > 1) {
            m_row--;
        }
        break;
    case DOWN:
        if (m_row < m_arena->rows()) {
            m_row++;
        }
        break;
    case LEFT:
        if (m_col > 1) {
            m_col--;
        }
        break;
    case RIGHT:
        if (m_col < m_arena->cols()) {
            m_col++;
        }
        break;
    }
}

bool Robot::takeDamageAndLive()
{
    // If the robot has been hit once before, return false (since a
    // second hit kills a robot).  Otherwise, return true (since the robot
    // survived the damage).
    health--;
    if (health > 0) {
        return true;
    }

    else {
        return false;
    }
}
