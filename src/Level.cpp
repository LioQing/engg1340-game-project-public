#include <Level.h>

#include <limits>
#include <numeric>
#include <vector>
#include <algorithm>
#include <GameData.h>
#include <Serial.h>

void Level::GotoNextLevel()
{
    rooms.clear();
    
    ++level;

    auto dir = Vec2i(rng.GetInt(0, 1) ? 1 : -1, rng.GetInt(0, 1) ? 1 : -1);
    std::vector<Vec2i> available_rooms = { Vec2i(0, -1), Vec2i(0, 1), Vec2i(1, 0), Vec2i(-1, 0) };
    auto curr_pos = Vec2i(0, 0);
    AddRoom(curr_pos, Room());

    // main rooms
    for (int i = 0; i < 4; ++i)
    {
        int axis = rng.GetInt(0, 1);
        curr_pos += axis == 0 ? Vec2i(dir.x, 0) : Vec2i(0, dir.y);

        Room room = GenerateRoom();
        if (i == 3)
            room.is_exit = true;

        AddRoom(curr_pos, room);

        // update available rooms
        available_rooms.erase(std::find(available_rooms.begin(), available_rooms.end(), curr_pos));
        for (auto& pos : { Vec2i(0, -1), Vec2i(0, 1), Vec2i(1, 0), Vec2i(-1, 0) })
        {
            if (!HasRoom(curr_pos + pos) && std::find(available_rooms.begin(), available_rooms.end(), curr_pos + pos) == available_rooms.end())
            {
                available_rooms.emplace_back(curr_pos + pos);
            }
        }
    }

    // side rooms
    for (int i = 0; i < 4; ++i)
    {
        int j = rng.GetInt(0, available_rooms.size() - 1);
        AddRoom(available_rooms.at(j), GenerateRoom());
        available_rooms.erase(available_rooms.begin() + j);
    }
}

void Level::SetSeed(uint32_t seed)
{
    rng.SetSeed(seed);
}

uint32_t Level::GetSeed() const
{
    return rng.GetSeed();
}

void Level::AddRoom(const Vec2i& pos, const Room& room)
{
    rooms.emplace(pos, room);
}

bool Level::HasRoom(const Vec2i& pos) const
{
    return rooms.find(pos) != rooms.end();
}

const Room& Level::GetRoom(const Vec2i& pos) const
{
    return rooms.at(pos);
}

Room& Level::GetRoom(const Vec2i& pos)
{
    return rooms.at(pos);
}

void Level::SetLevel(uint32_t level)
{
    this->level = level;
}

uint32_t Level::GetLevel() const
{
    return level;
}

Vec2i Level::GetNeighbourPos(const Vec2i& pos, Room::Direction dir) const
{
    if      (dir == Room::North) return pos + Vec2i(0, -1);
    else if (dir == Room::South) return pos + Vec2i(0, 1);
    else if (dir == Room::East)  return pos + Vec2i(1, 0);
    else if (dir == Room::West)  return pos + Vec2i(-1, 0);

    throw std::out_of_range("Invalid direction");
}

std::bitset<4> Level::HasNeighbours(const Vec2i& pos) const
{
    std::bitset<4> result = 0;

    if (HasRoom(GetNeighbourPos(pos, Room::North))) result.set(0);
    if (HasRoom(GetNeighbourPos(pos, Room::South))) result.set(1);
    if (HasRoom(GetNeighbourPos(pos, Room::East)))  result.set(2);
    if (HasRoom(GetNeighbourPos(pos, Room::West)))  result.set(3);

    return result;
}

int Level::CountNeighbours(const Vec2i& pos) const
{
    return HasNeighbours(pos).count();
}

