#include "Menu.h"
#include "Engine.h"
#include "Log.h"
#include "SaveGame.h"

void Menu::OnEnter()
{
    LOG(LL_DEBUG, "Menu::OnEnter");
    m_mockup = Engine::LoadTexture("Assets/Design/Menu.png");
    m_whiteFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "whitefont", 32, NColor::White);
    m_orangeFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "redfont", 32, NColor(224, 80, 0, 255));
    m_title = Engine::LoadTexture("Assets/Images/Title.png");
}

void Menu::OnUpdate(float dt)
{
    if (Engine::GetKeyDown(KEY_START))
    {
        SaveGame::Load();
        Engine::SetState("game");
    }
}

void Menu::OnRender()
{
    //Engine::DrawTexture(m_mockup, false, false, NColor(255, 255, 255, 60));
    Engine::DrawTexture(m_title, 160, 210);

    Engine::DrawString("HIGH SCORE", m_orangeFont, 350.0f, 70.0f);
    Engine::DrawString(std::to_string(SaveGame::highScore), m_whiteFont, 450.0f, 110.0f);
    Engine::DrawString("1UP", m_orangeFont, 140.0f, 70.0f);
    Engine::DrawString(std::to_string(SaveGame::score), m_whiteFont, 180.0f, 110.0f);
    Engine::DrawString("PUSH START", m_whiteFont, 350.0f, 550.0f);
}

void Menu::OnExit()
{
}
