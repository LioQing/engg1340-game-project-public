#include <Armor.h>

#include <Serial.h>

Armor::Armor(const std::string& name, int defence, int agility)
    : name(name), defence(defence), agility(agility)
{
}

std::ostringstream Armor::Serialize() const
{
    std::ostringstream oss;

    oss
        << Serial<std::string>::Serialize(name).str()
        << Serial<int>::Serialize(defence).str()
        << Serial<int>::Serialize(agility).str();

    return oss;
}

void Armor::Deserialize(std::istream& data)
{
    name = Serial<std::string>::Deserialize(data);
    defence = Serial<int>::Deserialize(data);
    agility = Serial<int>::Deserialize(data);
}