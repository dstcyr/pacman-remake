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

    CDelegate OnPlayerDie;

private:
    std::vector<Entity*> m_activeEntity;
    std::vector<Ghost*> m_activeGhost;
    Player* m_player;
    float m_powerElapsed;
    bool m_powerActivated;
    bool m_playerDead;

    void OnPowerActivated(const Event& e);
};