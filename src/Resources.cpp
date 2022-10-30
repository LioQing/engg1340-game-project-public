#include <Resources.h>

Resources::Resources(const Level& level, const Player& player)
    : level(level), player(player)
{
}

std::ostringstream Resources::Serialize() const
{
    std::ostringstream oss;

    oss
        << level.Serialize().str()
        << player.Serialize().str();

    return oss;
}

void Resources::Deserialize(std::istream& data)
{
    level.Deserialize(data);
    player.Deserialize(data);
}