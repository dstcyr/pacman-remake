#pragma once
#include <vector>
#include "Entity.h"
#include "Animation.h"

class Player;

class Ghost : public Entity
{
public:
    Ghost();
    Ghost(Player* player);
    Ghost(int x, int y, Player* player);

    void Initialize() override;
    void Update(float dt) override;
    void Render() override;
    void Start() override;
    void MoveTo(int x, int y);
    void SetPlayer(Player* player);

protected:
    std::vector<std::pair<int, int>> m_path;
    Player* m_playerPtr;
    Animation m_animation;

    void NextLocationOnPath();
    virtual void ChooseNextAction();
    void SetAnimation(EEntityDirection direction);
};
