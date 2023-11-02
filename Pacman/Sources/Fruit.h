#pragma once


// In the original Pac-Man, the fruits are located below the Ghost House and won't move
// at all. The fruit appears after 70 dots are eaten and again after 170 dots are eaten
// unless the first fruit is still there. They will disappear if they are not eaten
// after 9-10 seconds.
class Fruit
{
public:
    void Initialize();
    bool Update(float dt);
    void GetPosition(int* x, int* y);
    void SetPosition(int x, int y);
    void Render();
    void AddScore();

private:
    float m_lifetime;
    int m_x;
    int m_y;
    int m_index;
    size_t m_fruitImage;
    size_t m_fruitSound;
};