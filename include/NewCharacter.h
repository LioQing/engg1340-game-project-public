#pragma once

#include <cstddef>
#include <Scene.h>

/**
 * @brief New character menu scene
 * 
 */
struct NewCharacter : Scene
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

    /**
     * @brief Get the number of points remaining
     * 
     * @param game The game object
     */
    size_t RemainingPoints(Game& game) const;

    size_t hp = 5;
    size_t hp_regen = 1;
    size_t speed = 7;

    static constexpr size_t pts = 10;
};