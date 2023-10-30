#include "Ghost.h"
#include "Level.h"
#include "Engine.h"
#include "AStar.h"
#include "Player.h"
#include "Vec2D.h"
#include "MathUtils.h"

Ghost::Ghost() : Ghost(0, 0, nullptr)
{
}

Ghost::Ghost(Player* player) : Ghost(0, 0, player)
{
}

Ghost::Ghost(int x, int y, Player* player) : Entity(x, y), m_playerPtr(player)
{
    m_state = EGhostState::CHASE;
    m_StartFleeing = false;
}

void Ghost::Initialize()
{
    m_state = EGhostState::CHASE;

    const float animSpeed = 0.05f;
    m_animation.AddClip("right", 0, 2, animSpeed);
    m_animation.AddClip("idle_right", 0, 1, animSpeed);
    m_animation.AddClip("up", 2, 2, animSpeed);
    m_animation.AddClip("idle_up", 2, 1, animSpeed);
    m_animation.AddClip("left", 4, 2, animSpeed);
    m_animation.AddClip("idle_left", 4, 1, animSpeed);
    m_animation.AddClip("down", 6, 2, animSpeed);
    m_animation.AddClip("idle_down", 6, 1, animSpeed);
    m_animation.AddClip("fleeing", 8, 2, animSpeed);
    m_animation.AddClip("fleeing_end", 8, 4, animSpeed);
    m_animation.AddClip("dead_left", 12, 1, animSpeed);
    m_animation.AddClip("dead_right", 13, 1, animSpeed);
    m_animation.AddClip("dead_up", 14, 1, animSpeed);
    m_animation.AddClip("dead_down", 15, 1, animSpeed);
    m_animation.Play("idle_left", false);

    m_interpSpeed = 0.3f;
}

