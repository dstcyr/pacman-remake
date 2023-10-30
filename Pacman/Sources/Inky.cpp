#include "Inky.h"
#include "Blinky.h"
#include "Clyde.h"
#include "Player.h"
#include "AStar.h"

Inky::Inky() : Inky(0, 0, nullptr, nullptr, nullptr)
{
}

Inky::Inky(Player* player) : Inky(0, 0, player, nullptr, nullptr)
{
}

Inky::Inky(int x, int y, Player* player, Blinky* blinky, Clyde* clyde) : Ghost(x, y, player), m_blinky(blinky), m_clyde(clyde)
{
}

void Inky::Initialize()
{
    m_animation.Init("Assets/Images/inky.png", 8, 32, 32);
    Ghost::Initialize();
}

void Inky::Start()
{
    ChooseNextAction();
}

void Inky::ExecuteBehaviour()
{
    // Inky's behavior in the original Pac-Man game is more complex, as it depends
    // on the positions of both Blinky and Clyde. Inky's movement is influenced by
    // the following logic:
    //  - Inky first calculates a target point that is two grid spaces in front of
    //    Pac-Man's current direction. This position is represented as a potential
    //    destination for Inky.
    //
    //  - Inky then considers the current positions of Blinky and Clyde. He draws a
    //    vector from Blinky to Clyde and doubles its length.This extended vector is
    //    used to modify the target point.
    //
    // The result is a new target point for Inky, which is two grid spaces in front
    // of Pac-Man's current direction, but also influenced by the position of Blinky
    // and Clyde.

    if (m_playerPtr)
    {
        int px, py;
        m_playerPtr->GetPosition(&px, &py);

        int bx, by;
        m_blinky->GetPosition(&bx, &by);

        int cx, cy;
        m_clyde->GetPosition(&cx, &cy);

        int vectorX = cx - bx;
        int vectorY = cy - by;
        int targeyX = px + 2 * vectorX;
        int targeyY = py + 2 * vectorY;

        std::pair<EEntityDirection, int> selection = { EEntityDirection::MOVING_IDLE, 100000 };

        if (CanMove(EEntityDirection::MOVING_RIGHT) && m_direction != EEntityDirection::MOVING_LEFT)
        {
            int h = Engine::Heuristic(m_localX + 1, m_localY, targeyX, targeyY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_RIGHT, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_LEFT) && m_direction != EEntityDirection::MOVING_RIGHT)
        {
            int h = Engine::Heuristic(m_localX - 1, m_localY, targeyX, targeyY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_LEFT, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_UP) && m_direction != EEntityDirection::MOVING_DOWN)
        {
            int h = Engine::Heuristic(m_localX, m_localY - 1, targeyX, targeyY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_UP, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_DOWN) && m_direction != EEntityDirection::MOVING_UP)
        {
            // Don't go down in the middle when not dead
            if (!(m_localX == 10 && m_localY == 10))
            {
                int h = Engine::Heuristic(m_localX, m_localY + 1, targeyX, targeyY);
                if (h < selection.second)
                {
                    selection = { EEntityDirection::MOVING_DOWN, h };
                }
            }
        }

        if (selection.first != EEntityDirection::MOVING_IDLE)
        {
            SetupInterpolation(selection.first);
            SetDirection(selection.first);
            SetAnimation(m_direction);
        }
    }
}
