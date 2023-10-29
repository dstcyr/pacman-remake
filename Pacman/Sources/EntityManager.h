#pragma once
#include "Entity.h"

class Player;

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

private:
    std::vector<Entity*> m_activeEntity;
    Player* m_player;
};