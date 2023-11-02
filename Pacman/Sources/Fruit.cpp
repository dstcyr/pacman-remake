#include "Fruit.h"
#include "Engine.h"
#include "Level.h"
#include "SaveGame.h"

int fruitScore[8] = { 100, 300, 500, 700, 1000, 2000, 3000, 5000 };

void Fruit::Initialize()
{
    m_lifetime = 10.0f;
    m_x = 10;
    m_y = 15;

    // The way fruits are drawn in the original Pac - Man is through three seperate algorithmns.
    //     In levels 0 - 7, the fruits are drawn starting from the cherry to the fruit that matches the current level.
    //     In levels 8 - 18 in the orginal Pac - Man, the first fruit drawn is offset by the level counter, pushing 
    //     out older fruits as the game progresses and new fruits are drawn.
    //     In levels 19 and up, the only fruits being drawn at this point are the 7 keys.

    int currentLevel = (SaveGame::round - 1) % 8;
    if (currentLevel < 19)
    {
        m_index = currentLevel;
    }
    else
    {
        m_index = 7;
    }

    m_fruitImage = Engine::LoadTexture("Assets/Images/fruits.png");
    m_fruitSound = Engine::LoadSound("Assets/Audio/fruit.wav");
    Engine::SetVolume(m_fruitSound, 60);
}

bool Fruit::Update(float dt)
{
    m_lifetime -= dt;
    return m_lifetime > 0.0f;
}

void Fruit::GetPosition(int* x, int* y)
{
    *x = m_x;
    *y = m_y;
}

void Fruit::SetPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Fruit::Render()
{
    float wx, wy;
    Level::Get().Transform(m_x, m_y, &wx, &wy);

    float dstSize = 48.0f;
    Engine::DrawTexture(m_fruitImage,
        Rect<int>(m_index * 32, 0, 32, 32),
        Rect<float>(wx - (dstSize / 2.0f), wy - (dstSize / 2.0f), dstSize, dstSize),
        0.0,
        false,
        false,
        NColor::White);
}

void Fruit::AddScore()
{
    int score = fruitScore[m_index];
    SaveGame::AddScore(score);
    Engine::PlaySFX(m_fruitSound);
}
