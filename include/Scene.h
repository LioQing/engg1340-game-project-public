#pragma once

struct Game;

/**
 * @brief Interface of a scene
 * 
 */
struct Scene
{
    virtual ~Scene() = default;

    /**
     * @brief Called when the scene is first loaded
     * 
     * @param game The game object
     */
    virtual void Start(Game& game) {}

    /**
     * @brief Called every game loop
     * 
     * @param game The game object
     */
    virtual void Update(Game& game) = 0;

    /**
     * @brief Called when the scene is unloaded
     * 
     * @param game The game object
     */
    virtual void Finish(Game& game) {}
};