MapString Level::GetMapString(const Vec2i& player_pos, Room::Direction dir) const
{
    MapString map_str;

    auto generate_room_str = [&](const Vec2i& pos, bool highlighted) -> std::vector<std::string>
    {
        static const std::string normal_path_char[] =
        {
            "\u2534", "\u252c", "\u251c\u2500", "\u2524",
        };

        static const std::vector<std::string> normal_room_str =
        {
            "\u250c\u2500\u2500\u2500\u2510 ", // ┌───┐
            "\u2502   \u2502 ",                // │   │
            "\u2514\u2500\u2500\u2500\u2518 ", // └───┘
        };

        static const std::string highlighted_path_char[] =
        {
            "\u2567", "\u2564", "\u255f\u2500", "\u2562",
        };

        static const std::vector<std::string> highlighted_room_str =
        {
            "\u2554\u2550\u2550\u2550\u2557 ", // ╔═══╗
            "\u2551   \u2551 ",                // ║   ║
            "\u255a\u2550\u2550\u2550\u255d ", // ╚═══╝
        };

        if (!HasRoom(pos))
        {
            return std::vector<std::string>(room_str_height, std::string(room_str_width, ' '));
        }

        const auto& path_char = highlighted ? highlighted_path_char : normal_path_char;

        auto room_str = highlighted ? highlighted_room_str : normal_room_str;

        // replace walls with path char according to neighbours
        auto has_neighbours = HasNeighbours(pos);

        if (has_neighbours.test(0)) room_str.at(0).replace(6, path_char[0].size(), path_char[0]);
        if (has_neighbours.test(1)) room_str.at(2).replace(6, path_char[1].size(), path_char[1]);
        if (has_neighbours.test(2)) room_str.at(1).replace(6, path_char[2].size(), path_char[2]);
        if (has_neighbours.test(3)) room_str.at(1).replace(0, path_char[3].size(), path_char[3]);

        if      (pos == player_pos)    room_str.at(1).at(4) = 'X';
        else if (GetRoom(pos).is_exit) room_str.at(1).at(4) = 'E';

        return room_str;
    };

    // find room with min and max x and y pos
    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max();
    int max_y = std::numeric_limits<int>::min();
    for (const auto& room : rooms)
    {
        const Vec2i& pos = room.first;
        if (pos.x < min_x) min_x = pos.x;
        if (pos.x > max_x) max_x = pos.x;
        if (pos.y < min_y) min_y = pos.y;
        if (pos.y > max_y) max_y = pos.y;
    }

    // set width and height as the number of rooms in each direction
    map_str.width = max_x - min_x + 1;
    map_str.height = max_y - min_y + 1;

    std::vector<std::string> room_strs(map_str.height * room_str_height, std::string());
    for (size_t y = 0; y < static_cast<size_t>(map_str.height); ++y)
    for (size_t x = 0; x < static_cast<size_t>(map_str.width); ++x)
    {
        size_t i = y * room_str_height;
        int x_pos = x + min_x;
        int y_pos = y + min_y;

        auto is_highlighted = dir == Room::Pause ? false : GetNeighbourPos(player_pos, dir) == Vec2i(x_pos, y_pos);
        auto room_str = generate_room_str(Vec2i(x_pos, y_pos), is_highlighted);

        // append room_str to room_strs
        for (size_t j = 0; j < room_str_height; ++j)
        {
            room_strs.at(i + j) += room_str.at(j);
        }
    }

    // join room_strs with newlines to map_str.str
    for (size_t i = 0; i < room_strs.size(); ++i)
    {
        if (i != 0) map_str.str += "\n";
        map_str.str += room_strs.at(i);
    }

    return map_str;
}

std::ostringstream Level::Serialize() const
{
    std::ostringstream oss;

    oss 
        << rng.Serialize().str()
        << Serial<uint32_t>::Serialize(level).str()
        << Serial<std::unordered_map<Vec2i, Room, Vec2iHash>>::Serialize(rooms).str();

    return oss;
}

void Level::Deserialize(std::istream& data)
{
    rng.Deserialize(data);
    level = Serial<uint32_t>::Deserialize(data);
    rooms = Serial<std::unordered_map<Vec2i, Room, Vec2iHash>>::Deserialize(data);
}

Room Level::GenerateRoom()
{
    constexpr int enemy_room_prop = 75;
    constexpr int weapon_chest_room_prop = 60;

    Room room;
    if (rng.GetInt(0, 100) < enemy_room_prop) // enemy room
    {
        auto enemy_room = Room::EnemyRoomInfo();
        auto enemy_count = rng.GetInt(1, 3);
        auto enemy_names = GameData::GetEnemies(level);
        for (int i = 0; i < enemy_count; ++i)
        {
            auto enemy_idx = rng.GetInt(0, enemy_names.size() - 1);
            auto enemy = GameData::GetEnemy(enemy_names.at(enemy_idx), level, rng);
            enemy_room.enemies.emplace_back(enemy);
        }

        room = Room(enemy_room);
    }
    else // chest room
    {
        auto chest_room = Room::ChestRoomInfo();

        if (rng.GetInt(0, 100) < weapon_chest_room_prop) // weapon
        {
            auto loots = std::array<Weapon, 3>();
            auto weapon_names = GameData::GetWeapons(level);
            for (int i = 0; i < 3; ++i)
            {
                auto weapon_idx = rng.GetInt(0, weapon_names.size() - 1);
                loots.at(i) = GameData::GetWeapon(weapon_names.at(weapon_idx), level, rng);
                if (weapon_names.size() > 1)
                    weapon_names.erase(weapon_names.begin() + weapon_idx);
            }
            chest_room.loots = loots;
        }
        else // armor
        {
            auto loots = std::array<Armor, 3>();
            auto armor_names = GameData::GetArmors(level);
            for (int i = 0; i < 3; ++i)
            {
                auto armor_idx = rng.GetInt(0, armor_names.size() - 1);
                loots.at(i) = GameData::GetArmor(armor_names.at(armor_idx), level, rng);
                if (armor_names.size() > 1)
                    armor_names.erase(armor_names.begin() + armor_idx);
            }
            chest_room.loots = loots;
        }

        chest_room.is_locked = rng.GetInt(0, 1) == 0;

        room = Room(chest_room);
    }

    return room;
}