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
    m_playerAnim.AddClip("idle_right", 2, 1, playerAnimationSpeed);
    m_playerAnim.AddClip("left", 3, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_left", 5, 1, playerAnimationSpeed);
    m_playerAnim.AddClip("up", 6, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_up", 8, 1, playerAnimationSpeed);
    m_playerAnim.AddClip("down", 9, 3, playerAnimationSpeed);
    m_playerAnim.AddClip("idle_down", 11, 1, playerAnimationSpeed);
    m_playerAnim.Play("left", true);

    m_eatA = Engine::LoadSound("Assets/Audio/eatA.wav");
    Engine::SetVolume(m_eatA, 50);
    m_eatB = Engine::LoadSound("Assets/Audio/eatB.wav");
    Engine::SetVolume(m_eatB, 50);
    m_eatToggle = true;
}

void Player::Update(float dt)
{
    

    if (Engine::GetKey(KEY_LEFT))
    {
        if (moveX != -1)
        {
            int leftTile = Level::Get().GetTileAt(m_gridX - 1, m_gridY);
            if (leftTile != COLLISION_TILE && leftTile != INVALID_TILE)
            {
                if (moveX == 0 && moveY == 0)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;
                    m_playerAnim.Play("left", true);

                    m_gridX -= 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                }

                moveX = -1;
                moveY = 0;
            }
        }

    }
    else if (Engine::GetKey(KEY_RIGHT))
    {
        if (moveX != 1)
        {
            int rightTile = Level::Get().GetTileAt(m_gridX + 1, m_gridY);
            if (rightTile != COLLISION_TILE && rightTile != INVALID_TILE)
            {
                if (moveX == 0 && moveY == 0)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;
                    m_playerAnim.Play("right", true);

                    m_gridX += 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                }

                moveX = 1;
                moveY = 0;
            }
        }
    }
    else if (Engine::GetKey(KEY_UP))
    {
        if (moveY != -1)
        {
            int upTile = Level::Get().GetTileAt(m_gridX, m_gridY - 1);
            if (upTile != COLLISION_TILE && upTile != INVALID_TILE)
            {
                if (moveX == 0 && moveY == 0)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;
                    m_playerAnim.Play("up", true);

                    m_gridY -= 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                }

                moveX = 0;
                moveY = -1;
            }
        }
    }
    else if (Engine::GetKey(KEY_DOWN))
    {
        if (moveY != 1)
        {
            int downTile = Level::Get().GetTileAt(m_gridX, m_gridY + 1);
            if (downTile != COLLISION_TILE && downTile != INVALID_TILE)
            {
                if (moveX == 0 && moveY == 0)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;
                    m_playerAnim.Play("down", true);

                    m_gridY += 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                }

                moveX = 0;
                moveY = 1;
            }
        }
    }

    if (moveX != 0 || moveY != 0)
    {
        if (time < duration)
        {
            m_pixelX = Engine::Linear(time, startX, endX, duration);
            m_pixelY = Engine::Linear(time, startY, endY, duration);
            time += dt;

            m_playerAnim.Update(dt);
        }

        if (time >= duration)
        {
            int tile = Level::Get().GetTileAt(m_gridX, m_gridY);
            if (tile == PILL_TILE || tile == POWER_TILE)
            {
                Level::Get().SetTile(m_gridX, m_gridY, EMPTY_TILE);

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



            if (moveX == -1)
            {
                int leftTile = Level::Get().GetTileAt(m_gridX - 1, m_gridY);
                if (leftTile != COLLISION_TILE && leftTile != INVALID_TILE)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;

                    m_gridX -= 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                    time = 0.0f;

                    moveX = -1;
                    moveY = 0;
                    m_playerAnim.Play("left", true);
                }
                else
                {
                    moveX = 0;
                    moveY = 0;
                    time = 0.0f;
                    m_playerAnim.Play("idle_left", false);
                }
            }
            else if (moveX == 1)
            {
                int rightTile = Level::Get().GetTileAt(m_gridX + 1, m_gridY);
                if (rightTile != COLLISION_TILE && rightTile != INVALID_TILE)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;

                    m_gridX += 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                    time = 0.0f;

                    moveX = 1;
                    moveY = 0;
                    m_playerAnim.Play("right", true);
                }
                else
                {
                    moveX = 0;
                    moveY = 0;
                    time = 0.0f;
                    m_playerAnim.Play("idle_right", false);
                }
            }
            else if (moveY == -1)
            {
                int upTile = Level::Get().GetTileAt(m_gridX, m_gridY - 1);
                if (upTile != COLLISION_TILE && upTile != INVALID_TILE)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;

                    m_gridY -= 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                    time = 0.0f;

                    moveX = 0;
                    moveY = -1;
                    m_playerAnim.Play("up", true);
                }
                else
                {
                    moveX = 0;
                    moveY = 0;
                    time = 0.0f;
                    m_playerAnim.Play("idle_up", false);
                }
            }
            else if (moveY == 1)
            {
                int downTile = Level::Get().GetTileAt(m_gridX, m_gridY + 1);
                if (downTile != COLLISION_TILE && downTile != INVALID_TILE)
                {
                    startX = m_pixelX;
                    startY = m_pixelY;

                    m_gridY += 1;
                    Level::Get().Transform(m_gridX, m_gridY, &endX, &endY);
                    time = 0.0f;

                    moveX = 0;
                    moveY = 1;
                    m_playerAnim.Play("down", true);
                }
                else
                {
                    moveX = 0;
                    moveY = 0;
                    time = 0.0f;
                    m_playerAnim.Play("idle_down", false);
                }
            }
            else
            {
                moveX = 0;
                moveY = 0;
                time = 0.0f;
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
