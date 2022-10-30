#pragma once

#include <string>
#include <UI.h>

/**
 * @brief Command-line interface (CLI) class
 * 
 */
struct CLI : UI
{
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

private:

    static const std::string horiz_line;

    void OnTypeUpdated() override;

    void OutputMenu();

    void OutputMap();

    void OutputEnemyRoom();

    void OutputChestRoom();

    void PressEnterToContinue(const std::string& text = "--Press Enter to continue--");

    std::pair<bool, std::string> SelectionInput(widget::Selection& selection);

    std::pair<bool, std::string> ListInput(widget::List& list);

    std::pair<bool, std::string> PointAllocationInput(widget::PointAllocation& pt_alloc);
    
    std::pair<bool, std::string> TextBoxInput(widget::TextBox& text_box);

    std::pair<bool, std::string> EnemySelectionInput(size_t& out_choice, bool& out_is_valid);
};