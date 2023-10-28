#include "Player.h"
#include "Engine.h"
#include "Level.h"
#include "Config.h"
#include "MathUtils.h"
#include "Log.h"

int moveX = 0;
int moveY = 0;
float time = 0.0f;
float startX, startY;
float endX, endY;
int targetX = 0;
int targetY = 0;
float duration = 0.2f;

void Player::Initialize()
{
    m_gridX = 10;
    m_gridY = 20;
    Level::Get().Transform(m_gridX, m_gridY, &m_pixelX, &m_pixelY);

    float playerAnimationSpeed = 0.05f;
    m_playerAnim.Init("Assets/Images/player.png", 6, 32, 32);
    m_playerAnim.AddClip("right", 0, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_right", 1, 1, playerAnimationSpeed);
    m_playerAnim.AddClip("left", 3, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_left", 4, 1, playerAnimationSpeed);
    m_playerAnim.AddClip("up", 6, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_up", 7, 1, playerAnimationSpeed);
    m_playerAnim.AddClip("down", 9, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_down", 10, 1, playerAnimationSpeed);
    m_playerAnim.Play("idle_left", false);

    m_eatA = Engine::LoadSound("Assets/Audio/eatA.wav");
    Engine::SetVolume(m_eatA, 50);
    m_eatB = Engine::LoadSound("Assets/Audio/eatB.wav");
    Engine::SetVolume(m_eatB, 50);
    m_eatToggle = true;
}

void Player::Update(float dt)
{
    UpdateInputs();

    if (!IsStopped())
    {
        UpdateMovements(dt);

        if (time >= duration)
        {
            int tile = Level::Get().GetTileAt(m_gridX, m_gridY);
            if (tile == PILL_TILE || tile == POWER_TILE)
            {
                Level::Get().SetTile(m_gridX, m_gridY, EMPTY_TILE);
                PlayEatSFX();
            }

            if (m_gridX == 1 && m_gridY == 13)
            {
                m_gridX = 19;
                m_gridY = 13;

                Level::Get().Transform(m_gridX, m_gridY, &m_pixelX, &m_pixelY);
                startX = m_pixelX;
                startY = m_pixelY;

                Level::Get().Transform(m_gridX - 1, m_gridY, &endX, &endY);
                time = 0;
            }
            else if (m_gridX == 19 && m_gridY == 13)
            {
                m_gridX = 1;
                m_gridY = 13;

                Level::Get().Transform(m_gridX, m_gridY, &m_pixelX, &m_pixelY);
                startX = m_pixelX;
                startY = m_pixelY;

                Level::Get().Transform(m_gridX - 1, m_gridY, &endX, &endY);
                time = 0;
            }

            if (moveX == -1)
            {
                if (!CheckCollision(m_gridX - 1, m_gridY))
                {
                    UpdateInterpolation(-1, 0);
                    SetDirection(-1, 0);
                    time = 0.0f;
                    m_playerAnim.Play("left", true);
                }
                else
                {
                    Stop();
                    m_playerAnim.Play("idle_left", false);
                }
            }
            else if (moveX == 1)
            {
                if (!CheckCollision(m_gridX + 1, m_gridY))
                {
                    UpdateInterpolation(1, 0);
                    SetDirection(1, 0);
                    time = 0.0f;
                    m_playerAnim.Play("right", true);
                }
                else
                {
                    Stop();
                    m_playerAnim.Play("idle_right", false);
                }
            }
            else if (moveY == -1)
            {
                if (!CheckCollision(m_gridX, m_gridY - 1))
                {
                    UpdateInterpolation(0, -1);
                    SetDirection(0, -1);
                    time = 0.0f;
                    m_playerAnim.Play("up", true);
                }
                else
                {
                    Stop();
                    m_playerAnim.Play("idle_up", false);
                }
            }
            else if (moveY == 1)
            {
                if (!CheckCollision(m_gridX, m_gridY + 1))
                {
                    UpdateInterpolation(0, 1);
                    SetDirection(0, 1);
                    time = 0.0f;
                    m_playerAnim.Play("down", true);
                }
                else
                {
                    Stop();
                    m_playerAnim.Play("idle_down", false);
                }
            }
            else
            {
                Stop();
            }
        }
    }
}

void Player::Render()
{
    float dstSize = 52.0f;
    m_playerAnim.Render({
        m_pixelX - (dstSize / 2.0f),
        m_pixelY - (dstSize / 2.0f),
        dstSize,
        dstSize
        });

#if DEBUG_PLAYER_MOVEMENTS
    float wX, wY;
    Level::Get().Transform(m_gridX, m_gridY, &wX, &wY);
    Engine::DrawCircle(wX, wY, 8.0f, NColor::Red);
    Engine::DrawCircle(m_pixelX, m_pixelY, 8.0f, NColor::Yellow);
    Engine::DrawLine(m_pixelX, 0.0f, m_pixelX, SCREEN_HEIGHT, NColor::LightGreen);
    Engine::DrawLine(0.0f, m_pixelY, SCREEN_WIDTH, m_pixelY, NColor::LightGreen);
#endif
}

bool Player::IsStopped()
{
    return moveX == 0 && moveY == 0;
}

void Player::UpdateInputs()
{
    if (Engine::GetKey(KEY_LEFT))
    {
        if (moveX != -1)
        {
            if (!CheckCollision(m_gridX - 1, m_gridY))
            {
                if (IsStopped())
                {
                    UpdateInterpolation(-1, 0);
                    m_playerAnim.Play("left", true);
                }

                SetDirection(-1, 0);
            }
        }
    }
    else if (Engine::GetKey(KEY_RIGHT))
    {
        if (moveX != 1)
        {
            if (!CheckCollision(m_gridX + 1, m_gridY))
            {
                if (IsStopped())
                {
                    UpdateInterpolation(1, 0);
                    m_playerAnim.Play("right", true);
                }

                SetDirection(1, 0);
            }
        }
    }
    else if (Engine::GetKey(KEY_UP))
    {
        if (moveY != -1)
        {
            if (!CheckCollision(m_gridX, m_gridY - 1))
            {
                if (IsStopped())
                {
                    UpdateInterpolation(0, -1);
                    m_playerAnim.Play("up", true);
                }

                SetDirection(0, -1);
            }
        }
    }
    else if (Engine::GetKey(KEY_DOWN))
    {
        if (moveY != 1)
        {
            if (!CheckCollision(m_gridX, m_gridY + 1))
            {
                if (IsStopped())
                {
                    UpdateInterpolation(0, 1);
                    m_playerAnim.Play("down", true);
                }

                SetDirection(0, 1);
            }
        }
    }
}

void Player::UpdateMovements(float dt)
{
    if (time < duration)
    {
        m_pixelX = Engine::Linear(time, startX, endX, duration);
        m_pixelY = Engine::Linear(time, startY, endY, duration);
        time += dt;

        m_playerAnim.Update(dt);
    }
}

void Player::PlayEatSFX()
{
    if (m_eatToggle)
    {
        Engine::PlaySFX(m_eatA);
    }
    else
    {
        Engine::PlaySFX(m_eatB);
    }

    m_eatToggle = !m_eatToggle;
}

void Player::UpdateInterpolation(int dx, int dy)
{
    startX = m_pixelX;
    startY = m_pixelY;
    m_gridX += dx;
    m_gridY += dy;
    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
}

void Player::Stop()
{
    moveX = 0;
    moveY = 0;
    time = 0.0f;
}

void Player::Idle()
{
    if (IsStopped())
    {
        m_playerAnim.Play("idle_right", false);
    }
    else
    {
        if (moveX == 1)
        {
            m_playerAnim.Play("idle_right", false);
        }
        else if (moveX == -1)
        {
            m_playerAnim.Play("idle_left", false);
        }
        else if (moveY == 1)
        {
            m_playerAnim.Play("idle_down", true);
        }
        else if (moveY == -1)
        {
            m_playerAnim.Play("idle_up", true);
        }
    }
}

void Player::Start()
{
    UpdateInterpolation(-1, 0);
    SetDirection(-1, 0);
    m_playerAnim.Play("left", true);
}

void Player::SetDirection(int dx, int dy)
{
    moveX = dx;
    moveY = dy;
}

bool Player::CheckCollision(int x, int y)
{
    int tileValue = Level::Get().GetTileAt(x, y);
    return tileValue == COLLISION_TILE || tileValue == INVALID_TILE;
}
