#include <NewCharacter.h>

#include <NewGame.h>
#include <Introduction.h>
#include <Game.h>

void NewCharacter::Start(Game& game)
{
    game.GetUI().SetMenu("New Character", { "Start", "Back" }, 
        widget::PointAllocation("Attributes",
        {
            widget::PointAllocation::Option("HP - Health", hp, 5, 10, 10),
            widget::PointAllocation::Option("HPR - HP Regen", hp_regen, 1, 5, 5),
            widget::PointAllocation::Option("SPD - Speed", speed, 7, 15),
        }, pts)
    );
}

void NewCharacter::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            auto remaining = RemainingPoints(game);
            if (remaining != 0)
            {
                game.GetUI().Output("You must allocate all the remaining points (" + std::to_string(remaining) + " pts) before proceeding!");
                return;
            }

            auto& res = game.GetResources();

            res.player.max_hp = hp * 10;
            res.player.hp = hp * 10;
            res.player.hp_regen = hp_regen * 5;
            res.player.speed = speed;

            game.ChangeScene<Introduction>();
        }
        else if (choice == 1)
        {
            game.ChangeScene<NewGame>();
        }
    }
}

size_t NewCharacter::RemainingPoints(Game& game) const
{
    return pts - ((hp - 5) + (hp_regen - 1) + (speed - 7));
}