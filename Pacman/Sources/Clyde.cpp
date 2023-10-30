#include "Clyde.h"
#include "Player.h"
#include "Engine.h"
#include "Level.h"
#include "Log.h"
#include "AStar.h"
#include "MathUtils.h"

Clyde::Clyde() : Clyde(0, 0, nullptr)
{
}

Clyde::Clyde(Player* player) : Clyde(0, 0, player)
{
}

Clyde::Clyde(int x, int y, Player* player) : Ghost(x, y, player)
{
}

void Clyde::Initialize()
{
    m_animation.Init("Assets/Images/clyde.png", 8, 32, 32);
    Ghost::Initialize();
}

void Clyde::Start()
{
    ChooseNextAction();
}

void Clyde::ExecuteBehaviour()
{
    // Clyde uses random directions in the maze
    if (m_playerPtr)
    {
        int px, py;
        m_playerPtr->GetPosition(&px, &py);

        std::vector<std::pair<EEntityDirection, int>> availableDirections;
        if (CanMove(EEntityDirection::MOVING_RIGHT) && m_direction != EEntityDirection::MOVING_LEFT)
        {
            availableDirections.emplace_back(EEntityDirection::MOVING_RIGHT,
                Engine::Heuristic(m_localX + 1, m_localY, px, py)
            );
        }

        if (CanMove(EEntityDirection::MOVING_LEFT) && m_direction != EEntityDirection::MOVING_RIGHT)
        {
            availableDirections.emplace_back(EEntityDirection::MOVING_LEFT,
                Engine::Heuristic(m_localX - 1, m_localY, px, py)
            );
        }

        if (CanMove(EEntityDirection::MOVING_UP) && m_direction != EEntityDirection::MOVING_DOWN)
        {
            availableDirections.emplace_back(EEntityDirection::MOVING_UP,
                Engine::Heuristic(m_localX, m_localY - 1, px, py)
            );
        }

        if (CanMove(EEntityDirection::MOVING_DOWN) && m_direction != EEntityDirection::MOVING_UP)
        {
            // Don't go down in the middle when not dead
            if (!(m_localX == 10 && m_localY == 10))
            {
                availableDirections.emplace_back(EEntityDirection::MOVING_DOWN,
                    Engine::Heuristic(m_localX, m_localY + 1, px, py)
                );
            }
        }

        if (!availableDirections.empty())
        {
            int nextAction = Engine::RandRange(0, static_cast<int>(availableDirections.size()) - 1);
            SetupInterpolation(availableDirections[nextAction].first);
            SetDirection(availableDirections[nextAction].first);
            SetAnimation(m_direction);
        }
    }
}
