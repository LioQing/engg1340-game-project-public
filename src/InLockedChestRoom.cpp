#include <InLockedChestRoom.h>

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <Game.h>
#include <InMap.h>
#include <InHangman.h>
#include <InExitRoom.h>
#include <GameFinished.h>

void InLockedChestRoom::Start(Game& game)
{
    game.GetUI().SetMenu("In Locked Chest Room", { "Attempt to Unlock Chest", "Return to Map" });

    game.GetUI().Output("You entered a room with locked chests! You can only open one.");
}

void InLockedChestRoom::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            game.ChangeScene<InMiniGame>();
        }
        else if (choice == 1)
        {
            auto& res = game.GetResources();
            auto& room = res.level.GetRoom(res.player.room_pos);
            
            if (room.is_exit)
            {
                if (res.level.GetLevel() == 5)
                {
                    game.ChangeScene<GameFinished>(GameFinished::Result::Victory);
                }
                else
                {
                    game.ChangeScene<InExitRoom>();
                }
            }
            else game.ChangeScene<InMap>();
        }
    }
}