#include <TUI.h>

#include <Game.h>
#include <numeric>

const ttui::Appearance TUI::tooltip_appear = ttui::Appearance(ttui::Color::Black(), ttui::Color::White());

TUI::TUI()
{
    handle.Init();
    ttui::screen::Clear();
    ttui::cursor::SetVisible(false);

    tooltip_text.SetBackgroundColor(tooltip_appear.bg_color);
    tooltip_text.SetBorder(ttui::Border::None());
    tooltip_text.SetParagraph(ttui::Paragraph("", tooltip_appear));
}

TUI::~TUI()
{
    ttui::cursor::SetVisible(true);
    ttui::screen::Clear();
}

void TUI::Output(const std::string& text)
{
    if (GetType() == Type::Menu)
    {
        std::get<tui_data::Menu>(tui_data).Output(text);
    }
    else if (GetType() == Type::Map)
    {
        std::get<tui_data::Map>(tui_data).Output(text);
    }
    else if (GetType() == Type::EnemyRoom)
    {
        std::get<tui_data::EnemyRoom>(tui_data).Output(text);
    }
    else if (GetType() == Type::ChestRoom)
    {
        std::get<tui_data::ChestRoom>(tui_data).Output(text);
    }
}

std::optional<size_t> TUI::Input()
{
    return std::visit([&](const auto& data){ return data.input; }, tui_data);
}

void TUI::PreUpdate(Game& game)
{
    std::visit([&](auto& data){ return data.PollEvent(game, handle); }, tui_data);
}

void TUI::PostUpdate(Game& game)
{
    if (!draw_required)
        return;
        
    ttui::cursor::SetVisible(false);

    if (GetType() == Type::Menu)
    {
        auto& menu = std::get<tui_data::Menu>(tui_data);
        handle.Render(menu.grid, ttui::Rect(0, 0, handle.GetWidth(), handle.GetHeight() - 1));
        handle.Render(ttui::Empty(), ttui::Rect(0, 0, handle.GetWidth(), handle.GetHeight() - 1));
        handle.Render(tooltip_text, ttui::Rect(0, handle.GetHeight() - 1, handle.GetWidth(), 1));
        handle.Draw();
        UpdateCursor();
    }
    else if (GetType() == Type::Map)
    {
        auto& map = std::get<tui_data::Map>(tui_data);
        handle.Render(map.layout, ttui::Rect(0, 0, handle.GetWidth(), handle.GetHeight() - 1));
        handle.Render(tooltip_text, ttui::Rect(0, handle.GetHeight() - 1, handle.GetWidth(), 1));
        handle.Draw();
    }
    else if (GetType() == Type::EnemyRoom)
    {
        auto& room = std::get<tui_data::EnemyRoom>(tui_data);

        handle.Render(room.turn_gauge, ttui::Rect(0, 0, handle.GetWidth(), 3));

        if (
            room.stage == tui_data::EnemyRoom::Stage::Attack ||
            room.stage == tui_data::EnemyRoom::Stage::Target
            )
        {
            handle.Render(room.enemies_layout, room.GetEnemyViewRect());
            handle.Render(room.layout, ttui::Rect(0, 3, handle.GetWidth(), handle.GetHeight() - 4));
        }
        else // dice rolling
        {
            handle.Render(room.fullscreen_text, ttui::Rect(0, 3, handle.GetWidth(), handle.GetHeight() - 4));
        }

        handle.Render(tooltip_text, ttui::Rect(0, handle.GetHeight() - 1, handle.GetWidth(), 1));
        handle.Draw();
    }
    else if (GetType() == Type::ChestRoom)
    {
        auto& room = std::get<tui_data::ChestRoom>(tui_data);

        handle.Render(room.loots_layout, room.GetLootsViewRect());

        if (!room.tmp_input)
            handle.Render(room.no_take_text, room.GetNoTakeTextRect());
            
        handle.Render(room.layout, ttui::Rect(0, 0, handle.GetWidth(), handle.GetHeight() - 1));
        handle.Render(tooltip_text, ttui::Rect(0, handle.GetHeight() - 1, handle.GetWidth(), 1));
        handle.Draw();
    }
    else if (GetType() == Type::DisplayText)
    {
        auto& text = std::get<tui_data::DisplayText>(tui_data);
        handle.Render(text.grid, ttui::Rect(0, 0, handle.GetWidth(), handle.GetHeight() - 1));
        handle.Render(ttui::Empty(), ttui::Rect(0, 0, handle.GetWidth(), handle.GetHeight() - 1));
        handle.Render(tooltip_text, ttui::Rect(0, handle.GetHeight() - 1, handle.GetWidth(), 1));
        handle.Draw();
    }

    draw_required = false;
}

void TUI::UpdateCursor()
{
    if (GetType() == Type::Menu)
    {
        std::get<tui_data::Menu>(tui_data).UpdateCursor();
    }
}

void TUI::OnTypeUpdated()
{
    if (GetType() == Type::Menu)
    {
        auto& tui_menu = tui_data.emplace<tui_data::Menu>();
        tui_menu.draw_required = &draw_required;
        tui_menu.menu = &GetData<Type::Menu>();
        tui_menu.tooltip_text = &tooltip_text;

        tui_menu.InitWidgets();
    }
    else if (GetType() == Type::Map)
    {
        auto& tui_map = tui_data.emplace<tui_data::Map>();
        tui_map.draw_required = &draw_required;
        tui_map.map = &GetData<Type::Map>();
        tui_map.tooltip_text = &tooltip_text;
        tui_map.handle = &handle;

        tui_map.InitWidgets();
    }
    else if (GetType() == Type::EnemyRoom)
    {
        auto& tui_map = tui_data.emplace<tui_data::EnemyRoom>();
        tui_map.draw_required = &draw_required;
        tui_map.enemy_room = &GetData<Type::EnemyRoom>();
        tui_map.tooltip_text = &tooltip_text;
        tui_map.handle = &handle;

        tui_map.InitWidgets();
    }
    else if (GetType() == Type::ChestRoom)
    {
        auto& tui_map = tui_data.emplace<tui_data::ChestRoom>();
        tui_map.draw_required = &draw_required;
        tui_map.chest_room = &GetData<Type::ChestRoom>();
        tui_map.tooltip_text = &tooltip_text;
        tui_map.handle = &handle;

        tui_map.InitWidgets();
    }
    else if (GetType() == Type::DisplayText)
    {
        auto& tui_map = tui_data.emplace<tui_data::DisplayText>();
        tui_map.draw_required = &draw_required;
        tui_map.text = &GetData<Type::DisplayText>();
        tui_map.tooltip_text = &tooltip_text;

        tui_map.InitWidgets();
    }
}