#pragma once

#include <string>
#include <cstddef>
#include <Scene.h>

/**
 * @brief Main menu scene
 * 
 */
struct MainMenu : Scene
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

    size_t test_selection_option = 0;
    size_t test_list_option = 0;
    size_t test_option1_pts = 1;
    size_t test_option2_pts = 1;
    size_t test_option3_pts = 1;
    std::string test_text_box_text = "Only numbers allowed";
};