void Ghost::Update(float dt)
{
    m_animation.Update(dt);

    if (!IsStopped())
    {
        UpdateInterpolation(dt);
        if (m_interpTime >= m_interpSpeed)
        {
            if (m_state == EGhostState::DEAD)
            {
                Stop();
            }
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

void Ghost::SetState(EGhostState state)
{
    if (m_state != EGhostState::FLEE && state == EGhostState::FLEE)
    {
        m_StartFleeing = true;
        m_animation.Play("fleeing", true);
    }

    m_state = state;
}

void Ghost::Flash()
{
    m_animation.Play("fleeing_end", true);
}

EGhostState Ghost::GetState() const
{
    return m_state;
}

void Ghost::Kill()
{
    m_state = EGhostState::DEAD;
    m_interpSpeed = 0.1f;
    m_exiting = false;
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
            SetAnimation(dx, dy);
        }
    }
    else
    {
        if (m_localX == 10 && m_localY == 13)
        {
            Stop();
            MoveTo(10, 10);
            m_exiting = true;
            SetAnimation(EEntityDirection::MOVING_UP);
        }
        else if (m_localX == 10 && m_localY == 10 && m_exiting)
        {
            m_exiting = false;
            m_interpSpeed = 0.3f;
            m_direction = EEntityDirection::MOVING_IDLE;
            SetState(EGhostState::CHASE);

            int randomDirection = Engine::RandRange(0, 1) == 0 ? -1 : 1;
            if (IsStopped())
            {
                SetupInterpolation(randomDirection, 0);
            }

            SetDirection(randomDirection, 0);
            SetAnimation(randomDirection, 0);
        }
        else
        {
            Stop();
            MoveTo(10, 13);
        }
    }
}

void Ghost::ChooseNextAction()
{
    switch (m_state)
    {
    case CHASE:
        ExecuteBehaviour();
        break;

    case FLEE:
        MoveAwayFromPlayer();
        break;

    case DEAD:
        NextLocationOnPath();
        break;
    }
}

void Ghost::SetAnimation(EEntityDirection direction)
{
    if (m_state == EGhostState::CHASE)
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
    else if (m_state == EGhostState::FLEE)
    {
        m_animation.Play("fleeing", true);
    }
    else if (m_state == EGhostState::DEAD)
    {
        switch (direction)
        {
        case MOVING_LEFT: m_animation.Play("dead_left", true);
            break;
        case MOVING_RIGHT: m_animation.Play("dead_right", true);
            break;
        case MOVING_UP: m_animation.Play("dead_up", true);
            break;
        case MOVING_DOWN: m_animation.Play("dead_down", true);
            break;
        }
    }
}

void Ghost::SetAnimation(int dx, int dy)
{
    if (dx > 0)
    {
        SetAnimation(EEntityDirection::MOVING_RIGHT);
    }
    else if (dx < 0)
    {
        SetAnimation(EEntityDirection::MOVING_LEFT);
    }
    else if (dy > 0)
    {
        SetAnimation(EEntityDirection::MOVING_DOWN);
    }
    else if (dy < 0)
    {
        SetAnimation(EEntityDirection::MOVING_UP);
    }
}

void Ghost::MoveAwayFromPlayer()
{
    if (m_playerPtr)
    {
        int px, py;
        m_playerPtr->GetPosition(&px, &py);

        float wx, wy;
        Level::Get().Transform(px, py, &wx, &wy);
        float cellWidth = Level::Get().CellWidth();
        float cellHeight = Level::Get().CellHeight();

        std::pair<EEntityDirection, float> selection = { EEntityDirection::MOVING_IDLE, 0.0f };

        if (m_StartFleeing)
        {
            m_StartFleeing = false;
            if (CanMove(EEntityDirection::MOVING_RIGHT))
            {
                float hf = Vec2D::Distance(m_worldX + cellWidth, m_worldY, wx, wy);

                if (hf > selection.second)
                {
                    selection = { EEntityDirection::MOVING_RIGHT, hf };
                }
            }

            if (CanMove(EEntityDirection::MOVING_LEFT))
            {
                float hf = Vec2D::Distance(m_worldX - cellWidth, m_worldY, wx, wy);
                if (hf > selection.second)
                {
                    selection = { EEntityDirection::MOVING_LEFT, hf };
                }
            }

            if (CanMove(EEntityDirection::MOVING_UP))
            {
                float hf = Vec2D::Distance(m_worldX, m_worldY - cellHeight, wx, wy);
                if (hf > selection.second)
                {
                    selection = { EEntityDirection::MOVING_UP, hf };
                }
            }

            if (CanMove(EEntityDirection::MOVING_DOWN))
            {
                // Don't go down in the middle when not dead
                if (!(m_localX == 10 && m_localY == 10))
                {
                    float hf = Vec2D::Distance(m_worldX, m_worldY + cellHeight, wx, wy);
                    if (hf > selection.second)
                    {
                        selection = { EEntityDirection::MOVING_DOWN, hf };
                    }
                }
            }
        }
        else
        {
            if (CanMove(EEntityDirection::MOVING_RIGHT) && m_direction != EEntityDirection::MOVING_LEFT)
            {
                float hf = Vec2D::Distance(m_worldX + cellWidth, m_worldY, wx, wy);

                if (hf > selection.second)
                {
                    selection = { EEntityDirection::MOVING_RIGHT, hf };
                }
            }

            if (CanMove(EEntityDirection::MOVING_LEFT) && m_direction != EEntityDirection::MOVING_RIGHT)
            {
                float hf = Vec2D::Distance(m_worldX - cellWidth, m_worldY, wx, wy);
                if (hf > selection.second)
                {
                    selection = { EEntityDirection::MOVING_LEFT, hf };
                }
            }

            if (CanMove(EEntityDirection::MOVING_UP) && m_direction != EEntityDirection::MOVING_DOWN)
            {
                float hf = Vec2D::Distance(m_worldX, m_worldY - cellHeight, wx, wy);
                if (hf > selection.second)
                {
                    selection = { EEntityDirection::MOVING_UP, hf };
                }
            }

            if (CanMove(EEntityDirection::MOVING_DOWN) && m_direction != EEntityDirection::MOVING_UP)
            {
                // Don't go down in the middle when not dead
                if (!(m_localX == 10 && m_localY == 10))
                {
                    float hf = Vec2D::Distance(m_worldX, m_worldY + cellHeight, wx, wy);
                    if (hf > selection.second)
                    {
                        selection = { EEntityDirection::MOVING_DOWN, hf };
                    }
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
