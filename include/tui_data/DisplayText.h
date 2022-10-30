#pragma once

#include <chrono>
#include <ttui/ttui.hpp>
#include <UI.h>

namespace tui_data
{
    struct DisplayText
    {
        using Clock = std::chrono::high_resolution_clock;

        ttui::Grid grid;
        std::optional<size_t> input;
        std::chrono::time_point<Clock> start_time;
        bool finished_anim = false;

        bool* draw_required;
        UI::DisplayText* text;
        ttui::Text* tooltip_text;

        DisplayText() = default;
        DisplayText(DisplayText&&) = default;

        void InitWidgets();

        void PollEvent(Game& game, ttui::Handle& handle);

        void UpdateWidgets();

        std::string GetCurrText();

        static constexpr uint16_t column_width = 64;

        static const std::string tooltip;

        static constexpr int64_t ms_per_char = 30;
    };
}