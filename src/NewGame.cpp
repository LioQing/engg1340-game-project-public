#include <NewGame.h>

#include <random>
#include <limits>
#include <MainMenu.h>
#include <NewCharacter.h>
#include <Game.h>
#include <GameData.h>

void NewGame::Start(Game& game)
{
    seed_input = std::to_string(static_cast<uint32_t>(std::random_device()()));
    game.GetUI().SetMenu("New Game", { "Next", "Back" },
        widget::TextBox("Seed", seed_input, [](const std::string& text) -> bool
        {
            return 
                std::all_of(text.begin(), text.end(), [](char c){ return std::isdigit(c); }) &&
                std::stoul(text) < std::numeric_limits<uint32_t>::max();
        })
    );
}

void NewGame::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            if (game.IsResourcesInitialized())
                game.ClearResources();
            
            // level
            Level level;
            level.SetSeed(std::stoul(seed_input));
            level.GotoNextLevel();

            // player
            Player player;
            player.room_pos = Vec2i(0, 0);

            player.armor = GameData::armors.at("Zion's Feathers").data;
            player.weapons.emplace_back(GameData::weapons.at("Wooden Stick").data);
            player.weapons.emplace_back(GameData::weapons.at("Bare Hand").data);

            Resources res;
            res.level = level;
            res.player = player;

            game.InitResources(res);

            game.ChangeScene<NewCharacter>();
        }
        else if (choice == 1)
        {
            game.ChangeScene<MainMenu>();
        }
    }
}