#include "Player.h"
#include "Engine.h"
#include "Level.h"

Player::Player() : Player(0, 0)
{
}

Player::Player(int x, int y) : Entity(x, y)
{
}

void Player::Initialize()
{
    m_interpSpeed = 0.2f;
    SetPosition(10, 20);

    const float animSpeed = 0.05f;
    m_playerAnim.Init("Assets/Images/player.png", 6, 32, 32);
    m_playerAnim.AddClip("right", 0, 3, animSpeed);
    m_playerAnim.AddClip("idle_right", 1, 1, animSpeed);
    m_playerAnim.AddClip("left", 3, 3, animSpeed);
    m_playerAnim.AddClip("idle_left", 4, 1, animSpeed);
    m_playerAnim.AddClip("up", 6, 3, animSpeed);
    m_playerAnim.AddClip("idle_up", 7, 1, animSpeed);
    m_playerAnim.AddClip("down", 9, 3, animSpeed);
    m_playerAnim.AddClip("idle_down", 10, 1, animSpeed);
    m_playerAnim.AddClip("die", 12, 11, 0.17f);
    m_playerAnim.Play("idle_left", false);

    m_eatA = Engine::LoadSound("Assets/Audio/eatA.wav");
    Engine::SetVolume(m_eatA, 50);
    m_eatB = Engine::LoadSound("Assets/Audio/eatB.wav");
    Engine::SetVolume(m_eatB, 50);
    m_playerDiedSfx = Engine::LoadSound("Assets/Audio/playerdied.wav");
    Engine::SetVolume(m_playerDiedSfx, 50);
    m_eatToggle = true;
    m_dead = false;
}

void Player::Update(float dt)
{
    if (m_dead)
    {
        m_playerAnim.Update(dt);
    }
    else
    {
        UpdateInputs();

        if (!IsStopped())
        {
            UpdateInterpolation(dt);
            m_playerAnim.Update(dt);

            if (m_interpTime >= m_interpSpeed)
            {
                int tile = Level::Get().GetTileAt(m_localX, m_localY);
                if (tile == PILL_TILE)
                {
                    Level::Get().SetTile(m_localX, m_localY, EMPTY_TILE);
                    PlayEatSFX();
                }

                if (tile == POWER_TILE)
                {
                    Level::Get().SetTile(m_localX, m_localY, EMPTY_TILE);
                    PlayEatSFX();
                    OnPowerActivated.Invoke<Event>();
                }

                if (m_localX == 1 && m_localY == 13)
                {
                    m_localX = 19;
                    m_localY = 13;

                    Level::Get().Transform(m_localX, m_localY, &m_worldX, &m_worldY);
                    m_interpStartX = m_worldX;
                    m_interpStartY = m_worldY;

                    Level::Get().Transform(m_localX - 1, m_localY, &m_interpEndX, &m_interpEndY);
                    m_interpTime = 0;
                }
                else if (m_localX == 19 && m_localY == 13)
                {
                    m_localX = 1;
                    m_localY = 13;

                    Level::Get().Transform(m_localX, m_localY, &m_worldX, &m_worldY);
                    m_interpStartX = m_worldX;
                    m_interpStartY = m_worldY;

                    Level::Get().Transform(m_localX - 1, m_localY, &m_interpEndX, &m_interpEndY);
                    m_interpTime = 0;
                }

                if (m_directionX == -1)
                {
                    if (!CheckCollision(m_localX - 1, m_localY))
                    {
                        SetupInterpolation(-1, 0);
                        SetDirection(-1, 0);
                        m_interpTime = 0.0f;
                        m_playerAnim.Play("left", true);
                    }
                    else
                    {
                        Stop();
                        m_playerAnim.Play("idle_left", false);
                    }
                }
                else if (m_directionX == 1)
                {
                    if (!CheckCollision(m_localX + 1, m_localY))
                    {
                        SetupInterpolation(1, 0);
                        SetDirection(1, 0);
                        m_interpTime = 0.0f;
                        m_playerAnim.Play("right", true);
                    }
                    else
                    {
                        Stop();
                        m_playerAnim.Play("idle_right", false);
                    }
                }
                else if (m_directionY == -1)
                {
                    if (!CheckCollision(m_localX, m_localY - 1))
                    {
                        SetupInterpolation(0, -1);
                        SetDirection(0, -1);
                        m_interpTime = 0.0f;
                        m_playerAnim.Play("up", true);
                    }
                    else
                    {
                        Stop();
                        m_playerAnim.Play("idle_up", false);
                    }
                }
                else if (m_directionY == 1)
                {
                    if (!CheckCollision(m_localX, m_localY + 1))
                    {
                        SetupInterpolation(0, 1);
                        SetDirection(0, 1);
                        m_interpTime = 0.0f;
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
}

void Player::Render()
{
    float dstSize = 52.0f;
    m_playerAnim.Render({
        m_worldX - (dstSize / 2.0f),
        m_worldY - (dstSize / 2.0f),
        dstSize, dstSize }
    );
}

void Player::Start()
{
    SetupInterpolation(-1, 0);
    SetDirection(-1, 0);
    m_playerAnim.Play("left", true);
}

void Player::Idle()
{
    if (IsStopped())
    {
        m_playerAnim.Play("idle_right", false);
    }
    else
    {
        if (m_directionX == 1)
        {
            m_playerAnim.Play("idle_right", false);
        }
        else if (m_directionX == -1)
        {
            m_playerAnim.Play("idle_left", false);
        }
        else if (m_directionY == 1)
        {
            m_playerAnim.Play("idle_down", true);
        }
        else if (m_directionY == -1)
        {
            m_playerAnim.Play("idle_up", true);
        }
    }
}

void Player::Die()
{
    m_dead = true;
    m_playerAnim.Play("die", false);
    Engine::PlaySFX(m_playerDiedSfx);
}

void Player::UpdateInputs()
{
    if (Engine::GetKey(KEY_LEFT))
    {
        if (m_directionX != -1)
        {
            if (!CheckCollision(m_localX - 1, m_localY))
            {
                if (IsStopped())
                {
                    SetupInterpolation(-1, 0);
                    m_playerAnim.Play("left", true);
                }

                SetDirection(-1, 0);
            }
        }
    }
    else if (Engine::GetKey(KEY_RIGHT))
    {
        if (m_directionX != 1)
        {
            if (!CheckCollision(m_localX + 1, m_localY))
            {
                if (IsStopped())
                {
                    SetupInterpolation(1, 0);
                    m_playerAnim.Play("right", true);
                }

                SetDirection(1, 0);
            }
        }
    }
    else if (Engine::GetKey(KEY_UP))
    {
        if (m_directionY != -1)
        {
            if (!CheckCollision(m_localX, m_localY - 1))
            {
                if (IsStopped())
                {
                    SetupInterpolation(0, -1);
                    m_playerAnim.Play("up", true);
                }

                SetDirection(0, -1);
            }
        }
    }
    else if (Engine::GetKey(KEY_DOWN))
    {
        if (m_directionY != 1)
        {
            if (!CheckCollision(m_localX, m_localY + 1))
            {
                if (IsStopped())
                {
                    SetupInterpolation(0, 1);
                    m_playerAnim.Play("down", true);
                }

                SetDirection(0, 1);
            }
        }
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
