#pragma once
#include "Ghost.h"

class Clyde : public Ghost
{
public:
    Clyde();
    Clyde(Player* player);
    Clyde(int x, int y, Player* player);

    void Initialize();
    void Start() override;

protected:
    void ExecuteBehaviour() override;
};