#pragma once
#include "Ghost.h"

class Blinky;
class Clyde;

class Inky : public Ghost
{
public:
    Inky();
    Inky(Player* player);
    Inky(int x, int y, Player* player, Blinky* blinky, Clyde* clyde);

    void Initialize() override;
    void Start() override;

protected:
    void ChooseNextAction() override;

private:
    Blinky* m_blinky;
    Clyde* m_clyde;
};