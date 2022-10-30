#include <Attack.h>

#include <Serial.h>

Attack::Attack(const std::string& name, int base_dmg, int dmg_die, int base_hit, int hit_die)
    : name(name), base_dmg(base_dmg), dmg_die(dmg_die), base_hit(base_hit), hit_die(hit_die)
{
}

std::ostringstream Attack::Serialize() const
{
    std::ostringstream oss;

    oss
        << Serial<std::string>::Serialize(name).str()
        << Serial<int>::Serialize(base_dmg).str()
        << Serial<int>::Serialize(dmg_die).str()
        << Serial<int>::Serialize(base_hit).str()
        << Serial<int>::Serialize(hit_die).str();

    return oss;
}

void Attack::Deserialize(std::istream& data)
{
    name = Serial<std::string>::Deserialize(data);
    base_dmg = Serial<int>::Deserialize(data);
    dmg_die = Serial<int>::Deserialize(data);
    base_hit = Serial<int>::Deserialize(data);
    hit_die = Serial<int>::Deserialize(data);
}