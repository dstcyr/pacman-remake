#pragma once
#include <vector>

enum ETile
{
    EMPTY_TILE,
    COLLISION_TILE,
    POWER_TILE,
    PILL_TILE
};

class Level
{
public:
    void Reset();
    void Render();

private:
    std::vector<int> m_gridData;
    int m_width = 21;
    int m_height = 27;
    int m_tileWidth = 35;
    int m_tileHeight = 35;
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;
    size_t m_pill;
    size_t m_power;

    int GetIndexFromPosition(int x, int y) const;
};