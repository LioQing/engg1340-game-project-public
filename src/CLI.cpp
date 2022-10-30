#include <CLI.h>

#include <iostream>
#include <limits>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <vector>
#include <Game.h>
#include <Room.h>
#include <Combat.h>
#include <Random.h>
#include <Visitor.h>

const std::string CLI::horiz_line = "\n--------------------------------\n";

void CLI::Output(const std::string& text)
{
    std::cout << text << std::endl;
}

std::optional<size_t> CLI::Input()
{
    auto clear_ignore = [](std::istream& is){ is.clear(); is.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); };

    // display text ui directly returns
    if (GetType() == Type::DisplayText)
    {
        PressEnterToContinue();
        return 0;
    }

    // get user input
    std::cout << "> " << std::flush;

    std::string str;
    std::getline(std::cin, str);

    if (std::cin.fail() || str.empty() || !std::all_of(str.begin(), str.end(), [](char c){ return isdigit(c); }))
    {
        std::cout << "Invalid input" << std::endl;
        PressEnterToContinue();
        std::cin.clear();
        return std::nullopt;
    }
    std::cin.clear();

    std::istringstream iss(str);

    if (GetType() == UI::Type::Menu)
    {
        auto& menu = GetData<Type::Menu>();

        int choice;
        iss >> choice;

        // check if input is valid
        if (iss.fail() || choice < 0)
        {
            std::cout << "Invalid input" << std::endl;
            PressEnterToContinue();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice > static_cast<int>(menu.options.size()))
        {
            size_t widget_idx = choice - menu.options.size() - 1;
            if (widget_idx < menu.widgets.size())
            {
                auto& widget = *menu.widgets.at(widget_idx);

                std::cout << horiz_line << std::endl;
                std::cout << widget.title << std::endl;

                std::pair<bool, std::string> result;
                if (widget.GetType() == widget::Type::Selection)
                {
                    result = SelectionInput(static_cast<widget::Selection&>(widget));
                }
                else if (widget.GetType() == widget::Type::List)
                {
                    result = ListInput(static_cast<widget::List&>(widget));
                }
                else if (widget.GetType() == widget::Type::PointAllocation)
                {
                    result = PointAllocationInput(static_cast<widget::PointAllocation&>(widget));
                }
                else if (widget.GetType() == widget::Type::TextBox)
                {
                    result = TextBoxInput(static_cast<widget::TextBox&>(widget));
                }

                // invalid input prompt
                if (!result.first)
                {
                    std::cout << result.second << std::endl;
                    PressEnterToContinue();
                }

                OutputMenu();
                clear_ignore(iss);
                return std::nullopt;
            }

            std::cout << "Invalid input" << std::endl;
            PressEnterToContinue();
            clear_ignore(iss);
            return std::nullopt;
        }
        clear_ignore(iss);

        // show menu again
        if (choice == 0)
        {
            OutputMenu();
            return std::nullopt;
        }

        return static_cast<size_t>(choice - 1);
    }
    else if (GetType() == UI::Type::Map)
    {
        auto& map = GetData<Type::Map>();
        auto& res = map.game.GetResources();

        auto has_neighbours = res.level.HasNeighbours(res.player.room_pos);

        // map choice to Room::Direction
        std::vector<size_t> choice_room_map;
        size_t i;
        for (i = 0; i < has_neighbours.size(); ++i)
        {
            if (has_neighbours.test(i))
                choice_room_map.push_back(i);
        }
        choice_room_map.push_back(Room::Pause);

        int choice;
        iss >> choice;

        // check if input is valid
        if (iss.fail() || choice < 0)
        {
            std::cout << "Invalid input" << std::endl;
            PressEnterToContinue();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice == 0)
        {
            OutputMap();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice <= static_cast<int>(choice_room_map.size()))
        {
            clear_ignore(iss);
            return choice_room_map.at(choice - 1);
        }

        std::cout << "Invalid input" << std::endl;
        PressEnterToContinue();
        clear_ignore(iss);
        return std::nullopt;
    }
    else if (GetType() == Type::EnemyRoom)
    {
        auto& room = GetData<Type::EnemyRoom>();
        auto& res = room.game.GetResources();

        int choice;
        iss >> choice;

        // check if input is valid
        if (iss.fail() || choice < 0)
        {
            std::cout << "Invalid input" << std::endl;
            PressEnterToContinue();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice == 0)
        {
            OutputEnemyRoom();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice == static_cast<int>(res.player.weapons.size() * 2) + 1)
        {
            clear_ignore(iss);
            return Room::EnemyRoomInfo::ReturnType::Pause;
        }
        else if (choice <= static_cast<int>(res.player.weapons.size() * 2))
        {
            choice -= 1;

            // select enemy
            size_t enemy_index = 0;
            bool out_is_valid = false;
            auto result = EnemySelectionInput(enemy_index, out_is_valid);

            if (!result.first)
            {
                std::cout << result.second << std::endl;
                PressEnterToContinue();
            }
            else if (out_is_valid)
            {
                auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);
                auto& enemy = room.enemies.at(enemy_index);

                auto& player = res.player;
                auto& attack = player.weapons.at(choice / 2).attacks.at(choice % 2);

                std::cout << "Your attack's base hit is " << attack.base_hit <<
                    ", hit number > " << enemy.armor.agility << " required to hit." << std::endl;
                PressEnterToContinue("--Press Enter to roll hit die--");

                int hit = attack.base_hit + Random().GetInt(0, attack.hit_die);
                std::cout << "Hit number: " << hit << std::endl;

                if (hit <= enemy.armor.agility)
                {
                    std::cout << "The hit missed!" << std::endl;
                }
                else
                {
                    std::cout << "The hit landed!" << std::endl;
                    std::cout << std::endl;

                    std::cout << "Your attack's base damage is " << attack.base_dmg << ", roll to get extra damage." << std::endl;
                    PressEnterToContinue("--Press Enter to roll damage die--");
                    
                    int dmg = attack.base_dmg + Random().GetInt(0, attack.dmg_die);
                    std::cout << "Damage number: " << dmg << std::endl;

                    std::cout << "Total damage dealt: " << Combat::Attack(enemy, dmg) << std::endl;

                    if (enemy.hp <= 0)
                    {
                        std::cout << std::endl;
                        std::cout << enemy.name << " is defeated!" << std::endl;
                        room.enemies.erase(room.enemies.begin() + enemy_index);

                        if (room.enemies.empty())
                        {
                            std::cout << std::endl;
                            std::cout << "You have defeated all enemies in this room!" << std::endl;
                            PressEnterToContinue();

                            clear_ignore(iss);
                            return Room::EnemyRoomInfo::Victory;
                        }
                    }
                }

                std::cout << std::endl;
                PressEnterToContinue();

                // take turn
                auto enemy_actions = Combat::TakeTurn(room, res.player);

                for (auto& enemy_idx : enemy_actions)
                {
                    std::cout << horiz_line << std::endl;

                    auto& enemy = room.enemies.at(enemy_idx);

                    std::cout << enemy.name << " attacked you!" << std::endl;

                    int hit = enemy.attack.base_hit + Random().GetInt(0, enemy.attack.hit_die);

                    if (hit <= player.armor.agility)
                    {
                        std::cout << "The hit missed!" << std::endl;
                    }
                    else
                    {
                        std::cout << "The hit landed!" << std::endl;
                        std::cout << std::endl;

                        int dmg = enemy.attack.base_dmg + Random().GetInt(0, enemy.attack.dmg_die);
                        std::cout << "Damage received: " << Combat::Attack(player, dmg) << std::endl;

                        if (player.hp <= 0)
                        {
                            std::cout << std::endl;
                            std::cout << "You have been defeated!" << std::endl;
                            std::cout << std::endl;
                            PressEnterToContinue();

                            clear_ignore(iss);
                            return Room::EnemyRoomInfo::Defeat;
                        }
                    }

                    std::cout << std::endl;
                    PressEnterToContinue();
                }
            }

            OutputEnemyRoom();
            clear_ignore(iss);
            return std::nullopt;
        }

        std::cout << "Invalid input" << std::endl;
        PressEnterToContinue();
        clear_ignore(iss);
        return std::nullopt;
    }
    else if (GetType() == Type::ChestRoom)
    {
        int choice;
        iss >> choice;

        // check if input is valid
        if (iss.fail() || choice < 0)
        {
            std::cout << "Invalid input" << std::endl;
            PressEnterToContinue();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice == 0)
        {
            OutputChestRoom();
            clear_ignore(iss);
            return std::nullopt;
        }
        else if (choice == 4 || choice == 5)
        {
            clear_ignore(iss);
            return choice + 6;
        }
        else if (choice <= 3)
        {
            auto& room = GetData<Type::ChestRoom>();
            auto& res = room.game.GetResources();
            auto& chest_room = std::get<Room::Type::Chest>(res.level.GetRoom(res.player.room_pos).room);

            if (chest_room.loots.index() == 0) // loots is weapons, then ask for which weapon to swap out
            {
                std::cout << "Choose the weapon to swap out: " << std::endl;
                size_t n = 0;
                for (size_t i = 0; i < res.player.weapons.size(); ++i)
                {
                    auto& weapon = res.player.weapons.at(i);

                    std::cout << n + 1 << ". " << weapon.name << ":" << std::endl;

                    for (size_t j = 0; j < weapon.attacks.size(); ++j)
                    {
                        auto& attack = weapon.attacks.at(j);

                        std::cout << "    " << weapon.attacks.at(j).name << 
                            " (dmg: " << attack.base_dmg << " + " << attack.dmg_die << 
                            ", hit: " << attack.base_hit << " + " << attack.hit_die << ")" << std::endl;
                    }

                    ++n;
                    std::cout << std::endl;
                }

                int weapon_choice;
                clear_ignore(iss);
                while (true)
                {
                    std::cout << "> " << std::flush;

                    std::string str;
                    std::getline(std::cin, str);

                    if (std::cin.fail() || str.empty() || !std::all_of(str.begin(), str.end(), [](char c){ return isdigit(c); }))
                    {
                        std::cout << "Invalid input" << std::endl;
                        PressEnterToContinue();
                        continue;
                    }
                    std::cin.clear();

                    std::istringstream iss(str);
                    iss >> weapon_choice;
                    
                    if (iss.fail() || (weapon_choice != 1 && weapon_choice != 2))
                    {
                        std::cout << "Invalid input" << std::endl;
                        PressEnterToContinue();
                    }
                    else 
                    {
                        break;
                    }
                }

                clear_ignore(iss);
                return (choice - 1) + (weapon_choice - 1) * 4;
            }

            std::cout << "Invalid input" << std::endl;
            PressEnterToContinue();
            clear_ignore(iss);
            return choice - 1;
        }
        

        std::cout << "Invalid input" << std::endl;
        PressEnterToContinue();
        clear_ignore(iss);
        return choice - 1;
    }

    return std::nullopt;
}

