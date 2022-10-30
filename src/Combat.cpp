#include <Combat.h>

#include <map>
#include <algorithm>

std::vector<size_t> Combat::TakeTurn(Room::EnemyRoomInfo& room, const Player& player)
{
    std::multimap<int, size_t> enemy_time_actions;

    for (size_t i = 0; i < room.enemies.size(); ++i)
    {
        auto& enemy = room.enemies.at(i);
        enemy.turn_time += enemy.speed;

        if (enemy.turn_time < player.speed)
            continue;
        
        while (enemy.turn_time >= player.speed)
        {
            enemy.turn_time -= player.speed;
            enemy_time_actions.emplace(enemy.turn_time, i);
        }
    }

    std::vector<size_t> enemy_actions(enemy_time_actions.size());
    std::transform(enemy_time_actions.rbegin(), enemy_time_actions.rend(), enemy_actions.begin(),
    [](auto& pair) { return pair.second; });

    return enemy_actions;
}

std::vector<std::string> Combat::TurnGauge(const Room::EnemyRoomInfo& room, const Player& player, size_t count)
{
    auto room_copy = room;

    std::vector<std::string> turn_gauge;
    
    while (turn_gauge.size() < count)
    {
        turn_gauge.emplace_back("You");

        auto enemy_actions = TakeTurn(room_copy, player);
        for (auto& enemy_idx : enemy_actions)
        {
            if (turn_gauge.size() >= count)
                return turn_gauge;
            
            auto& enemy = room_copy.enemies.at(enemy_idx);
            turn_gauge.emplace_back(enemy.name);
        }
    }

    return turn_gauge;
}