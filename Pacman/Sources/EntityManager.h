#pragma once
#include "Entity.h"
#include "Delegate.h"
#include "Fruit.h"

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
    Fruit* m_activeFruit;
    Player* m_player;
    float m_powerElapsed;
    bool m_powerActivated;
    size_t m_ghostSFX;
    size_t m_powerSFX;
    size_t m_eatGhostSFX;
    float m_ghostSFXElapsed;
    bool m_playerCaught;
    float m_playerCaughtElapsed;
    bool m_playerDie;
    float m_playerDieElapsed;
    bool m_GameStopped;
    int m_ghostEaten;
    float m_pauseElapsed;
    bool m_paused;

    void OnPowerActivated(const Event& e);
};