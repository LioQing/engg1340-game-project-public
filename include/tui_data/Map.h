#pragma once

#include <optional>
#include <ttui/ttui.hpp>
#include <UI.h>

namespace tui_data
{
    struct Map
    {
        ttui::Layout layout;
        std::optional<size_t> input;
        Room::Direction dir_selected = Room::Pause;

        bool* draw_required;
        UI::Map* map;
        ttui::Text* tooltip_text;
        ttui::Handle* handle;

        Map() = default;
        Map(const Map&) = default;

        void InitWidgets();

        void UpdateSize();

        void Output(const std::string& text);

        void PollEvent(Game& game, ttui::Handle& handle);

        ttui::Paragraph GenerateMapStrPara();

        static const std::string tooltip;
    };
}