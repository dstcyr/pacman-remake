#include "Game.h"
#include "Engine.h"

void Game::OnEnter()
{
    Level::Get().Reset();
    m_background = Engine::LoadTexture("Assets/Images/Background.png");
    m_flash = Engine::LoadTexture("Assets/Images/Flash.png");

    m_whiteFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "whitefont", 32, NColor::White);
    m_orangeFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "redfont", 32, NColor(224, 80, 0, 255));
    m_peachFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "peachfont", 32, NColor::DarkPeach);
    m_blueFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "bluefont", 32, NColor::LightSkyBlue);

    m_playerReadySfx = Engine::LoadSound("Assets/Audio/playerready.wav");
    Engine::SetVolume(m_playerReadySfx, 50);
    Engine::PlaySFX(m_playerReadySfx);

    m_playerReady = false;
    m_playerReadyDelay = 0.0f;
    m_player.Initialize();
}

void Game::OnUpdate(float dt)
{
    if (!m_playerReady)
    {
        m_playerReadyDelay += dt;
        if (m_playerReadyDelay >= 4.0f)
        {
            m_playerReadyDelay = 0.0f;
            m_playerReady = true;
        }
    }
    else
    {
        m_player.Update(dt);
    }
}

void Game::OnRender()
{
    Engine::DrawTexture(m_background, 0.0f, 0.0f, 35.0f * 21.0f, 35.0f * 27.0f);
    Level::Get().Render();

    if (!m_playerReady)
    {
        Engine::DrawString("PLAYER", m_blueFont, 275.0f, 355.0f);
        Engine::DrawString("READY!", m_peachFont, 285.0f, 530.0f);
    }

    m_player.Render();
}

void Game::OnExit()
{
}
