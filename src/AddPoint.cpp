#include <AddPoint.h>

#include <Game.h>
#include <InMap.h>

void AddPoint::Start(Game& game)
{
    // get player stats
    auto& res = game.GetResources();
    auto& player = res.player;
    hp = player.max_hp / 10;
    hp_regen = player.hp_regen / 5;
    speed = player.speed;

    // menus
    game.GetUI().SetMenu("Add Points", { "Confirm & go to next level", "Return to map" },
        widget::PointAllocation("Attributes",
        {
            widget::PointAllocation::Option("HP - Health", hp, hp, 30, 10),
            widget::PointAllocation::Option("HPR - HP Regen", hp_regen, hp_regen, 15, 5),
            widget::PointAllocation::Option("SPD - Speed", speed, speed, 25),
        }, pts)
    );

    game.GetUI().Output("You can now add extra points to your attributes!");
}

void AddPoint::Update(Game& game)
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

            res.level.GotoNextLevel();
            res.player.room_pos = Vec2i(0, 0);

            // confirm allocation
            auto old_max_hp = res.player.max_hp;
            res.player.max_hp = hp * 10;
            res.player.hp *= res.player.max_hp / old_max_hp;
            res.player.hp_regen = hp_regen * 5;
            res.player.speed = speed;

            game.ChangeScene<InMap>();
        }
        else if (choice == 1)
        {
            game.ChangeScene<InMap>();
        }
    }
}

size_t AddPoint::RemainingPoints(Game& game) const
{
    auto& res = game.GetResources();
    return pts - ((hp - res.player.max_hp / 10) + (hp_regen - res.player.hp_regen / 5) + (speed - res.player.speed));
}