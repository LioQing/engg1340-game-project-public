#include <GameFinished.h>

#include <MainMenu.h>
#include <Game.h>

GameFinished::GameFinished(Result result) : result(result)
{
}

void GameFinished::Start(Game& game)
{
    game.GetUI().SetMenu("Game Ended", { "Return to Main Menu" });

    if (result == Result::Victory)
    {
        game.GetUI().Output("You won! You got the Insulin, and finally your city can be saved!");
    }
    else if (result == Result::Defeat)
    {
        game.GetUI().Output("You lost! You are now forever lost in the sugar.");
    }
}

void GameFinished::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            game.ChangeScene<MainMenu>();
            game.ClearResources();
        }
    }
}