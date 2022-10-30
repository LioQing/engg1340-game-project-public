#pragma once

#include <cstdint>
#include <Attack.h>
#include <Armor.h>
#include <Serializable.h>

struct Enemy : Serializable
{
    std::string name = "Enemy not initialized";
    int hp;
    int max_hp;
    int speed;
    Armor armor;
    Attack attack;

    int turn_time = 0;

    Enemy() = default;
    Enemy(const Enemy&) = default;
    Enemy(const std::string& name, int hp, int speed, const Armor& armor, const Attack& attack);
    
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