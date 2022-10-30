#include <MainMenu.h>

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <Game.h>
#include <NewGame.h>
#include <LoadGame.h>

void MainMenu::Start(Game& game)
{
    // game.GetUI().SetMenu("Main Menu", { "New Game", "Load Game", "Exit" },
    //     // selection widget
    //     widget::Selection("Test Selection", { "Option 1", "Option 2", "Option 3" }, test_selection_option),
    //     // list widget
    //     widget::List("Test List", { "Option 1", "Option 2", "Option 3" }, test_list_option),
    //     // point allocation widget
    //     widget::PointAllocation("Test Point Allocation",
    //     {
    //         widget::PointAllocation::Option("Option 1", test_option1_pts, 1, 10),
    //         widget::PointAllocation::Option("Option 2", test_option2_pts, 1, 10, 10),
    //         widget::PointAllocation::Option("Option 3", test_option3_pts, 2, 15, 10),
    //     }, 15),
    //     // text box widget
    //     widget::TextBox("Test Text Box", test_text_box_text, [](const std::string& text)
    //     {
    //         return std::all_of(text.begin(), text.end(), [](char c){ return std::isdigit(c); });
    //     })
    // );
    
    // game.GetUI().Output("Welcome to the engg1340-game-project!");

    game.GetUI().SetMenu("Main Menu", { "New Game", "Load Game", "Exit" });
}

void MainMenu::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if      (choice == 0) game.ChangeScene<NewGame>();
        else if (choice == 1) game.ChangeScene<LoadGame>();
        else if (choice == 2) game.Exit();
    }
}