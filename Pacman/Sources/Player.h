#pragma once
#include "MovingObject.h"
#include "Animation.h"

class Player
{
public:
    void Initialize();
    void Update(float dt);
    void Render();

private:
    Animation m_playerAnim;
    float m_pixelX;
    float m_pixelY;
    int m_gridX;
    int m_gridY;
    size_t m_eatA;
    size_t m_eatB;
    bool m_eatToggle;
};