void CLI::OnTypeUpdated()
{
    if (GetType() == Type::Menu)
    {
        auto& menu = GetData<Type::Menu>();

        std::cout << horiz_line << std::endl;

        std::cout << menu.title << std::endl;

        OutputMenu();
    }
    else if (GetType() == Type::Map)
    {
        auto& map = GetData<Type::Map>();
        auto& res = map.game.GetResources();

        std::cout << horiz_line << std::endl;

        std::cout << "Level " << res.level.GetLevel() << " - Room " << res.player.room_pos << std::endl;

        OutputMap();
    }
    else if (GetType() == Type::EnemyRoom)
    {
        auto& room = GetData<Type::EnemyRoom>();
        auto& res = room.game.GetResources();

        std::cout << horiz_line << std::endl;

        std::cout << "Level " << res.level.GetLevel() << " - Room " << res.player.room_pos << std::endl;
        std::cout << "You encountered enemy!" << std::endl;

        std::cout << std::endl;
        PressEnterToContinue();

        OutputEnemyRoom();
    }
    else if (GetType() == Type::ChestRoom)
    {
        auto& room = GetData<Type::ChestRoom>();
        auto& res = room.game.GetResources();

        std::cout << horiz_line << std::endl;

        std::cout << "Level " << res.level.GetLevel() << " - Room " << res.player.room_pos << std::endl;
        std::cout << "You found loots! You can only choose one." << std::endl;

        std::cout << std::endl;
        PressEnterToContinue();

        OutputChestRoom();
    }
    else if (GetType() == Type::DisplayText)
    {
        auto& room = GetData<Type::DisplayText>();

        std::cout << horiz_line << std::endl;

        std::cout << room.text << std::endl;
        std::cout << std::endl;
    }
}

