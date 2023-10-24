#pragma once
#include "Config.h"
#include "StateMachine.h"
#include "Level.h"

class Game : public IState
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnExit() override;

private:
    Level m_level;
    size_t m_background;
    size_t m_flash;
};