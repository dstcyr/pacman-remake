#pragma once
#include "Animation.h"

class Player
{
public:
    void Initialize();
    void Update(float dt);
    void Render();
    void Stop();
    void Idle();
    void Start();

private:
    Animation m_playerAnim;
    float m_pixelX;
    float m_pixelY;
    int m_gridX;
    int m_gridY;
    size_t m_eatA;
    size_t m_eatB;
    bool m_eatToggle;

    bool IsStopped();
    void UpdateInputs();
    void UpdateMovements(float dt);
    void PlayEatSFX();
    void UpdateInterpolation(int param1, int param2);
    void SetDirection(int param1, int param2);
    bool CheckCollision(int x, int y);
};