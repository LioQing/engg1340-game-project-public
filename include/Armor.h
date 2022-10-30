#pragma once

#include <string>
#include <Serializable.h>

struct Armor : Serializable
{
    std::string name = "Armor not initialized";

    int defence;
    int agility;

    Armor() = default;
    Armor(const Armor&) = default;
    Armor(const std::string& name, int defence, int agility);

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