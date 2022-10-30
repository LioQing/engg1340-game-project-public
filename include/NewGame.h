#pragma once

#include <string>
#include <Scene.h>

/**
 * @brief New game menu scene
 * 
 */
struct NewGame : Scene
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

    std::string seed_input;
};