#include "Ghost.h"
#include "Level.h"
#include "Engine.h"
#include "AStar.h"

Ghost::Ghost() : Ghost(0, 0, nullptr)
{
}

Ghost::Ghost(Player* player) : Ghost(0, 0, player)
{
}

Ghost::Ghost(int x, int y, Player* player) : Entity(x, y), m_playerPtr(player)
{
}

void Ghost::Initialize()
{
    const float animSpeed = 0.05f;
    m_animation.AddClip("right", 0, 2, animSpeed);
    m_animation.AddClip("idle_right", 0, 1, animSpeed);
    m_animation.AddClip("up", 2, 2, animSpeed);
    m_animation.AddClip("idle_up", 2, 1, animSpeed);
    m_animation.AddClip("left", 4, 2, animSpeed);
    m_animation.AddClip("idle_left", 4, 1, animSpeed);
    m_animation.AddClip("down", 6, 2, animSpeed);
    m_animation.AddClip("idle_down", 6, 1, animSpeed);
    m_animation.Play("idle_left", false);
}

void Ghost::Update(float dt)
{
    m_animation.Update(dt);

    if (!IsStopped())
    {
        UpdateInterpolation(dt);
        if (m_interpTime >= m_interpSpeed)
        {
            //Stop();
            m_interpTime = 0.0f;
            ChooseNextAction();
        }
    }
    else
    {
        
    }
}

void Ghost::Render()
{
    //float wX, wY;
    //Level::Get().Transform(m_localX, m_localY, &wX, &wY);
    //Engine::DrawCircle(wX, wY, 8.0f, NColor::Red);
    //Engine::DrawCircle(m_worldX, m_worldY, 8.0f, NColor::Yellow);
    //Engine::DrawLine(m_worldX, 0.0f, m_worldX, SCREEN_HEIGHT, NColor::LightGreen);
    //Engine::DrawLine(0.0f, m_worldY, SCREEN_WIDTH, m_worldY, NColor::LightGreen);

    float dstSize = 52.0f;
    m_animation.Render({
        m_worldX - (dstSize / 2.0f),
        m_worldY - (dstSize / 2.0f),
        dstSize, dstSize }
    );
}

void Ghost::Start()
{
    ChooseNextAction();
}

void Ghost::MoveTo(int x, int y)
{
    m_path.clear();
    int rows = Level::Get().GetHeight();
    int cols = Level::Get().GetWidth();
    m_path = Engine::AStar(Level::Get().GetData(), rows, cols, m_localX, m_localY, x, y);

    // Remove the first location since we are already there:
    m_path.erase(m_path.begin());

    if (!m_path.empty())
    {
        std::pair<int, int> destination = m_path.front();
        m_path.erase(m_path.begin());

        int dx = destination.first - m_localX;
        int dy = destination.second - m_localY;
        if (IsStopped())
        {
            SetupInterpolation(dx, dy);
        }

        SetDirection(dx, dy);
    }
    else
    {
        Stop();
    }
}

void Ghost::SetPlayer(Player* player)
{
    m_playerPtr = player;
}

void Ghost::NextLocationOnPath()
{
    if (!m_path.empty())
    {
        std::pair<int, int> destination = m_path.front();
        m_path.erase(m_path.begin());

        int dx = destination.first - m_localX;
        int dy = destination.second - m_localY;

        if (!CheckCollision(m_localX + dx, m_localY + dy))
        {
            if (IsStopped())
            {
                SetupInterpolation(dx, dy);
            }

            SetDirection(dx, dy);
        }
    }
    else
    {
        Stop();
    }
}

void Ghost::ChooseNextAction()
{
    NextLocationOnPath();
}

void Ghost::SetAnimation(EEntityDirection direction)
{
    switch (direction)
    {
    case MOVING_LEFT: m_animation.Play("left", true);
        break;
    case MOVING_RIGHT: m_animation.Play("right", true);
        break;
    case MOVING_UP: m_animation.Play("up", true);
        break;
    case MOVING_DOWN: m_animation.Play("down", true);
        break;
    }
}
