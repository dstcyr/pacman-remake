#include "EntityManager.h"
#include "Player.h"
#include "Blinky.h"
#include "Clyde.h"
#include "Inky.h"
#include "Pinky.h"
#include "Log.h"
#include "MathUtils.h"
#include "SaveGame.h"
#include "Engine.h"

EntityManager& EntityManager::Get()
{
    static EntityManager manager;
    return manager;
}

void EntityManager::Initialize()
{
    m_activeFruit = nullptr;

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

    m_ghostSFX = Engine::LoadSound("Assets/Audio/ghost.wav");
    m_powerSFX = Engine::LoadSound("Assets/Audio/power.wav");
    m_eatGhostSFX = Engine::LoadSound("Assets/Audio/eatghost.wav");
    Engine::SetVolume(m_powerSFX, 40);
    Engine::SetVolume(m_ghostSFX, 40);
    Engine::SetVolume(m_eatGhostSFX, 100);

    m_pauseElapsed = 0.0f;
    m_paused = false;

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
        if (m_paused)
        {
            m_pauseElapsed += dt;
            if (m_pauseElapsed >= 0.5f)
            {
                m_paused = false;
                m_pauseElapsed = 0.0f;
            }
            else
            {
                return;
            }
        }

        if ((SaveGame::dots == 70 || SaveGame::dots == 170) && m_activeFruit == nullptr)
        {
            m_activeFruit = new Fruit();
            m_activeFruit->Initialize();
        }

        if (m_powerActivated)
        {
            m_ghostSFXElapsed += dt;
            if (m_ghostSFXElapsed > 0.15f)
            {
                m_ghostSFXElapsed = 0.0f;
                Engine::PlaySFX(m_powerSFX, GHOST_SFX_CHANNEL, 0);
            }

            m_powerElapsed += dt;
            if (m_powerElapsed >= 8.0f)
            {
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
        else
        {
            m_ghostSFXElapsed += dt;
            if (m_ghostSFXElapsed > 0.35f)
            {
                m_ghostSFXElapsed = 0.0f;
                Engine::PlaySFX(m_ghostSFX, GHOST_SFX_CHANNEL, 0);
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
#if !INVINSIBLE
                        if (!m_player->IsDead())
                        {
                            m_playerCaught = true;
                            m_playerCaughtElapsed = 0.0f;
                        }
#endif
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
                        Engine::PlaySFX(m_eatGhostSFX, GHOST_SFX_CHANNEL, 0);
                        ghost->Kill();
                        m_paused = true;
                        m_pauseElapsed = 0.0f;
                    }
                }
            }
        }


        if (m_player && m_activeFruit)
        {
            int px, py;
            m_player->GetPosition(&px, &py);

            int fx, fy;
            m_activeFruit->GetPosition(&fx, &fy);

            if (px == fx && py == fy)
            {
                m_activeFruit->AddScore();
                SAFE_DELETE(m_activeFruit);
            }
        }
    }
}

void EntityManager::Render()
{
    if (!m_paused)
    {
        m_player->Render();
    }

    for (Ghost* ghost : m_activeGhost)
    {
        ghost->Render();
    }

    if (m_activeFruit)
    {
        m_activeFruit->Render();
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
