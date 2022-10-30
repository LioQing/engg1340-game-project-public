#pragma once

#include <optional>
#include <vector>
#include <ttui/ttui.hpp>
#include <UI.h>

namespace tui_data
{
    struct EnemyRoom
    {
        enum class Stage
        {
            Attack,
            Target,
            HitDie,
            DamageDie,
            PostAttack,
            EnemyActions,
        };

        ttui::Layout layout;
        ttui::Layout enemies_layout;
        ttui::Text fullscreen_text;
        ttui::Text turn_gauge;
        std::optional<size_t> input;
        size_t selected_item = 0;
        Stage stage;
        std::string esc_tooltip;

        std::optional<int> hit_die_no = std::nullopt;
        std::optional<int> dmg_die_no = std::nullopt;

        std::vector<size_t> enemy_actions;
        size_t enemy_actions_idx;
        std::optional<size_t> enemy_actions_dmg;

        bool* draw_required;
        UI::EnemyRoom* enemy_room;
        ttui::Text* tooltip_text;
        ttui::Handle* handle;

        EnemyRoom() = default;
        EnemyRoom(const EnemyRoom&) = default;

        void InitWidgets();

        void UpdateSize();

        void UpdateEnemyView();

        void UpdateSelectedItem();

        void Output(const std::string& text);

        void PollEvent(Game& game, ttui::Handle& handle);

        ttui::Rect GetEnemyViewRect() const;

        void UpdateFullscreenText();

        void UpdateEnemyActions();

        void UpdateTurnGauge();

        ttui::Paragraph GetEnemyPara(size_t enemy_idx) const;

        ttui::Paragraph GetPlayerPara() const;
    };
}