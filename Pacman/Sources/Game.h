#pragma once
#include "Config.h"
#include "StateMachine.h"
#include "Level.h"
#include "Game.h"
#include "Player.h"

class Game : public IState
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnExit() override;

private:
    size_t m_whiteFont = 0;
    size_t m_orangeFont = 0;
    size_t m_peachFont = 0;
    size_t m_blueFont = 0;
    size_t m_background;
    size_t m_flash;
    size_t m_currentBackground;
    size_t m_playerReadySfx = 0;
    bool m_playerReady;
    bool m_levelCleared;
    float m_playerReadyDelay;
    Player m_player;
    float m_flashElapsed;
    float m_flashTotalDelay;
};