#include "Arena.h"
#include <iostream>
#include "Player.h" // for call: m_player -> row()
#include "Robot.h"  // to access row() and col()

using namespace std;

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena()
{
    delete m_player;
    for (int i = 0; i < m_nRobots; i++) {
        delete m_robots[i];
    }
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
    // Check if Coordinates are in Bounds
    if (r < 1 || r > m_rows || c < 1 || c > m_cols) return 0;

    // Return the number of robots at row r, column c.
    int totalRobotsAtPosition = 0;

    for (int i = 0; i < m_nRobots; i++) {
        if ((*m_robots[i]).row() == r && (*m_robots[i]).col() == c)
            totalRobotsAtPosition++;
    }

    return totalRobotsAtPosition;
}

void Arena::display(string msg) const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;


    // Indicate each robot's position
    for (int r = 0; r < rows(); r++) {

        for (int c = 0; c < cols(); c++) {

            int numRobotsAtPosition = nRobotsAt(r + 1, c + 1); // +1 and +1 b/c 2D Array Index and row, col index are diff


            // Fill the grid with dots
            // Position (row,col) in the arena coordinate system is represented in
            // the array element grid[row-1][col-1]
            grid[r][c] = '.';


            if (numRobotsAtPosition == 1)
                grid[r][c] = 'R';
            else if (numRobotsAtPosition >= 2 && numRobotsAtPosition <= 8)
                grid[r][c] = '0' + numRobotsAtPosition; // NEED TO ADD ZERO TO PRINT numRobotsAtPosition as a char!
            else if (numRobotsAtPosition >= 9)
                grid[r][c] = '9';
        }
    }

    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a robot there,
        // in which case set it to '*'.
        char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, robot, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;

    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c)
{
    // If MAXROBOTS have already been added, return false.  Otherwise,
    // dynamically allocate a new robot at coordinates (r,c).  Save the
    // pointer to the newly allocated robot and return true.

    if (m_nRobots < MAXROBOTS) {
        m_robots[m_nRobots] = new Robot(this, r, c);
        m_nRobots++;
        return true;
    }

    else
        return false;
}

bool Arena::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

void Arena::damageRobotAt(int r, int c)
{
    // Damage one robot at row r, column c if at least one is there.
    // If the robot does not survive the damage, destroy it.
    if (nRobotsAt(r, c) > 0) { // Make sure a robot IS there
        int damagedRobot = 0;
        for (int i = 0; i < m_nRobots; i++) {
            if (m_robots[i]->row() == r && m_robots[i]->col() == c) {

                if (!(m_robots[i]->takeDamageAndLive())) {
                    delete m_robots[i];

                    m_robots[i] = m_robots[m_nRobots - 1];

                    m_robots[m_nRobots - 1] = nullptr; // So We can delete it if another robot replaces it in addRobot
                    // No memory leak b/c now m_robots[i] points to this
                    m_nRobots--;
                }
                break;
            }
        }
    }
}

bool Arena::moveRobots()
{
    for (int k = 0; k < m_nRobots; k++)
    {
        //        Have the k-th robot in the arena make one move.
        //        If that move results in that robot being in the same
        //        position as the player, the player dies.
        m_robots[k]->move();
        if (m_robots[k]->row() == m_player->row() && m_robots[k]->col() == m_player->col())
            m_player->setDead();

    }

    // return true if the player is still alive, false otherwise
    return !m_player->isDead();
}