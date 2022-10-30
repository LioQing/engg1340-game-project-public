#pragma once

#include <Scene.h>

/**
 * @brief Pause menu scene
 * 
 */
struct Pause : Scene
{
    enum class ResumeType
    {
        Map, EnemyRoom, ChestRoom
    };

    /**
     * @brief Construct a new Pause object given the resume type
     * 
     * @param resume_type The resume type indicating what scene to resume to
     */
    Pause(ResumeType resume_type);

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

    ResumeType resume_type;
};