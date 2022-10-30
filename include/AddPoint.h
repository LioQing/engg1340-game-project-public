#pragma once

#include <cstddef>
#include <Scene.h>

struct AddPoint : Scene
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

    size_t hp;
    size_t hp_regen;
    size_t speed;

    static constexpr size_t pts = 3;
};