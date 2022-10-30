#include <UI.h>

void UI::SetMap(Game& game)
{
    type = Type::Map;

    data.emplace<static_cast<size_t>(Type::Map)>(game);

    OnTypeUpdated();
}

void UI::SetEnemy(Game& game)
{
    type = Type::EnemyRoom;

    data.emplace<static_cast<size_t>(Type::EnemyRoom)>(game);

    OnTypeUpdated();
}

void UI::SetChest(Game& game)
{
    type = Type::ChestRoom;
    
    data.emplace<static_cast<size_t>(Type::ChestRoom)>(game);

    OnTypeUpdated();
}

void UI::SetDisplayText(const std::string& text)
{
    type = Type::DisplayText;

    data.emplace<static_cast<size_t>(Type::DisplayText)>(text);

    OnTypeUpdated();
}

UI::Type UI::GetType() const
{
    return type;
}

UI::GameInfo::GameInfo(Game& game) : game(game) 
{
}

UI::DisplayText::DisplayText(const std::string& text) : text(text)
{    
}