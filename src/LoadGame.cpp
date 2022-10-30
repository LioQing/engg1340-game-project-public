#include <LoadGame.h>

#include <MainMenu.h>
#include <Game.h>
#include <SaveManager.h>
#include <Pause.h>
#include <InMap.h>
#include <InEnemyRoom.h>
#include <InChestRoom.h>

void LoadGame::Start(Game& game)
{
    save_entries = SaveManager::Entries();

    game.GetUI().SetMenu("Load Game", { "Load", "Back" },
        widget::List("Saves", save_entries, save_selected)
    );
}

void LoadGame::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            auto save_data = SaveManager::Load(save_entries.at(save_selected));

            if (game.IsResourcesInitialized())
                game.ClearResources();

            game.InitResources(save_data.res);

            if      (save_data.resume_type == Pause::ResumeType::Map)       game.ChangeScene<InMap>();
            else if (save_data.resume_type == Pause::ResumeType::EnemyRoom) game.ChangeScene<InEnemyRoom>();
            else if (save_data.resume_type == Pause::ResumeType::ChestRoom) game.ChangeScene<InChestRoom>();
        }
        if (choice == 1)
        {
            game.ChangeScene<MainMenu>();
        }
    }
}