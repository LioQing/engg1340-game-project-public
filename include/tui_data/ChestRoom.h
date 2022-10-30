#pragma once

#include <ttui/ttui.hpp>
#include <UI.h>

namespace tui_data
{
    struct ChestRoom
    {
        ttui::Layout layout;
        ttui::Layout loots_layout;
        ttui::Text no_take_text;
        std::optional<size_t> input;
        std::optional<size_t> tmp_input;
        size_t selected_loot = 0;

        bool* draw_required;
        UI::ChestRoom* chest_room;
        ttui::Text* tooltip_text;
        ttui::Handle* handle;
        
        ChestRoom() = default;
        ChestRoom(const ChestRoom&) = default;

        void InitWidgets();

        void UpdateSize();

        void UpdateLootView();

        void Output(const std::string& text);

        void PollEvent(Game& game, ttui::Handle& handle);

        ttui::Rect GetLootsViewRect() const;

        ttui::Rect GetNoTakeTextRect() const;

        ttui::Paragraph GetPlayerPara() const;
    };
}