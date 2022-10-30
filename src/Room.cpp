#include <Room.h>

#include <Serial.h>
#include <Visitor.h>

Room::Room(const Variant& room, bool is_exit) : room(room), is_exit(is_exit)
{
}

std::string Room::DirectionToString(uint8_t dir)
{
    switch (dir)
    {
        case North: return "North";
        case South: return "South";
        case East:  return "East";
        case West:  return "West";
        case Pause: return "Pause";
        default:    return "Unknown";
    }
}

std::ostringstream Room::Serialize() const
{
    std::ostringstream oss;

    oss << Serial<uint32_t>::Serialize(room.index()).str();

    std::visit(Visitor {
        [&oss](const NoneRoomInfo&) {},
        [&oss](const EnemyRoomInfo& info) { oss << Serial<std::vector<Enemy>>::Serialize(info.enemies).str(); },
        [&oss](const ChestRoomInfo& info) {
            oss << Serial<uint32_t>::Serialize(info.loots.index()).str();
            std::visit(Visitor {
                [&oss](const std::array<Weapon, 3>& loots) { oss << Serial<std::array<Weapon, 3>>::Serialize(loots).str(); },
                [&oss](const std::array<Armor, 3>& loots) { oss << Serial<std::array<Armor, 3>>::Serialize(loots).str(); },
            }, info.loots);

            oss << Serial<bool>::Serialize(info.is_locked).str();
        },
    }, room);

    oss << Serial<bool>::Serialize(is_exit).str();

    return oss;
}

void Room::Deserialize(std::istream& data)
{
    auto index = Serial<uint32_t>::Deserialize(data);
    if      (index == 0) room = NoneRoomInfo {};
    else if (index == 1) room = EnemyRoomInfo {};
    else if (index == 2) room = ChestRoomInfo {};
    else                 room = NoneRoomInfo {};

    std::visit(Visitor {
        [&data](NoneRoomInfo&) {},
        [&data](EnemyRoomInfo& info) { info.enemies = Serial<std::vector<Enemy>>::Deserialize(data); },
        [&data](ChestRoomInfo& info) {
            auto index = Serial<uint32_t>::Deserialize(data);
            if      (index == 0) info.loots = std::array<Weapon, 3> {};
            else if (index == 1) info.loots = std::array<Armor, 3> {};

            std::visit(Visitor {
                [&data](std::array<Weapon, 3>& loots) { loots = Serial<std::array<Weapon, 3>>::Deserialize(data); },
                [&data](std::array<Armor, 3>& loots) { loots = Serial<std::array<Armor, 3>>::Deserialize(data); },
            }, info.loots);

            info.is_locked = Serial<bool>::Deserialize(data);
        },
    }, room);

    is_exit = Serial<bool>::Deserialize(data);
}