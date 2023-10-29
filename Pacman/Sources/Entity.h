#pragma once
#include <vector>
#include "Config.h"

enum EEntityDirection
{
    MOVING_LEFT,
    MOVING_RIGHT,
    MOVING_UP,
    MOVING_DOWN,
    MOVING_IDLE
};

class Entity
{
public:
    Entity();
    Entity(int x, int y);

    virtual void Initialize();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Start();

    void GetPosition(int* x, int* y);
    void SetPosition(int x, int y);
    void Stop();
    void GetDirection(int* x, int* y);

protected:
    float m_worldX = 0.0f;
    float m_worldY = 0.0f;
    int m_localX = 0;
    int m_localY = 0;

    int m_directionX = 0;
    int m_directionY = 0;
    EEntityDirection m_direction;

    float m_interpTime = 0.0f;
    float m_interpSpeed = 0.4f;
    float m_interpStartX = 0.0f;
    float m_interpStartY = 0.0f;
    float m_interpEndX = 0.0f;
    float m_interpEndY = 0.0f;

    void SetupInterpolation(int dx, int dy);
    void SetupInterpolation(EEntityDirection direction);
    void SetDirection(int dx, int dy);
    void SetDirection(EEntityDirection direction);
    bool CanMove(EEntityDirection direction);


    void UpdateInterpolation(float dt);
    bool IsStopped() const;
    bool CheckCollision(int x, int y);
};
