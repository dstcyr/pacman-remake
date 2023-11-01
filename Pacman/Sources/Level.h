#pragma once
#include <vector>

enum ETile
{
    EMPTY_TILE,
    COLLISION_TILE,
    POWER_TILE,
    PILL_TILE,
    INVALID_TILE
};

class Level
{
public:
    static Level& Get();

    void Reset();
    void Render();

    void GetStartingPosition(float* x, float* y);
    void ToLocal(const float worldX, const float worldY, int* localX, int* localY) const;
    void ToWorld(const int localX, const int localY, float* worldX, float* worldY) const;
    int GetTileAt(int x, int y);
    void Transform(int localX, int localY, float* worldX, float* worldY);
    void SetTile(int x, int y, ETile tileValue);
    bool CheckLevelClear();

    void RemoveNPills(int param1);
    std::vector<int>& GetData();

    int GetWidth() const;
    int GetHeight() const;
    float CellWidth();
    float CellHeight();

    bool Empty() const;

private:
    Level() = default;
    Level(const Level& other) = default;
    ~Level() = default;

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