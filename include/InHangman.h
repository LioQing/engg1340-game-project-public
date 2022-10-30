#pragma once

#include <Scene.h>
#include <Hangman.h>

struct InMiniGame : Scene
{
    /**
     * @brief Called when the scene is first loaded
     * 
     * @param game The game object
     */
    void Start(Game& game) override;

    /**
     * @brief Called every game loop
     * 
     * @param game The game object
     */
    void Update(Game& game) override;

private:

    Hangman minigame;

    std::string input;
    std::string prev_input;
    bool unlocked;
    bool failed;
};