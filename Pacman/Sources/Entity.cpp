#include "Entity.h"
#include "Engine.h"
#include "Level.h"
#include "MathUtils.h"

Entity::Entity() : Entity(0, 0)
{
}

Entity::Entity(int x, int y)
{
    SetPosition(x, y);
}

void Entity::Initialize()
{
}

void Entity::Update(float dt)
{
}

void Entity::Render()
{
}

void Entity::Start()
{

}

void Entity::GetPosition(int* x, int* y)
{
    *x = m_localX;
    *y = m_localY;
}

void Entity::SetPosition(int x, int y)
{
    m_localX = x;
    m_localY = y;
    Level::Get().Transform(m_localX, m_localY, &m_worldX, &m_worldY);
}

void Entity::SetupInterpolation(int dx, int dy)
{
    m_interpStartX = m_worldX;
    m_interpStartY = m_worldY;
    m_localX += dx;
    m_localY += dy;
    Level::Get().Transform(m_localX, m_localY, &m_interpEndX, &m_interpEndY);
}

void Entity::SetupInterpolation(EEntityDirection direction)
{
    switch (direction)
    {
    case MOVING_LEFT: SetupInterpolation(-1, 0);
        break;
    case MOVING_RIGHT: SetupInterpolation(1, 0);
        break;
    case MOVING_UP: SetupInterpolation(0, -1);
        break;
    case MOVING_DOWN: SetupInterpolation(0, 1);
        break;
    }
}

void Entity::UpdateInterpolation(float dt)
{
    if (m_interpTime < m_interpSpeed)
    {
        m_worldX = Engine::Linear(m_interpTime, m_interpStartX, m_interpEndX, m_interpSpeed);
        m_worldY = Engine::Linear(m_interpTime, m_interpStartY, m_interpEndY, m_interpSpeed);
        m_interpTime += dt;
    }
}

void Entity::SetDirection(int dx, int dy)
{
    m_directionX = dx;
    m_directionY = dy;

    if (dx > 0)
    {
        m_direction = EEntityDirection::MOVING_RIGHT;
    }
    else if (dx < 0)
    {
        m_direction = EEntityDirection::MOVING_LEFT;
    }
    else if (dy > 0)
    {
        m_direction = EEntityDirection::MOVING_DOWN;
    }
    else if (dy < 0)
    {
        m_direction = EEntityDirection::MOVING_UP;
    }
    else
    {
        m_direction = EEntityDirection::MOVING_IDLE;
    }
}

void Entity::SetDirection(EEntityDirection direction)
{
    switch (direction)
    {
    case MOVING_LEFT:
        SetDirection(-1, 0);
        break;
    case MOVING_RIGHT:
        SetDirection(1, 0);
        break;
    case MOVING_UP:
        SetDirection(0, -1);
        break;
    case MOVING_DOWN:
        SetDirection(0, 1);
        break;
    case MOVING_IDLE:
        SetDirection(0, 0);
        break;
    }
}

bool Entity::CanMove(EEntityDirection direction)
{
    switch (direction)
    {
    case MOVING_LEFT:
        return !CheckCollision(m_localX - 1, m_localY);
        break;
    case MOVING_RIGHT:
        return !CheckCollision(m_localX + 1, m_localY);
        break;
    case MOVING_UP:
        return !CheckCollision(m_localX, m_localY - 1);
        break;
    case MOVING_DOWN:
        return !CheckCollision(m_localX, m_localY + 1);
        break;
    }

    return false;
}

bool Entity::IsStopped() const
{
    return m_directionX == 0 && m_directionY == 0;
}

void Entity::Stop()
{
    m_directionX = 0;
    m_directionY = 0;
    m_interpTime = 0.0f;
}

void Entity::GetDirection(int* x, int* y)
{
    *x = m_directionX;
    *y = m_directionY;
}

bool Entity::CheckCollision(int x, int y)
{
    int tileValue = Level::Get().GetTileAt(x, y);
    return tileValue == COLLISION_TILE || tileValue == INVALID_TILE;
}
