#pragma once

#include <cstddef>
#include <Scene.h>

struct InChestRoom : Scene
{
    enum Result : size_t
    {
        NoTake = 10,
        Pause = 11,
    };

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
};