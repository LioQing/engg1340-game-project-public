#include <InExitRoom.h>

#include <Game.h>
#include <InMap.h>
#include <AddPoint.h>

void InExitRoom::Start(Game& game)
{
    game.GetUI().SetMenu("Exit Room", { "Go to next level", "Return to map" });

    game.GetUI().Output("You are in the exit room");
}

void InExitRoom::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            game.ChangeScene<AddPoint>();
        }
        else if (choice == 1)
        {
            game.ChangeScene<InMap>();
        }
    }
}