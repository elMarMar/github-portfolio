#include "Player.h"
#include <iostream>
#include "Arena.h" // Access Arena rows() and cols()

using namespace std;

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

string Player::takeComputerChosenTurn()
{
    //   If moving in some direction would put me in less immediate danger
    //     than standing, then move in that direction.
    //   else shoot in the direction of the nearest robot I can hit.

    bool moveTaken = false;

    bool inImmediateDanger, dangerUp, dangerDown, dangerLeft, dangerRight;
    inImmediateDanger = dangerUp = dangerDown = dangerLeft = dangerRight = false;

    if (m_row > 1 && m_arena->nRobotsAt(m_row - 1, m_col) > 0) dangerUp = true;
    if (m_row < m_arena->rows() && m_arena->nRobotsAt(m_row + 1, m_col) > 0) dangerDown = true;
    if (m_col > 1 && m_arena->nRobotsAt(m_row, m_col - 1) > 0) dangerLeft = true;
    if (m_col < m_arena->cols() && m_arena->nRobotsAt(m_row, m_col + 1) > 0) dangerRight = true;

    if (dangerUp || dangerDown || dangerLeft || dangerRight) inImmediateDanger = true;

    if (inImmediateDanger) {
        moveTaken = true;

        if (!dangerUp && m_row > 1) move(UP);
        else if (!dangerDown && m_row < m_arena->rows()) move(DOWN);
        else if (!dangerLeft && m_col > 1) move(LEFT);
        else if (!dangerRight && m_col < m_arena->cols()) move(RIGHT);

        else
            moveTaken = false;

        if (moveTaken)
            return "Moved.";
    }

    //Shoot Nearest Robot
    moveTaken = false;
    bool hitRobot = false;

    for (int target = 1; target <= 5; target++) {
        int positiveOffset = target;
        int negativeOffset = (-1) * target;

        if (m_arena->nRobotsAt(m_row + negativeOffset, m_col) > 0) {
            if (shoot(UP)) hitRobot = true;
            moveTaken = true;
            break;
        }
        else if (m_arena->nRobotsAt(m_row + positiveOffset, m_col) > 0) {
            if (shoot(DOWN)) hitRobot = true;
            moveTaken = true;
            break;
        }
        else if (m_arena->nRobotsAt(m_row, m_col + negativeOffset) > 0) {
            if (shoot(LEFT)) hitRobot = true;
            moveTaken = true;
            break;
        }
        else if (m_arena->nRobotsAt(m_row, m_col + positiveOffset) > 0) {
            if (shoot(RIGHT)) hitRobot = true;
            moveTaken = true;
            break;
        }
    }

    if (moveTaken == true && hitRobot) return "Shot and hit!";
    else if (moveTaken == true && !hitRobot) return "Shot and missed!";
    else {
        stand();
        return "Stood.";
    }



}

void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    m_age++;


    switch (dir)
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

bool Player::shoot(int dir)
{
    m_age++;

    if (rand() % 3 == 0)  // miss with 1/3 probability
        return false;

    // Damage the nearest robot in direction dir, returning
    // true if a robot is hit and damaged, false if not hit.

    for (int i = 1; i <= 5; i++) {
        if (dir == UP) {
            int upTargetCoord = m_row - i;
            if (upTargetCoord > 0) {
                if ((m_arena->nRobotsAt(upTargetCoord, m_col)) > 0) {
                    m_arena->damageRobotAt(upTargetCoord, m_col);
                    return true;

                }
            }
        }
        else if (dir == RIGHT) {
            int rightTargetCoord = m_col + i;
            if (rightTargetCoord <= m_arena->cols()) {
                if ((m_arena->nRobotsAt(m_row, rightTargetCoord)) > 0) {
                    m_arena->damageRobotAt(m_row, rightTargetCoord);
                    return true;

                }
            }
        }
        else if (dir == DOWN) {
            int downTargetCoord = m_row + i;
            if (downTargetCoord <= m_arena->rows()) {
                if ((m_arena->nRobotsAt(downTargetCoord, m_col)) > 0) {
                    m_arena->damageRobotAt(downTargetCoord, m_col);
                    return true;

                }
            }
        }
        else if (dir == LEFT) {
            int leftTargetCoord = m_col - i;
            if (leftTargetCoord > 0) {
                if ((m_arena->nRobotsAt(m_row, leftTargetCoord)) > 0) {
                    m_arena->damageRobotAt(m_row, leftTargetCoord);
                    return true;

                }
            }
        }
    }
    return false;
}



bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}