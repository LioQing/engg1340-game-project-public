#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <Vec2.h>
#include <Weapon.h>
#include <Serializable.h>

/**
 * @brief create struct for Player
 * 
 * @param hp
 * @param max_hp
 * @param speed
 * @param armor
 * @param weapon
 * @param room_pos To store the position of player (which room the player is in)
 * 
 */
struct Player : Serializable
{
    int hp = 1;
    int max_hp = 1;
    int hp_regen = 1;
    int speed = 1;
    Armor armor;
    std::vector<Weapon> weapons;

    Vec2i room_pos;

    Player() = default;
    Player(const Player&) = default;
    Player(int hp, int max_hp, int hp_regen, int speed, const Armor& armor, const std::vector<Weapon>& weapons, const Vec2i& room_pos = Vec2i());

    /**
     * @brief Serialize the data to a stream
     * 
     * @return std::ostream The serialized data
     */
    std::ostringstream Serialize() const override;

    /**
     * @brief Deserialize the data from an istream
     * 
     * @param data The serialized data
     */
    void Deserialize(std::istream& data) override;
};