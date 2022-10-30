#include <InMap.h>

#include <Game.h>
#include <Pause.h>
#include <InEnemyRoom.h>
#include <InChestRoom.h>
#include <InLockedChestRoom.h>
#include <InExitRoom.h>
#include <GameFinished.h>

void InMap::Start(Game& game)
{
    game.GetUI().SetMap(game);

    auto& res = game.GetResources();

    game.GetUI().Output("Currently in Level " + std::to_string(res.level.GetLevel()) + 
        ", Room (" + std::to_string(res.player.room_pos.x) + ", " + std::to_string(res.player.room_pos.y) + ")");
}

void InMap::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        auto& res = game.GetResources();
        auto has_neighbours = res.level.HasNeighbours(res.player.room_pos);

        if (has_neighbours.none())
            throw std::runtime_error("Room has no neighbours");

        if (choice == Room::North || choice == Room::South || choice == Room::East || choice == Room::West)
        {
            res.player.room_pos = res.level.GetNeighbourPos(res.player.room_pos, static_cast<Room::Direction>(choice.value()));

            size_t index = res.level.GetRoom(res.player.room_pos).room.index();

            // change scene according to room type
            if      (index == Room::Type::Enemy) game.ChangeScene<InEnemyRoom>();
            else if (index == Room::Type::Chest) 
            {
                auto& room = std::get<Room::Type::Chest>(res.level.GetRoom(res.player.room_pos).room);
                if (room.is_locked) game.ChangeScene<InLockedChestRoom>();
                else game.ChangeScene<InChestRoom>();
            }
            else
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
        else if (choice == Room::Pause)
        {
            game.ChangeScene<Pause>(Pause::ResumeType::Map);
        }
    }
}