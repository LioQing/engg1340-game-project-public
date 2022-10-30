#pragma once

#include <cstdint>
#include <vector>
#include <Player.h>
#include <Room.h>

struct Combat
{
    /**
     * @brief Take a turn for the player, which now is enemy's turns
     * 
     * @param room The room object
     * @param player The player object
     * @return std::vector<size_t> The indices of the enemies that are taking their turn in chronological order
     */
    static std::vector<size_t> TakeTurn(Room::EnemyRoomInfo& room, const Player& player);

    /**
     * @brief Get a vector representing the turn gauge as if the player is taking a turn.
     * 
     * @param room The room object
     * @param player The player object
     * @param count The number of turns to get
     * @return std::vector<std::string> The turn gauge
     */
    static std::vector<std::string> TurnGauge(const Room::EnemyRoomInfo& room, const Player& player, size_t count);
    
    /**
     * @brief Attack a target
     * 
     * @tparam Target A class that implement armor and hp
     * @param target The target object
     * @param dmg The raw damage to deal
     * @return int The net damage dealt
     */
    template <typename Target>
    static int Attack(Target& target, int dmg)
    {
        dmg -= target.armor.defence;
        
        if (dmg < 0)
            dmg = 0;
        
        target.hp -= dmg;
        return dmg;
    }
};