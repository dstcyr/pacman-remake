#include "Level.h"
#include "Engine.h"
#include "Config.h"
#include "Collision.h"
#include "MathUtils.h"

Level& Level::Get()
{
    static Level levelStaticInstance;
    return levelStaticInstance;
}

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

                    Engine::DrawRect(
                        static_cast<float>(x * m_tileWidth + m_offsetX),
                        static_cast<float>(y * m_tileHeight + m_offsetY),
                        static_cast<float>(m_tileWidth),
                        static_cast<float>(m_tileHeight),
                        NColor(255, 0, 0, 100)
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

void Level::GetStartingPosition(float* x, float* y)
{
    int localX = 10;
    int localY = 20;

    float wX, wY;
    ToWorld(localX, localY, &wX, &wY);
    *x = wX + (m_tileWidth / 2.0f);
    *y = wY + (m_tileHeight / 2.0f);
}

void Level::ToWorld(const int localX, const int localY, float* worldX, float* worldY) const
{
    *worldX = static_cast<float>(localX * m_tileWidth) + m_offsetX;
    *worldY = static_cast<float>(localY * m_tileHeight) + m_offsetY;
}

bool Level::CheckCollision(float x, float y, float w, float h, int* id) const
{
    return false;
}

int Level::GetTileAt(int x, int y)
{
    int idx = GetIndexFromPosition(x, y);

    if (idx >= static_cast<int>(m_gridData.size()))
    {
        return INVALID_TILE;
    }

    return m_gridData[idx];
}

void Level::Transform(int localX, int localY, float* worldX, float* worldY)
{
    ToWorld(localX, localY, worldX, worldY);
    *worldX += (m_tileWidth / 2.0f);
    *worldY += (m_tileHeight / 2.0f);
}

void Level::SetTile(int x, int y, ETile tileValue)
{
    int idx = GetIndexFromPosition(x, y);

    if (idx < static_cast<int>(m_gridData.size()))
    {
        m_gridData[idx] = tileValue;
    }
}

void Level::ToLocal(const float worldX, const float worldY, int* localX, int* localY) const
{
    *localX = static_cast<int>((worldX - m_offsetX) / m_tileWidth);
    *localY = static_cast<int>((worldY - m_offsetY) / m_tileHeight);
}

int Level::GetIndexFromPosition(int x, int y) const
{
    return x + m_width * y;
}
