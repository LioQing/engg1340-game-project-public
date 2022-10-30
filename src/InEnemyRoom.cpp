#include <InEnemyRoom.h>

#include <stdexcept>
#include <Game.h>
#include <Pause.h>
#include <GameFinished.h>
#include <InMap.h>
#include <InExitRoom.h>

void InEnemyRoom::Start(Game& game)
{
    if (!game.IsResourcesInitialized())
    {
        throw std::runtime_error("Resources are not initialized");
    }

    game.GetUI().SetEnemy(game);
}

void InEnemyRoom::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == Room::EnemyRoomInfo::ReturnType::Pause)
        {
            game.ChangeScene<Pause>(Pause::ResumeType::EnemyRoom);
        }
        else if (choice == Room::EnemyRoomInfo::ReturnType::Victory)
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

            // hp regen
            res.player.hp += res.player.hp_regen;
            if (res.player.hp > res.player.max_hp)
                res.player.hp = res.player.max_hp;
        }
        else if (choice == Room::EnemyRoomInfo::ReturnType::Defeat)
        {
            game.ChangeScene<GameFinished>(GameFinished::Result::Defeat);
        }
    }
}