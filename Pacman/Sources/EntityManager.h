#pragma once
#include "Entity.h"
#include "Delegate.h"

class Player;
class Ghost;

class EntityManager
{
public:
    EntityManager& Get();

    void Initialize();
    void Update(float dt);
    void Render();
    void Start();
    void StopMovement();
    void PauseAnimations();
    void Clear();

    CDelegate OnPlayerDied;

private:
    std::vector<Entity*> m_activeEntity;
    std::vector<Ghost*> m_activeGhost;
    Player* m_player;
    float m_powerElapsed;
    bool m_powerActivated;

    bool m_playerCaught;
    float m_playerCaughtElapsed;

    bool m_playerDie;
    float m_playerDieElapsed;

    bool m_GameStopped;
    int m_ghostEaten;

    void OnPowerActivated(const Event& e);
};