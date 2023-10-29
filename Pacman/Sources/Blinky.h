#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
public:
    Blinky();
    Blinky(Player* player);
    Blinky(int x, int y, Player* player);

    void Initialize() override;
    void Start() override;

protected:
    void ChooseNextAction() override;
};