#pragma once

#include <optional>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <ttui/ttui.hpp>
#include <widget.h>
#include <UI.h>

namespace tui_data
{
    struct Menu
    {
        ttui::Grid grid;
        std::optional<size_t> input;

        size_t curr_widget; // if index is widgets.size then it is the menu list
        std::unordered_map<size_t, size_t> list_widget_curr_item;
        std::unordered_map<size_t, size_t> pt_alloc_widget_curr_item;
        std::unordered_map<size_t, size_t> text_box_widget_curr_idx;

        bool* draw_required;
        UI::Menu* menu;
        ttui::Text* tooltip_text;

        Menu() = default;
        Menu(Menu&&) = default;

        void InitWidgets();

        void UpdateWidgets();

        void UpdateCursor();

        void Output(const std::string& text);

        void PollEvent(Game& game, ttui::Handle& handle);

        // key events

        void KeyEvent(const ttui::KeyEvent& event);

        void SelectionKeyEvent(widget::Selection& selection, ttui::Text& tui_widget, const ttui::KeyEvent& event);

        void ListKeyEvent(widget::List& list, ttui::List& tui_widget, const ttui::KeyEvent& event);

        void PointAllocationKeyEvent(widget::PointAllocation& pt_alloc, ttui::List& tui_widget, const ttui::KeyEvent& event);

        void TextBoxKeyEvent(widget::TextBox& text_box, ttui::Text& tui_widget, const ttui::KeyEvent& event);

        static constexpr uint16_t column_width = 64;

        static const std::string tooltip;
        static const std::string menu_tooltip;
        static const std::string selection_tooltip;
        static const std::string list_tooltip;
        static const std::string point_allocation_tooltip;
        static const std::string text_box_tooltip;
        static const std::string text_box_enter_tooltip;

        static const ttui::Appearance curr_appear;
        static const ttui::Appearance list_selected_appear;
    };
}