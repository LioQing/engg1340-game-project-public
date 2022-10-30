#include <Enemy.h>

#include <Serial.h>

Enemy::Enemy(const std::string& name, int hp, int speed, const Armor& armor, const Attack& attack)
    : name(name)
    , hp(hp)
    , max_hp(hp)
    , speed(speed)
    , armor(armor)
    , attack(attack)
{
}

std::ostringstream Enemy::Serialize() const
{
    std::ostringstream oss;

    oss << Serial<std::string>::Serialize(name).str();
    oss << Serial<int>::Serialize(hp).str();
    oss << Serial<int>::Serialize(max_hp).str();
    oss << Serial<int>::Serialize(speed).str();
    oss << armor.Serialize().str();
    oss << attack.Serialize().str();

    return oss;
}

void Enemy::Deserialize(std::istream& data)
{
    name = Serial<std::string>::Deserialize(data);
    hp = Serial<int>::Deserialize(data);
    max_hp = Serial<int>::Deserialize(data);
    speed = Serial<int>::Deserialize(data);
    armor.Deserialize(data);
    attack.Deserialize(data);
}