#include <Pause.h>

#include <InEnemyRoom.h>
#include <InChestRoom.h>
#include <InMap.h>
#include <MainMenu.h>
#include <SaveGame.h>
#include <Game.h>

Pause::Pause(ResumeType resume_type) : resume_type(resume_type)
{
}

void Pause::Start(Game& game)
{
    game.GetUI().SetMenu("Pause Menu", { "Resume", "Save Game", "Return to Main Menu" });

    game.GetUI().Output("Current seed: " + std::to_string(game.GetResources().level.GetSeed()));
}

void Pause::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            if      (resume_type == ResumeType::Map)       game.ChangeScene<InMap>();
            else if (resume_type == ResumeType::EnemyRoom) game.ChangeScene<InEnemyRoom>();
            else if (resume_type == ResumeType::ChestRoom) game.ChangeScene<InChestRoom>();
        }
        else if (choice == 1)
        {
            game.ChangeScene<SaveGame>(resume_type);
        }
        else if (choice == 2)
        {
            game.ChangeScene<MainMenu>();
            game.ClearResources();
        }
    }
}