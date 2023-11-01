#include "Game.h"
#include "Engine.h"
#include "SaveGame.h"
#include "MathUtils.h"

void Game::OnEnter()
{
#if START_IN_GAME
    if (Level::Get().Empty())
    {
        Level::Get().Reset();
    }
#endif

    m_background = Engine::LoadTexture("Assets/Images/Background.png");
    m_mockup = Engine::LoadTexture("Assets/Design/Level1.png");
    m_flash = Engine::LoadTexture("Assets/Images/Flash.png");
    m_currentBackground = m_background;

    m_whiteFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "whitefont", 28, NColor::White);
    m_orangeFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "redfont", 28, NColor(224, 80, 0, 255));
    m_peachFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "peachfont", 32, NColor::DarkPeach);
    m_blueFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "bluefont", 32, NColor::LightSkyBlue);

    lifeSfx = Engine::LoadSound("Assets/Audio/extralife.wav");
    Engine::SetVolume(lifeSfx, 40);

    m_playerReadySfx = Engine::LoadSound("Assets/Audio/playerready.wav");
    Engine::SetVolume(m_playerReadySfx, 50);
    Engine::PlaySFX(m_playerReadySfx);

    m_lifeTexture = Engine::LoadTexture("Assets/Images/life.png");

    m_playerReady = false;
    m_playerReadyDelay = 0.0f;

    m_entityMgr.Initialize();
    m_entityMgr.OnPlayerDied.Bind(this, &Game::OnPlayerDied);

    m_levelCleared = false;
    m_flashElapsed = 0.0f;
    m_PlayerDied = false;

    SaveGame::OnPlayerGainLife.Bind(this, &Game::OnPlayerGainLife);
}

void Game::OnUpdate(float dt)
{
    if (m_PlayerDied)
    {
        SaveGame::life--;
        if (SaveGame::life <= 0)
        {
            SaveGame::life = 0;
            Engine::SetState("menu");
        }
        else
        {
            Engine::SetState("game");
        }
    }
    else
    {
        if (Engine::GetKeyDown(KEY_1))
        {
            Level::Get().RemoveNPills(10);
        }

        if (!m_playerReady)
        {
            m_playerReadyDelay += dt;
            if (m_playerReadyDelay >= 4.0f)
            {
                m_playerReadyDelay = 0.0f;
                m_playerReady = true;
                m_entityMgr.Start();
            }
        }
        else if (m_levelCleared)
        {
            m_flashElapsed += dt;
            m_flashTotalDelay += dt;
            if (m_flashElapsed > 0.1f)
            {
                m_flashElapsed = 0.0f;
                m_currentBackground = (m_currentBackground == m_background) ? m_flash : m_background;
            }
            if (m_flashTotalDelay > 3.0f)
            {
                Level::Get().Reset();
                Engine::SetState("game");
            }
        }
        else
        {
            m_entityMgr.Update(dt);

            m_levelCleared = Level::Get().CheckLevelClear();
            if (m_levelCleared)
            {
                m_flashElapsed = 0.0f;
                m_flashTotalDelay = 0.0f;
                m_entityMgr.StopMovement();
                m_entityMgr.PauseAnimations();
            }
        }
    }
}

void Game::OnRender()
{
    Engine::DrawTexture(m_currentBackground, 0.0f, 0.0f, 35.0f * 21.0f, 35.0f * 27.0f);
    //Engine::DrawTexture(m_mockup, -70.0f, 0.0f);
    Level::Get().Render();

    if (!m_playerReady)
    {
        Engine::DrawString("PLAYER", m_blueFont, 275.0f, 355.0f);
        Engine::DrawString("READY!", m_peachFont, 285.0f, 530.0f);
    }

    m_entityMgr.Render();

    Engine::DrawString("HIGH SCORE", m_orangeFont, 735.0f, 70.0f);
    Engine::DrawString(std::to_string(SaveGame::highScore), m_whiteFont, 820.0f, 110.0f);
    Engine::DrawString("1UP", m_orangeFont, 790.0f, 220.0f);
    Engine::DrawString(std::to_string(SaveGame::GetScore()), m_whiteFont, 820.0f, 260.0f);

    int numberOfLives = Engine::Clamp(SaveGame::life, 0, 9);
    for (int i = 0; i < numberOfLives; i++)
    {
        int y = i / 3;
        int x = i % 3;
        float offsetX = static_cast<float>(x) * 70.0f;
        float offsetY = static_cast<float>(y) * 70.0f;
        Engine::DrawTexture(m_lifeTexture, 780.0f + offsetX, 730.0f + offsetY);
    }
}

void Game::OnExit()
{
    m_entityMgr.Clear();
    SaveGame::CheckHighScore();
    SaveGame::OnPlayerGainLife.Clear();
}

void Game::OnPlayerDied(const Event& e)
{
    m_PlayerDied = true;
}

void Game::OnPlayerGainLife(const Event& e)
{
    Engine::PlaySFX(lifeSfx);
}
