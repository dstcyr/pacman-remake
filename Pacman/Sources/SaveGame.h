#pragma once
#include "File.h"
#include "Delegate.h"

class SaveGame
{
public:
    static int highScore;
    static int round;
    static int life;

    static void CheckHighScore();
    static void Save();
    static void Load();
    static void NextRound();
    static void AddScore(int scoreToAdd);
    static int GetScore();

    static CDelegate OnPlayerGainLife;

private:
    static int score;
    static int nextScoreLife;
    static CFile m_saveGameFile;
};