void CLI::OutputMenu()
{
    auto& menu = GetData<Type::Menu>();

    std::cout << horiz_line << std::endl;
    
    // menu options
    for (size_t i = 0; i < menu.options.size(); ++i)
    {
        std::cout << i + 1 << ". " << menu.options.at(i) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Choose the option by entering the number" << std::endl;
    std::cout << std::endl;

    // widget options
    for (size_t i = 0; i < menu.widgets.size(); ++i)
    {
        const auto& w = *menu.widgets.at(i);
        if (w.GetType() == widget::Type::Selection)
        {
            const auto& selection = static_cast<const widget::Selection&>(w);

            std::cout << menu.options.size() + i + 1 << ". " << 
                selection.title << ": " << selection.options.at(selection.curr_option) << std::endl;
        }
        else if (w.GetType() == widget::Type::List)
        {
            const auto& list = static_cast<const widget::List&>(w);

            std::cout << menu.options.size() + i + 1 << ". " << 
                list.title << ": " << list.options.at(list.curr_option) << std::endl;
        }
        else if (w.GetType() == widget::Type::PointAllocation)
        {
            const auto& pt_alloc = static_cast<const widget::PointAllocation&>(w);

            std::cout << menu.options.size() + i + 1 << ". " << pt_alloc.title << 
                " (Remaining pts: " << pt_alloc.remaining_pts << "): " << std::endl;
            for (size_t i = 0; i < pt_alloc.options.size(); ++i)
            {
                auto& opt = pt_alloc.options.at(i);
                std::cout << "    " << opt.name << ": " << opt.pts * opt.multiplier << std::endl;
            }
        }
        else if (w.GetType() == widget::Type::TextBox)
        {
            const auto& text_box = static_cast<const widget::TextBox&>(w);

            std::cout << menu.options.size() + i + 1 << ". " << text_box.title << ": " << text_box.text << std::endl;
        }
    }

    if (!menu.widgets.empty())
        std::cout << std::endl;

    std::cout << "0. Show this menu again" << std::endl;
    std::cout << std::endl;
}

void CLI::OutputMap()
{
    auto& map = GetData<Type::Map>();

    std::cout << horiz_line << std::endl;

    auto& res = map.game.GetResources();
    auto map_str = res.level.GetMapString(res.player.room_pos, Room::Pause);

    std::cout << map_str.str << std::endl;
    std::cout << std::endl;

    std::cout << "Choose the option by entering the number" << std::endl;
    std::cout << std::endl;
    
    // map options
    auto has_neighbours = res.level.HasNeighbours(res.player.room_pos);

    size_t i, j;
    for (i = 0, j = 0; i < has_neighbours.size(); ++i)
    {
        if (has_neighbours.test(i))
        {
            std::cout << j + 1 << ". Go to " << Room::DirectionToString(i) << std::endl;
            ++j;
        }
    }

    std::cout << j + 1 << ". Pause" << std::endl;
    std::cout << std::endl;

    std::cout << "0. Show this menu again" << std::endl;
    std::cout << std::endl;
}

void CLI::OutputEnemyRoom()
{
    auto& room = GetData<Type::EnemyRoom>();
    auto& res = room.game.GetResources();
    auto& enemy_room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);

    std::cout << horiz_line << std::endl;

    std::cout << "You: " << std::endl;
    std::cout << "hp: " << res.player.hp << " / " << res.player.max_hp << ", hpr: " << res.player.hp_regen << ", spd: " << res.player.speed <<
            ", def: " << res.player.armor.defence << ", agi: " << res.player.armor.agility << std::endl;
    std::cout << std::endl;

    std::cout << "Enemy: " << std::endl;

    for (auto& enemy : enemy_room.enemies)
    {
        std::cout << enemy.name << " (hp: " << enemy.hp << " / " << enemy.max_hp << ", spd: " << enemy.speed <<
            ", def: " << enemy.armor.defence << ", agi: " << enemy.armor.agility << ")" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Turn Gauge:" << std::endl;

    // print the turn gauge
    auto turn_gauge = Combat::TurnGauge(enemy_room, res.player, 5);
    for (size_t i = 0; i < turn_gauge.size(); ++i)
    {
        std::cout << turn_gauge.at(i) << " -> ";
    }

    std::cout << "..." << std::endl;

    std::cout << std::endl;
    PressEnterToContinue();

    std::cout << horiz_line << std::endl;
    std::cout << "Choose the option by entering the number" << std::endl;
    std::cout << std::endl;

    size_t n = 0;
    for (size_t i = 0; i < res.player.weapons.size(); ++i)
    {
        auto& weapon = res.player.weapons.at(i);

        std::cout << weapon.name << ":" << std::endl;

        for (size_t j = 0; j < weapon.attacks.size(); ++j)
        {
            auto& attack = weapon.attacks.at(j);

            std::cout << "    " << n + 1 << ". " << weapon.attacks.at(j).name << 
                " (dmg: " << attack.base_dmg << " + " << attack.dmg_die << 
                ", hit: " << attack.base_hit << " + " << attack.hit_die << ")" << std::endl;

            ++n;
        }

        std::cout << std::endl;
    }

    std::cout << n + 1 << ". Pause" << std::endl;
    std::cout << std::endl;

    std::cout << "0. Show this menu again" << std::endl;
    std::cout << std::endl;
}

void CLI::OutputChestRoom()
{
    auto& room = GetData<Type::ChestRoom>();
    auto& res = room.game.GetResources();
    auto& chest_room = std::get<Room::Type::Chest>(res.level.GetRoom(res.player.room_pos).room);

    std::cout << horiz_line << std::endl;

    size_t n = 0;
    std::visit(Visitor
    {
        [&](const std::array<Weapon, 3>& weapon_loots)
        {
            std::cout << "Your current weapons:" << std::endl;

            for (size_t i = 0; i < res.player.weapons.size(); ++i)
            {
                auto& weapon = res.player.weapons.at(i);

                std::cout << weapon.name << ":" << std::endl;

                for (size_t j = 0; j < weapon.attacks.size(); ++j)
                {
                    auto& attack = weapon.attacks.at(j);

                    std::cout << "    " << weapon.attacks.at(j).name << 
                        " (dmg: " << attack.base_dmg << " + " << attack.dmg_die << 
                        ", hit: " << attack.base_hit << " + " << attack.hit_die << ")" << std::endl;
                }

                std::cout << std::endl;
            }

            std::cout << "Choose the option by entering the number" << std::endl;
            std::cout << std::endl;

            for (size_t i = 0; i < weapon_loots.size(); ++i)
            {
                auto& weapon = weapon_loots.at(i);

                std::cout << n + 1 << ". " << weapon.name << ":" << std::endl;

                for (size_t j = 0; j < weapon.attacks.size(); ++j)
                {
                    auto& attack = weapon.attacks.at(j);

                    std::cout << "    " << weapon.attacks.at(j).name << 
                        " (dmg: " << attack.base_dmg << " + " << attack.dmg_die << 
                        ", hit: " << attack.base_hit << " + " << attack.hit_die << ")" << std::endl;
                }

                ++n;
                std::cout << std::endl;
            }
        },
        [&](const std::array<Armor, 3>& armor_loots)
        {
            std::cout << "Your current armor: " << res.player.armor.name << std::endl;
            std::cout << "def: " << res.player.armor.defence << std::endl;
            std::cout << "agi: " << res.player.armor.agility << std::endl;
            std::cout << std::endl;
            
            std::cout << "Choose the option by entering the number" << std::endl;
            std::cout << std::endl;

            for (size_t i = 0; i < armor_loots.size(); ++i)
            {
                auto& armor = armor_loots.at(i);

                std::cout << n + 1 << ". " << armor.name << " (def: " << 
                armor.defence << ", agi: " << armor.agility << ")" << std::endl;
                ++n;
            }
        },
    }, chest_room.loots);

    std::cout << n + 1 << ". Do not take any loot" << std::endl;
    ++n;

    std::cout << n + 1 << ". Pause" << std::endl;
    std::cout << std::endl;

    std::cout << "0. Show this menu again" << std::endl;
    std::cout << std::endl;
}

void CLI::PressEnterToContinue(const std::string& text)
{
    std::cout << text << std::endl;
    
    std::string str;
    std::getline(std::cin, str);
}

std::pair<bool, std::string> CLI::SelectionInput(widget::Selection& selection)
{
    std::cout << horiz_line << std::endl;
    std::cout << "Choose the option by entering the number" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < selection.options.size(); ++i)
    {
        std::cout << i + 1 << ". " << selection.options.at(i) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "0. Back" << std::endl;
    std::cout << std::endl;
    std::cout << "> " << std::flush;

    return selection.Update(std::cin);
}

std::pair<bool, std::string> CLI::ListInput(widget::List& list)
{
    std::cout << horiz_line << std::endl;
    std::cout << "Choose the option by entering the number" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < list.options.size(); ++i)
    {
        std::cout << i + 1 << ". " << list.options.at(i) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "0. Back" << std::endl;
    std::cout << std::endl;
    std::cout << "> " << std::flush;

    return list.Update(std::cin);
}

std::pair<bool, std::string> CLI::PointAllocationInput(widget::PointAllocation& pt_alloc)
{
    std::cout << horiz_line << std::endl;
    std::cout << "Allocate pts by entering in the format of \"X Y\" where X is the option, Y is the new pts allocated" << std::endl;
    std::cout << std::endl;

    std::cout << "Remaining pts: " << pt_alloc.remaining_pts << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < pt_alloc.options.size(); ++i)
    {
        std::cout << i + 1 << ". " << pt_alloc.GetOptionStr(i) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "0. Back" << std::endl;
    std::cout << std::endl;
    std::cout << "> " << std::flush;

    return pt_alloc.Update(std::cin);
}

std::pair<bool, std::string> CLI::TextBoxInput(widget::TextBox& text_box)
{
    std::cout << horiz_line << std::endl;
    std::cout << "Enter the new value (\":c\" to cancel)" << std::endl;
    std::cout << std::endl;

    std::cout << "Currently: " << text_box.text << std::endl;

    std::cout << std::endl;
    std::cout << "> " << std::flush;

    return text_box.Update(std::cin);
}

std::pair<bool, std::string> CLI::EnemySelectionInput(size_t& out_choice, bool& out_is_valid)
{
    out_is_valid = false;

    auto& room = GetData<Type::EnemyRoom>();
    auto& res = room.game.GetResources();
    auto& enemies = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room).enemies;

    std::cout << horiz_line << std::endl;
    std::cout << "Choose the option by entering the number" << std::endl;
    std::cout << std::endl;

    std::cout << "Enemy: " << std::endl;

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        auto& enemy = enemies.at(i);
        std::cout << i + 1 << ". " << enemy.name << " (hp: " << enemy.hp << " / " << enemy.max_hp << ", spd: " << enemy.speed <<
            ", def: " << enemy.armor.defence << ", agi: " << enemy.armor.agility << ")" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "0. Back" << std::endl;
    std::cout << std::endl;
    std::cout << "> " << std::flush;

    std::string str;
    std::getline(std::cin, str);

    if (std::cin.fail() || str.empty() || !std::all_of(str.begin(), str.end(), [](char c){ return isdigit(c); }))
    {
        return std::make_pair(false, std::string("Invalid input"));
    }

    std::stringstream iss(str);

    int choice;
    iss >> choice;

    if (choice == 0)
        return std::make_pair(true, std::string());

    choice -= 1;

    // check if input is valid
    if (iss.fail())
        return std::make_pair(false, std::string("Invalid input"));

    if (choice < 0 || choice >= static_cast<int>(enemies.size()))
        return std::make_pair(false, std::string("Option out of range"));

    out_is_valid = true;
    out_choice = static_cast<size_t>(choice);

    return std::make_pair(true, std::string());
}