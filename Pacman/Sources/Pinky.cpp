#include "Pinky.h"
#include "Player.h"
#include "Engine.h"
#include "AStar.h"

Pinky::Pinky() : Pinky(0, 0, nullptr)
{
}

Pinky::Pinky(Player* player) : Pinky(0, 0, player)
{
}

Pinky::Pinky(int x, int y, Player* player) : Ghost(x, y, player)
{
}

void Pinky::Initialize()
{
    m_animation.Init("Assets/Images/pinky.png", 8, 32, 32);
    Ghost::Initialize();
}

void Pinky::Start()
{
    ChooseNextAction();
}

void Pinky::ChooseNextAction()
{
    // Pinky is known for her unique behavior, which involves trying to position herself
    // in front of Pac-Man. Implementing Pinky's behavior in code can be achieved by calculating
    // a target point that is a few grid spaces in front of Pac-Man's current direction.

    if (m_playerPtr)
    {
        int targetX, targetY;
        m_playerPtr->GetPosition(&targetX, &targetY);

        int dirX, dirY;
        m_playerPtr->GetDirection(&dirX, &dirY);

        if (dirX > 0)
        {
            targetX += 4;
        }
        else if (dirX < 0)
        {
            targetX -= 4;
        }

        if (dirY > 0)
        {
            targetY += 4;
        }
        else if (dirY < 0)
        {
            targetY -= 4;
        }

        std::pair<EEntityDirection, int> selection = { EEntityDirection::MOVING_IDLE, 100000 };

        if (CanMove(EEntityDirection::MOVING_RIGHT) && m_direction != EEntityDirection::MOVING_LEFT)
        {
            int h = Engine::Heuristic(m_localX + 1, m_localY, targetX, targetY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_RIGHT, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_LEFT) && m_direction != EEntityDirection::MOVING_RIGHT)
        {
            int h = Engine::Heuristic(m_localX - 1, m_localY, targetX, targetY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_LEFT, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_UP) && m_direction != EEntityDirection::MOVING_DOWN)
        {
            int h = Engine::Heuristic(m_localX, m_localY - 1, targetX, targetY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_UP, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_DOWN) && m_direction != EEntityDirection::MOVING_UP)
        {
            int h = Engine::Heuristic(m_localX, m_localY + 1, targetX, targetY);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_DOWN, h };
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
