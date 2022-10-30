#pragma once

#include <string>
#include <Serializable.h>

struct Attack : Serializable
{
    std::string name = "Attack not initialized";

    int base_dmg = 1;
    int dmg_die = 1;
    int base_hit = 1;
    int hit_die = 1;

    Attack() = default;
    Attack(const Attack&) = default;
    Attack(const std::string& name, int base_dmg, int dmg_die, int base_hit, int hit_die);

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