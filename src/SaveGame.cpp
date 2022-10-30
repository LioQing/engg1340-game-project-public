#include <SaveGame.h>

#include <Pause.h>
#include <Game.h>
#include <SaveData.h>
#include <SaveManager.h>

SaveGame::SaveGame(Pause::ResumeType resume_type) : resume_type(resume_type)
{
}

void SaveGame::Start(Game& game)
{
    game.GetUI().SetMenu("Save Game", { "Save", "Back" },
        widget::TextBox("Save Name", save_name)
    );
}

void SaveGame::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            SaveManager::Save(save_name, SaveData(game.GetResources(), resume_type));
            game.GetUI().Output("Game saved");
        }
        else if (choice == 1)
        {
            game.ChangeScene<Pause>(resume_type);
        }
    }
}