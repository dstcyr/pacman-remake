#include "EntityManager.h"
#include "Player.h"
#include "Blinky.h"
#include "Clyde.h"
#include "Inky.h"
#include "Pinky.h"
#include "Log.h"

EntityManager& EntityManager::Get()
{
    static EntityManager manager;
    return manager;
}

void EntityManager::Initialize()
{
    m_powerElapsed = 0.0f;
    m_powerActivated = false;

    m_player = new Player(10, 20);
    m_player->OnPowerActivated.Bind(this, &EntityManager::OnPowerActivated);
    m_activeEntity.push_back(m_player);

    Blinky* blinky = new Blinky(1, 1, m_player);
    m_activeEntity.push_back(blinky);
    m_activeGhost.push_back(blinky);

    Clyde* clyde = new Clyde(19, 1, m_player);
    m_activeEntity.push_back(clyde);
    m_activeGhost.push_back(clyde);

    Inky* inky = new Inky(19, 25, m_player, blinky, clyde);
    m_activeEntity.push_back(inky);
    m_activeGhost.push_back(inky);

    Pinky* pinky = new Pinky(1, 25, m_player);
    m_activeEntity.push_back(pinky);
    m_activeGhost.push_back(pinky);

    for (Entity* entity : m_activeEntity)
    {
        entity->Initialize();
    }

    m_playerDead = false;
}

void EntityManager::Update(float dt)
{
    if(m_playerDead) return;

    if (m_powerActivated)
    {
        m_powerElapsed += dt;
        if (m_powerElapsed >= 8.0f)
        {
            LOG(LL_DEBUG, "Deactivate Power");
            m_powerElapsed = 0.0f;
            m_powerActivated = false;
            for (Ghost* ghost : m_activeGhost)
            {
                EGhostState currentState = ghost->GetState();
                if (currentState != EGhostState::DEAD)
                {
                    ghost->SetState(EGhostState::CHASE);
                }
            }
        }
        else if (m_powerElapsed >= 5.0f)
        {
            for (Ghost* ghost : m_activeGhost)
            {
                EGhostState currentState = ghost->GetState();
                if (currentState == EGhostState::FLEE)
                {
                    ghost->Flash();
                }
            }
        }
    }

    for (Entity* entity : m_activeEntity)
    {
        entity->Update(dt);
    }

    int px, py;
    m_player->GetPosition(&px, &py);
    for (Ghost* ghost : m_activeGhost)
    {
        EGhostState state = ghost->GetState();
        if (state != EGhostState::DEAD)
        {
            int gx, gy;
            ghost->GetPosition(&gx, &gy);

            if (gx == px && py == gy)
            {
                if (state == EGhostState::CHASE)
                {
                    // Player dies
                    m_playerDead = true;
                    OnPlayerDie.Invoke<Event>();
                }
                else if (state == EGhostState::FLEE)
                {
                    // Ghost dies
                    ghost->Kill();
                }
            }
        }
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

void EntityManager::Clear()
{
    for (Entity* entity : m_activeEntity)
    {
        SAFE_DELETE(entity);
    }

    m_activeEntity.clear();
    m_activeGhost.clear();
}

void EntityManager::OnPowerActivated(const Event& e)
{
    LOG(LL_DEBUG, "Activate Power");
    m_powerElapsed = 0.0f;
    m_powerActivated = true;

    for (Ghost* ghost : m_activeGhost)
    {
        EGhostState currentState = ghost->GetState();
        if (currentState != EGhostState::DEAD)
        {
            ghost->SetState(EGhostState::FLEE);
        }
    }
}
