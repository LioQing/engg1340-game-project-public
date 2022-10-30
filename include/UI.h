#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <type_traits>
#include <memory>
#include <widget.h>
#include <Level.h>

struct Game;

/**
 * @brief Interface of a user interface class
 * 
 */
struct UI
{
    enum class Type 
    {
        Menu,
        Map,
        EnemyRoom,
        ChestRoom,
        DisplayText,

        Uninitialized,
    };

    virtual ~UI() {}

    /**
     * @brief Set the UI to Menu type
     * 
     * @param title The title of the menu
     * @param options The options of the menu
     * @param widgets The optional widgets of the menu
     */
    template <typename... T>
    void SetMenu(const std::string& title, const std::vector<std::string>& options, T&&... widgets)
    {
        type = Type::Menu;

        auto& menu = data.emplace<Menu>();

        menu.title = title;
        menu.options = options;

        menu.widgets.clear();
        (menu.widgets.push_back(std::make_unique<typename std::remove_reference<T>::type>(std::forward<T>(widgets))), ...);

        OnTypeUpdated();
    }

    /**
     * @brief Set the UI to Map type
     * 
     * @param game The game object
     */
    void SetMap(Game& game);

    /**
     * @brief Set the UI to EnemyRoom type
     * 
     * @param game The game object
     */
    void SetEnemy(Game& game);

    /**
     * @brief Set the UI to ChestRoom type
     * 
     * @param game The game object
     */
    void SetChest(Game& game);

    /**
     * @brief Set the UI to Display Text type
     * 
     * @param text The text to display
     */
    void SetDisplayText(const std::string& text);

    /**
     * @brief Get the type of the UI
     * 
     * @return Type The type of the UI
     */
    Type GetType() const;

    /**
     * @brief Get the data associated with the UI type
     * 
     * @tparam T The UI type
     * @return auto& The data associated with the UI type
     */
    template <Type T>
    const auto& GetData() const
    {
        return std::get<static_cast<typename std::underlying_type<Type>::type>(T)>(data);
    }

    /**
     * @brief Output a text to the user
     * 
     * @param text The text to output
     */
    virtual void Output(const std::string& text) = 0;

    /**
     * @brief Get the user input for the main widget (to change or reset the UI)
     * 
     * @return std::optional<size_t> The user input, or std::nullopt if the input is invalid or unfinished
     */
    virtual std::optional<size_t> Input() = 0;
    
    /**
     * @brief Called before every update
     * 
     * @param game The game object
     */
    virtual void PreUpdate(Game& game) {}

    /**
     * @brief Called after every update
     * 
     * @param game The game object
     */
    virtual void PostUpdate(Game& game) {}

    struct Menu
    {
        std::string title;
        std::vector<std::string> options;

        std::vector<std::unique_ptr<widget::Widget>> widgets;

        Menu() = default;
        Menu(Menu&&) = default;
    };

    struct GameInfo
    {
        Game& game;

        GameInfo(Game& game);
    };

    using Map = GameInfo;
    using EnemyRoom = GameInfo;
    using ChestRoom = GameInfo;

    struct DisplayText
    {
        std::string text;

        DisplayText() = default;
        DisplayText(const std::string& text);
    };

protected:

    /**
     * @brief Called on type assigned
     * 
     * @param type The new type
     */
    virtual void OnTypeUpdated() {}

    /**
     * @brief Get the data associated with the UI type
     * 
     * @tparam T The UI type
     * @return auto& The data associated with the UI type
     */
    template <Type T>
    auto& GetData()
    {
        return std::get<static_cast<typename std::underlying_type<Type>::type>(T)>(data);
    }

private:

    Type type = Type::Uninitialized;
    std::variant<Menu, Map, EnemyRoom, ChestRoom, DisplayText> data;
};