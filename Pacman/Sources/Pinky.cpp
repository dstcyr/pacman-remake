#include "Pinky.h"
#include "Player.h"

Pinky::Pinky() : Pinky(0, 0, nullptr)
{
}

Pinky::Pinky(Player* player) : Pinky(0, 0, player)
{
}

Pinky::Pinky(int x, int y, Player* player) : Ghost(x, y, player)
{
}

void Pinky::Initialize()
{
    m_animation.Init("Assets/Images/pinky.png", 8, 32, 32);
    Ghost::Initialize();
}

void Pinky::Start()
{
}

void Pinky::ChooseNextAction()
{
    // Calculate the target point based on Pac-Man's current direction.
    // int targetX = pacMan.x;
    // int targetY = pacMan.y;

    // Adjust the target point based on Pac-Man's direction.
    // For example, if Pac-Man is moving up, targetY is adjusted to be a few spaces above Pac-Man.
    // if (pacMan.direction == 0) {
    //     targetY -= 4; // Move Pinky a few spaces above Pac-Man.
    // }
    // else if (pacMan.direction == 1) {
    //     targetX += 4; // Move Pinky a few spaces to the right of Pac-Man.
    // }
    // else if (pacMan.direction == 2) {
    //     targetY += 4; // Move Pinky a few spaces below Pac-Man.
    // }
    // else if (pacMan.direction == 3) {
    //     targetX -= 4; // Move Pinky a few spaces to the left of Pac-Man.
    // }

    // Now, move Pinky toward the calculated target point.
    // Implement your logic here to move Pinky toward the target.
    // This might involve pathfinding algorithms or simple movement rules.
}
