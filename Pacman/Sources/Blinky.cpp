#include "Blinky.h"
#include "Player.h"
#include "Engine.h"
#include "Level.h"
#include "Log.h"
#include "AStar.h"
#include "MathUtils.h"



Blinky::Blinky() : Blinky(0, 0, nullptr)
{
}

Blinky::Blinky(Player* player) : Blinky(0, 0, player)
{
}

Blinky::Blinky(int x, int y, Player* player) : Ghost(x, y, player)
{
}

void Blinky::Initialize()
{
    m_animation.Init("Assets/Images/blinky.png", 8, 32, 32);
    Ghost::Initialize();
}

void Blinky::Start()
{
    SetupInterpolation(EEntityDirection::MOVING_RIGHT);
    SetDirection(EEntityDirection::MOVING_RIGHT);
    SetAnimation(m_direction);
}

void Blinky::ChooseNextAction()
{
    // Blinky's main objective is to directly pursue Pac-Man.
    if (m_playerPtr)
    {
        int px, py;
        m_playerPtr->GetPosition(&px, &py);

        std::pair<EEntityDirection, int> selection = { EEntityDirection::MOVING_IDLE, 100000 };

        if (CanMove(EEntityDirection::MOVING_RIGHT) && m_direction != EEntityDirection::MOVING_LEFT)
        {
            int h = Engine::Heuristic(m_localX + 1, m_localY, px, py);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_RIGHT, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_LEFT) && m_direction != EEntityDirection::MOVING_RIGHT)
        {
            int h = Engine::Heuristic(m_localX - 1, m_localY, px, py);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_LEFT, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_UP) && m_direction != EEntityDirection::MOVING_DOWN)
        {
            int h = Engine::Heuristic(m_localX, m_localY - 1, px, py);
            if (h < selection.second)
            {
                selection = { EEntityDirection::MOVING_UP, h };
            }
        }

        if (CanMove(EEntityDirection::MOVING_DOWN) && m_direction != EEntityDirection::MOVING_UP)
        {
            int h = Engine::Heuristic(m_localX, m_localY + 1, px, py);
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
