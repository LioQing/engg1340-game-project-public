#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <array>
#include <Weapon.h>
#include <Armor.h>
#include <Enemy.h>
#include <Serializable.h>

struct Room : Serializable
{
    struct EnemyRoomInfo
    {
        enum ReturnType : size_t
        {
            Pause,
            Victory,
            Defeat,
        };

        std::vector<Enemy> enemies;
    };

    struct ChestRoomInfo
    {
        std::variant<
            std::array<Weapon, 3>,
            std::array<Armor, 3>
            > loots;
            
        bool is_locked = false;
    };

    struct Type
    {
        enum
        {
            None,
            Enemy,
            Chest,
        };
    };

    using NoneRoomInfo = std::monostate;

    using Variant = std::variant<
        NoneRoomInfo,
        EnemyRoomInfo,
        ChestRoomInfo
        >;

    Room() = default;
    Room(const Room&) = default;
    Room(const Variant& room, bool is_exit = false);

    Variant room = std::monostate();
    bool is_exit = false;

    enum Direction : uint8_t
    {
        North,
        South,
        East,
        West,
        Count,

        Pause = Count,
    };

    /**
     * @brief Convert direction to string
     * 
     * @param dir The direction
     * @return std::string The string representation of the direction
     */
    static std::string DirectionToString(uint8_t dir);
    
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