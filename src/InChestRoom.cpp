#include <InChestRoom.h>

#include <stdexcept>
#include <array>
#include <Game.h>
#include <Pause.h>
#include <GameFinished.h>
#include <InMap.h>
#include <Visitor.h>
#include <InExitRoom.h>

void InChestRoom::Start(Game& game)
{
    if (!game.IsResourcesInitialized())
    {
        throw std::runtime_error("Resources are not initialized");
    }

    game.GetUI().SetChest(game);
}

void InChestRoom::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == Pause)
        {
            game.ChangeScene<::Pause>(Pause::ResumeType::ChestRoom);
        }
        else if (choice == NoTake)
        {
            // set current room type to none
            auto& res = game.GetResources();
            auto& room = res.level.GetRoom(res.player.room_pos);
            room.room = Room::NoneRoomInfo();
            
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
        else
        {
            auto& res = game.GetResources();
            auto& chest_room = std::get<Room::Type::Chest>(res.level.GetRoom(res.player.room_pos).room);

            std::visit(Visitor
            {
                [&](const std::array<Weapon, 3>& loots) { res.player.weapons.at(choice.value() / 4) = loots.at(choice.value() % 4); },
                [&](const std::array<Armor, 3>& loots) { res.player.armor = loots.at(choice.value()); }
            }, chest_room.loots);

            // set current room type to none
            auto& room = res.level.GetRoom(res.player.room_pos);
            room.room = Room::NoneRoomInfo();
            
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