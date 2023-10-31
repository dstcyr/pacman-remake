#pragma once
#include <vector>
#include "Entity.h"
#include "Animation.h"
#include "Delegate.h"

class Player : public Entity
{
public:
    Player();
    Player(int x, int y);
    void Initialize() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void Idle();
    void Die();
    bool IsDead() const { return m_dead; }

    CDelegate OnPowerActivated;

private:
    size_t m_eatA;
    size_t m_eatB;
    size_t m_playerDiedSfx;
    bool m_eatToggle;
    Animation m_playerAnim;
    bool m_dead;

    void UpdateInputs();
    void PlayEatSFX();
};
