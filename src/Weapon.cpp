#include <Weapon.h>

#include <Serial.h>

Weapon::Weapon(const std::string& name, const std::array<Attack, 2>& attacks)
    : name(name), attacks(attacks)
{
}

std::ostringstream Weapon::Serialize() const
{
    std::ostringstream oss;

    oss
        << Serial<std::string>::Serialize(name).str()
        << Serial<std::array<Attack, 2>>::Serialize(attacks).str();

    return oss;
}

void Weapon::Deserialize(std::istream& data)
{
    name = Serial<std::string>::Deserialize(data);
    attacks = Serial<std::array<Attack, 2>>::Deserialize(data);
}