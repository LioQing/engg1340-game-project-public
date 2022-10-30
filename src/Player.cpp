#include <Player.h>

#include <Serial.h>

/**
 * @brief Construct a new Player:: Player object
 * 
 * @param hp 
 * @param max_hp 
 * @param hp_regen 
 * @param speed 
 * @param armor 
 * @param weapons 
 * @param room_pos 
 */

Player::Player(int hp, int max_hp, int hp_regen, int speed, const Armor& armor, const std::vector<Weapon>& weapons, const Vec2i& room_pos)
    : hp(hp)
    , max_hp(max_hp)
    , hp_regen(hp_regen)
    , speed(speed)
    , armor(armor)
    , weapons(weapons)
    , room_pos(room_pos)
{
}

std::ostringstream Player::Serialize() const
{
    std::ostringstream oss;

    oss
        << Serial<int>::Serialize(hp).str()
        << Serial<int>::Serialize(max_hp).str()
        << Serial<int>::Serialize(hp_regen).str()
        << Serial<int>::Serialize(speed).str()
        << Serial<Armor>::Serialize(armor).str()
        << Serial<std::vector<Weapon>>::Serialize(weapons).str()
        << Serial<Vec2i>::Serialize(room_pos).str();

    return oss;
}

void Player::Deserialize(std::istream& data)
{
    hp = Serial<int>::Deserialize(data);
    max_hp = Serial<int>::Deserialize(data);
    hp_regen = Serial<int>::Deserialize(data);
    speed = Serial<int>::Deserialize(data);
    armor = Serial<Armor>::Deserialize(data);
    weapons = Serial<std::vector<Weapon>>::Deserialize(data);
    room_pos = Serial<Vec2i>::Deserialize(data);
}