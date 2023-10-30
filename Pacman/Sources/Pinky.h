#pragma once
#include "Ghost.h"

class Player;

class Pinky : public Ghost
{
public:
    Pinky();
    Pinky(Player* player);
    Pinky(int x, int y, Player* player);

    void Initialize() override;
    void Start() override;

protected:
    void ExecuteBehaviour() override;
};