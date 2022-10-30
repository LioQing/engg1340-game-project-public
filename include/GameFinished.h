#pragma once

#include <Scene.h>

/**
 * @brief Game finished menu scene
 * 
 */
struct GameFinished : Scene
{
    enum class Result
    {
        Victory,
        Defeat
    };

    /**
     * @brief Construct a new GameFinished object with the result
     * 
     * @param result The result of the game
     */
    GameFinished(Result result);

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

    Result result;
};