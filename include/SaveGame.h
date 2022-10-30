#pragma once

#include <string>
#include <Pause.h>
#include <Scene.h>

/**
 * @brief Save game menu scene
 * 
 */
struct SaveGame : Scene
{
    SaveGame(Pause::ResumeType resume_type);

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

    Pause::ResumeType resume_type;
    std::string save_name;
};