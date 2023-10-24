#include "Level.h"
#include "Engine.h"
#include "Config.h"

void Level::Reset()
{
    m_pill = Engine::LoadTexture("Assets/Images/Pill.png");
    m_power = Engine::LoadTexture("Assets/Images/Power.png");

    m_gridData.clear();
    m_gridData = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,1,
        1,3,1,1,1,3,1,1,1,3,1,3,1,1,1,3,1,1,1,3,1,
        1,2,1,1,1,3,1,1,1,3,1,3,1,1,1,3,1,1,1,2,1,
        1,3,1,1,1,3,1,1,1,3,1,3,1,1,1,3,1,1,1,3,1,
        1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,
        1,3,1,1,1,3,1,3,1,1,1,1,1,3,1,3,1,1,1,3,1,
        1,3,1,1,1,3,1,3,1,1,1,1,1,3,1,3,1,1,1,3,1,
        1,3,3,3,3,3,1,3,3,3,1,3,3,3,1,3,3,3,3,3,1,
        1,1,1,1,1,3,1,1,1,0,1,0,1,1,1,3,1,1,1,1,1,
        1,1,1,1,1,3,1,0,0,0,0,0,0,0,1,3,1,1,1,1,1,
        1,1,1,1,1,3,1,0,1,1,0,1,1,0,1,3,1,1,1,1,1,
        1,1,1,1,1,3,1,0,1,0,0,0,1,0,1,3,1,1,1,1,1,
        0,0,0,0,0,3,0,0,1,0,0,0,1,0,0,3,0,0,0,0,0,
        1,1,1,1,1,3,1,0,1,1,1,1,1,0,1,3,1,1,1,1,1,
        1,1,1,1,1,3,1,0,0,0,0,0,0,0,1,3,1,1,1,1,1,
        1,1,1,1,1,3,1,0,1,1,1,1,1,0,1,3,1,1,1,1,1,
        1,1,1,1,1,3,1,0,1,1,1,1,1,0,1,3,1,1,1,1,1,
        1,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,1,
        1,3,1,1,1,3,1,1,1,3,1,3,1,1,1,3,1,1,1,3,1,
        1,2,3,3,1,3,3,3,3,3,0,3,3,3,3,3,1,3,3,2,1,
        1,1,1,3,1,3,1,3,1,1,1,1,1,3,1,3,1,3,1,1,1,
        1,1,1,3,1,3,1,3,1,1,1,1,1,3,1,3,1,3,1,1,1,
        1,3,3,3,3,3,1,3,3,3,1,3,3,3,1,3,3,3,3,3,1,
        1,3,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,
        1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    };
}

void Level::Render()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int idx = GetIndexFromPosition(x, y);
            if (m_gridData[idx] != EMPTY_TILE)
            {
#if SHOW_COLLISION
                if (m_gridData[idx] == COLLISION_TILE)
                {
                    Engine::FillRect(
                        static_cast<float>(x * m_tileWidth + m_offsetX),
                        static_cast<float>(y * m_tileHeight + m_offsetY),
                        static_cast<float>(m_tileWidth),
                        static_cast<float>(m_tileHeight),
                        NColor(255, 0, 0, 50)
                    );
                }
#endif

                if (m_gridData[idx] == PILL_TILE)
                {
                    Engine::DrawTexture(
                        m_pill,
                        static_cast<float>(x * m_tileWidth + m_offsetX),
                        static_cast<float>(y * m_tileHeight + m_offsetY),
                        static_cast<float>(m_tileWidth),
                        static_cast<float>(m_tileHeight)
                    );
                }

                if (m_gridData[idx] == POWER_TILE)
                {
                    Engine::DrawTexture(
                        m_power,
                        static_cast<float>(x * m_tileWidth + m_offsetX),
                        static_cast<float>(y * m_tileHeight + m_offsetY),
                        static_cast<float>(m_tileWidth),
                        static_cast<float>(m_tileHeight)
                    );
                }
            }
        }
    }
}

int Level::GetIndexFromPosition(int x, int y) const
{
    return x + m_width * y;
}
