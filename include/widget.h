#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <functional>
#include <optional>

namespace widget
{
    /**
     * @brief Widget type
     * 
     */
    enum class Type
    {
        Selection,
        List,
        PointAllocation,
        TextBox,
        Text,
    };

    /**
     * @brief Interface of a widget
     * 
     */
    struct Widget
    {
        std::string title;

        virtual ~Widget() {}

        /**
         * @brief Get the type of the widget
         * 
         * @return Type The type of the widget
         */
        virtual Type GetType() const = 0;

        /**
         * @brief Update the widget with an input stream (intended for CLI use only)
         * 
         * @param input The input stream
         * @return std::pair<bool, std::string> true if the update was successful, false otherwise, second element is an error message
         */
        virtual std::pair<bool, std::string> Update(std::istream& input) = 0;
    };

    /**
     * @brief Template of an option widget (for selection or list)
     * 
     * @tparam T The widget type
     */
    template <Type T>
    struct Options : Widget
    {
        std::vector<std::string> options;
        size_t& curr_option;

        Options() = default;
        Options(const Options&) = default;
        Options(const std::string& title, const std::vector<std::string>& options, size_t& curr_option)
            : options(options), curr_option(curr_option)
        {
            this->title = title;
        }

        Type GetType() const override
        {
            return T;
        }

        std::pair<bool, std::string> Update(std::istream& input) override
        {
            std::string str;
            std::getline(input, str);

            if (input.fail() || str.empty() || !std::all_of(str.begin(), str.end(), [](char c){ return isdigit(c); }))
            {
                return std::make_pair(false, std::string("Invalid input"));
            }

            std::stringstream iss(str);

            int choice;
            iss >> choice;

            if (choice == 0)
                return std::make_pair(true, std::string());

            choice -= 1;

            // check if input is valid
            if (iss.fail())
                return std::make_pair(false, std::string("Invalid input"));

            if (choice < 0 || choice >= static_cast<int>(options.size()))
                return std::make_pair(false, std::string("Option out of range"));

            curr_option = static_cast<size_t>(choice);

            return std::make_pair(true, std::string());
        }
    };

    /**
     * @brief Selection widget
     * 
     */
    using Selection = Options<Type::Selection>;

    /**
     * @brief List widget
     * 
     */
    using List = Options<Type::List>;

    /**
     * @brief Point allocation widget
     * 
     */
    struct PointAllocation : Widget
    {
        struct Option
        {
            std::string name;
            size_t& pts;
            size_t min;
            size_t max;
            size_t multiplier = 1;

            Option() = default;
            Option(const Option&) = default;
            Option(const std::string& name, size_t& pts, size_t min, size_t max, size_t multiplier = 1);
        };

        std::vector<Option> options;
        size_t remaining_pts;

        PointAllocation() = default;
        PointAllocation(const PointAllocation&) = default;
        PointAllocation(const std::string& title, const std::vector<Option>& options, size_t remaining_pts);

        /**
         * @brief Get the formatted string of an option
         * 
         * @param i The index of the option
         * @return std::string The formatted string
         */
        std::string GetOptionStr(size_t i) const;

        Type GetType() const override;

        std::pair<bool, std::string> Update(std::istream& input) override;
    };

    /**
     * @brief Text box widget
     * 
     */
    struct TextBox : Widget
    {
        std::string& text;
        std::function<bool(const std::string&)> validator;
        bool is_minigame = false; // special for mini game

        TextBox() = default;
        TextBox(const TextBox&) = default;
        TextBox(const std::string& title, std::string& text);

        TextBox(const std::string& title, std::string& text, std::function<bool(const std::string&)> validator, bool is_minigame = false);

        Type GetType() const override;

        std::pair<bool, std::string> Update(std::istream& input) override;
    };
}