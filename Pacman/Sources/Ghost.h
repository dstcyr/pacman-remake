#pragma once
#include <vector>
#include "Entity.h"
#include "Animation.h"

class Player;

enum EGhostState
{
    CHASE,
    FLEE,
    DEAD
};

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
    void SetState(EGhostState state);
    void Flash();
    EGhostState GetState() const;
    void Kill();

protected:
    std::vector<std::pair<int, int>> m_path;
    Player* m_playerPtr;
    Animation m_animation;
    EGhostState m_state;
    bool m_StartFleeing;
    bool m_exiting;

    void NextLocationOnPath();
    virtual void ChooseNextAction();
    void SetAnimation(EEntityDirection direction);
    void SetAnimation(int dx, int dy);

    virtual void ExecuteBehaviour() = 0;
    void MoveAwayFromPlayer();
};
