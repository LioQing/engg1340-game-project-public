#pragma once

#include <unordered_map>
#include <bitset>
#include <Vec2.h>
#include <Room.h>
#include <MapString.h>
#include <Random.h>
#include <Serializable.h>

struct Level : Serializable
{
    Level() = default;
    Level(const Level&) = default;

    void GotoNextLevel();
    
    void SetSeed(uint32_t seed);
    uint32_t GetSeed() const;

    void AddRoom(const Vec2i& pos, const Room& room);
    bool HasRoom(const Vec2i& pos) const;
    const Room& GetRoom(const Vec2i& pos) const;
    Room& GetRoom(const Vec2i& pos);

    void SetLevel(uint32_t level);
    uint32_t GetLevel() const;

    Vec2i GetNeighbourPos(const Vec2i& pos, Room::Direction dir) const;
    std::bitset<4> HasNeighbours(const Vec2i& pos) const;
    int CountNeighbours(const Vec2i& pos) const;

    MapString GetMapString(const Vec2i& player_pos, Room::Direction dir) const;

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

    static constexpr int room_str_width = 6;
    static constexpr int room_str_height = 3;

private:

    Room GenerateRoom();

    struct Vec2iHash
    {
        size_t operator()(const Vec2i& v) const
        {
            auto x_hash = std::hash<int>()(v.x);
            return x_hash ^ (std::hash<int>()(v.y) + 0x9e3779b9 + (x_hash << 6) + (x_hash >> 2));
        }
    };

    Random rng;

    uint32_t level = 0;
    std::unordered_map<Vec2i, Room, Vec2iHash> rooms;
};