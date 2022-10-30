#include <InHangman.h>

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <Game.h>
#include <InMap.h>
#include <InChestRoom.h>
#include <widget.h>
#include <InExitRoom.h>
#include <GameFinished.h>

void InMiniGame::Start(Game& game)
{
    if (!game.IsCLI())
    {
        game.GetUI().SetMenu("Locked Chest", { "Open", "Give up" }, 
            widget::TextBox("Your Guess", input, [](const std::string& text) -> bool
            {
                return text.size() == 1 && isalpha(text.at(0));
            }, true)
        );
    }

    minigame.Init(game.GetExeDirPath() + "/Hangman.txt");
    prev_input = "";
    unlocked = false;
    failed = false;

    std::ostringstream os;
    os << minigame;
    game.GetUI().Output(os.str());
}

void InMiniGame::Update(Game& game)
{
    if (!game.IsCLI())
    {
        if (!failed && !unlocked && input != prev_input)
        {
            if (!minigame.Run(input.at(0)))
            {
                std::ostringstream os;
                minigame.OutputAlreadyGuessed(os);
                game.GetUI().Output(os.str());
            }
            prev_input = input;

            if (minigame.Result() == true)
            {
                game.GetUI().Output("Congrats! You have successfully guessed the word " + minigame.GetAnswerWord() + ".\nChest unlocked!");
                unlocked = true;
            }
            else if (!minigame.HasTryLeft())
            {
                game.GetUI().Output("Too bad you didn't guessed the word.\nThe answer is " + minigame.GetAnswerWord() + ".");

                failed = true;
            }
            else
            {
                std::ostringstream os;
                os << minigame;
                game.GetUI().Output(os.str());
            }
        }

        if (auto choice = game.GetUI().Input())
        {
            if (choice == 0)
            {
                if (unlocked)
                {
                    // unlock
                    auto& room = std::get<Room::Type::Chest>(game.GetResources().level.GetRoom(game.GetResources().player.room_pos).room);
                    room.is_locked = false;

                    game.ChangeScene<InChestRoom>();
                }
                else
                {                
                    std::ostringstream os;
                    os << minigame;
                    game.GetUI().Output("The chest is still locked!\n" + os.str());
                }
            }
            else if (choice == 1)
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
        }
    }
    else
    {
        while (minigame.HasTryLeft())
        {
            std::cout << "> " << std::flush;

            char temp;
            std::cin >> temp;
            if (!minigame.Run(temp))
            {
                minigame.OutputAlreadyGuessed(std::cout);
            }

            std::cout << std::endl;
            
            if (minigame.Result() == true)
            {
                break;
            }
            
            std::cout << minigame << std::endl;
        }

        auto clear_ignore = [](std::istream& is){ is.clear(); is.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); };

        if (minigame.Result() == false)
        {
            std::cout << "Too bad you didn't guessed the word." << std::endl;
            std::cout << "The answer is " << minigame.GetAnswerWord() << "." << std::endl;
            
            // press enter to continue
            {
                std::cout << "--Press Enter to continue--" << std::endl;
                
                clear_ignore(std::cin);
                std::string str;
                std::getline(std::cin, str);
            }

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
            std::cout << "Congrats! You have successfully guessed the word" << minigame.GetAnswerWord()  << "."<< std::endl;
            
            // press enter to continue
            {
                std::cout << "--Press Enter to continue--" << std::endl;
                
                clear_ignore(std::cin);
                std::string str;
                std::getline(std::cin, str);
            }

            // unlock
            auto& room = std::get<Room::Type::Chest>(game.GetResources().level.GetRoom(game.GetResources().player.room_pos).room);
            room.is_locked = false;

            game.ChangeScene<InChestRoom>();
        }
    }
}