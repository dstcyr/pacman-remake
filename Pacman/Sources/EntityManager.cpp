#include "EntityManager.h"
#include "Player.h"
#include "Blinky.h"
#include "Clyde.h"
#include "Inky.h"
#include "Pinky.h"
#include "Log.h"
#include "MathUtils.h"
#include "SaveGame.h"

EntityManager& EntityManager::Get()
{
    static EntityManager manager;
    return manager;
}

void EntityManager::Initialize()
{
    m_powerElapsed = 0.0f;
    m_powerActivated = false;
    m_playerCaught = false;
    m_playerDie = false;
    m_GameStopped = false;

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
}

void EntityManager::Update(float dt)
{
    if (m_GameStopped) return;

    if (m_playerDie)
    {
        m_player->Update(dt);
        m_playerDieElapsed += dt;
        if (m_playerDieElapsed >= 4.0f)
        {
            m_GameStopped = true;
            OnPlayerDied.Invoke<Event>();
        }
    }
    else if (m_playerCaught)
    {
        m_playerCaughtElapsed += dt;
        if (m_playerCaughtElapsed >= 2.0f)
        {
            for (Ghost* ghost : m_activeGhost)
            {
                ghost->Hide();
            }

            m_player->Die();
            m_playerDie = true;
            m_playerDieElapsed = 0.0f;
        }
    }
    else
    {
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
#if !INVINSIBLE
                    if (state == EGhostState::CHASE)
                    {
                        if (!m_player->IsDead())
                        {
                            m_playerCaught = true;
                            m_playerCaughtElapsed = 0.0f;
                        }
                    }
                    else if (state == EGhostState::FLEE)
                    {
                        // Ghost dies
                        // First blue ghost : 200 points
                        // Second blue ghost : 400 points
                        // Third blue ghost : 800 points
                        // Fourth blue ghost : 1600 points
                        static int scores[4] = { 200, 400, 800, 1600 };
                        SaveGame::AddScore(scores[m_ghostEaten]);
                        m_ghostEaten = Engine::Clamp(m_ghostEaten + 1, 0, 3);

                        ghost->Kill();
                    }
#endif
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
    m_ghostEaten = 0;

    for (Ghost* ghost : m_activeGhost)
    {
        EGhostState currentState = ghost->GetState();
        if (currentState != EGhostState::DEAD)
        {
            ghost->SetState(EGhostState::FLEE);
        }
    }
}
