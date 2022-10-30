#pragma once

#include <Level.h>
#include <Player.h>
#include <Serializable.h>

struct Resources : Serializable
{
    Level level;
    Player player;

    Resources() = default;
    Resources(const Resources&) = default;
    Resources(const Level& level, const Player& player);
    
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