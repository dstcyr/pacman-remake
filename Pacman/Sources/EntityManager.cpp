#include "EntityManager.h"
#include "Player.h"
#include "Blinky.h"
#include "Clyde.h"
#include "Inky.h"
#include "Pinky.h"

EntityManager& EntityManager::Get()
{
    static EntityManager manager;
    return manager;
}

void EntityManager::Initialize()
{
    m_player = new Player(10, 20);
    m_activeEntity.push_back(m_player);

    Blinky* blinky = new Blinky(1, 1, m_player);
    m_activeEntity.push_back(blinky);

    Clyde* clyde = new Clyde(19, 1, m_player);
    m_activeEntity.push_back(clyde);

    Inky* inky = new Inky(19, 25, m_player, blinky, clyde);
    m_activeEntity.push_back(inky);

    Pinky* pinky = new Pinky(1, 25, m_player);
    m_activeEntity.push_back(pinky);

    for (Entity* entity : m_activeEntity)
    {
        entity->Initialize();
    }
}

void EntityManager::Update(float dt)
{
    for (Entity* entity : m_activeEntity)
    {
        entity->Update(dt);
    }
}

void EntityManager::Render()
{
    for (Entity* entity : m_activeEntity)
    {
        entity->Render();
    }
}

void EntityManager::Start()
{
    for (Entity* entity : m_activeEntity)
    {
        entity->Start();
    }
}

void EntityManager::StopMovement()
{
    for (Entity* entity : m_activeEntity)
    {
        entity->Stop();
    }
}

void EntityManager::PauseAnimations()
{
    m_player->Idle();
}
