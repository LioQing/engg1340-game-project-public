#pragma once

#include <string>
#include <array>
#include <Attack.h>
#include <Armor.h>
#include <Serializable.h>

struct Weapon : Serializable
{
    std::string name = "Weapon not initialized";

    std::array<Attack, 2> attacks;

    Weapon() = default;
    Weapon(const Weapon&) = default;
    Weapon(const std::string& name, const std::array<Attack, 2>& attacks);

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