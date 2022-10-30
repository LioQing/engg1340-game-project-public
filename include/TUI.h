#pragma once

#include <variant>
#include <cstdint>
#include <unordered_map>
#include <ttui/ttui.hpp>
#include <UI.h>
#include <tui_data.h>

struct Game;

/**
 * @brief Terminal user interface (TUI) class
 * 
 */
struct TUI : UI
{
    TUI();
    ~TUI();

    /**
     * @brief Output a text to the user
     * 
     * @param text The text to output
     */
    void Output(const std::string& text) override;

    /**
     * @brief Get the user input for the main widget (intended to change or reset the UI)
     * 
     * @return std::optional<size_t> The user input, or std::nullopt if the input is invalid or unfinished
     */
    std::optional<size_t> Input() override;

    /**
     * @brief Called before every update
     * 
     * @param game The game object
     */
    void PreUpdate(Game& game) override;

    /**
     * @brief Called after every update
     * 
     * @param game The game object
     */
    void PostUpdate(Game& game) override;

private:

    void OnTypeUpdated() override;

    void UpdateCursor();

    ttui::Handle handle;
    ttui::Text tooltip_text;
    bool draw_required = true;
    std::variant<
        tui_data::Menu,
        tui_data::Map,
        tui_data::EnemyRoom,
        tui_data::ChestRoom,
        tui_data::DisplayText
    > tui_data;

    static const ttui::Appearance tooltip_appear;
};