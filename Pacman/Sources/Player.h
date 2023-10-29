#pragma once
#include <vector>
#include "Entity.h"
#include "Animation.h"

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

private:
    size_t m_eatA;
    size_t m_eatB;
    bool m_eatToggle;
    Animation m_playerAnim;

    void UpdateInputs();
    void PlayEatSFX();
};
