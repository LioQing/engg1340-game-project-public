#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <Scene.h>

/**
 * @brief Load game menu scene
 * 
 */
struct LoadGame : Scene
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

    std::vector<std::string> save_entries;
    size_t save_selected = 0;
};