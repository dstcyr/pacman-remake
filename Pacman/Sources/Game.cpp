#include "Game.h"
#include "Engine.h"

void Game::OnEnter()
{
    m_level.Reset();
    m_background = Engine::LoadTexture("Assets/Images/Background.png");
    m_flash = Engine::LoadTexture("Assets/Images/Flash.png");
}

void Game::OnUpdate(float dt)
{
}

void Game::OnRender()
{
    Engine::DrawTexture(m_background, 0.0f, 0.0f, 35.0f * 21.0f, 35.0f * 27.0f);
    m_level.Render();
}

void Game::OnExit()
{
}
