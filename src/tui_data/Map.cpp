#include <tui_data/Map.h>

#include <Game.h>

namespace tui_data
{
    const std::string Map::tooltip = "Up/Down/Left/Right arrow to select; Enter to confirm selection; Esc to open pause menu; ";

    void Map::InitWidgets()
    {
        auto& game = map->game;
        auto& res = game.GetResources();

        // tooltip
        auto para = tooltip_text->GetParagraph();
        auto span = para.GetSpan(0, 0);
        span.str = tooltip;
        para.SetSpan(0, 0, span);
        tooltip_text->SetParagraph(para);
        
        // layout
        input = std::nullopt;

        layout.SetDirection(ttui::Direction::Horizontal);
        layout.SetDefinitions(
            {
                ttui::Definition::Relative(0.6f),
                ttui::Definition::Absolute(0.4f),
            }
        );

        // map
        {
            ttui::Text map;
            map.SetBorder(ttui::Border::Single("Level " + std::to_string(res.level.GetLevel())));
            map.SetHorizAlign(ttui::Align::Left);
            map.SetVertAlign(ttui::Align::Center);
            map.SetWrap(ttui::Wrap::None);
            map.SetParagraph(GenerateMapStrPara());

            layout.SetWidget(0, map);
        }

        // panel
        {
            ttui::Layout panel;
            panel.SetDirection(ttui::Direction::Vertical);
            panel.SetDefinitions(
                {
                    ttui::Definition::Fit(),
                    ttui::Definition::Absolute(0),
                }
            );

            // stats
            ttui::Text stat;
            stat.SetBorder(ttui::Border::Single("Player Stat"));
            stat.SetHorizAlign(ttui::Align::Left);
            stat.SetVertAlign(ttui::Align::Top);
            stat.SetWrap(ttui::Wrap::Word);

            ttui::Paragraph para;
            para.SetSpan(0, 0, "HP: " + std::to_string(res.player.hp) + " / " + std::to_string(res.player.max_hp));
            para.SetSpan(1, 0, "HPR: " + std::to_string(res.player.hp_regen));
            para.SetSpan(2, 0, "SPD: " + std::to_string(res.player.speed));
            para.SetSpan(4, 0, "Armor: " + res.player.armor.name);
            para.SetSpan(5, 2, "DEF: " + std::to_string(res.player.armor.defence));
            para.SetSpan(6, 2, "AGI: " + std::to_string(res.player.armor.agility));

            if (res.player.weapons.size() > 0)
            {
                auto& weapon = res.player.weapons.at(0);
                para.SetSpan(8, 0, "Weapon 1: " + weapon.name);

                auto& attacks = weapon.attacks;

                para.SetSpan(9, 2, attacks.at(0).name + ": DMG: " + 
                    std::to_string(attacks.at(0).base_dmg) + " + " + std::to_string(attacks.at(0).dmg_die) +
                    ", HIT: " + std::to_string(attacks.at(0).base_hit) + " + " + std::to_string(attacks.at(0).hit_die));

                para.SetSpan(10, 2, attacks.at(1).name + ": DMG: " + 
                    std::to_string(attacks.at(1).base_dmg) + " + " + std::to_string(attacks.at(1).dmg_die) +
                    ", HIT: " + std::to_string(attacks.at(1).base_hit) + " + " + std::to_string(attacks.at(1).hit_die));
            }

            if (res.player.weapons.size() > 1)
            {
                auto& weapon = res.player.weapons.at(1);
                para.SetSpan(12, 0, "Weapon 2: " + weapon.name);

                auto& attacks = weapon.attacks;

                para.SetSpan(13, 2, attacks.at(0).name + ": DMG: " + 
                    std::to_string(attacks.at(0).base_dmg) + " + " + std::to_string(attacks.at(0).dmg_die) +
                    ", HIT: " + std::to_string(attacks.at(0).base_hit) + " + " + std::to_string(attacks.at(0).hit_die));

                para.SetSpan(14, 2, attacks.at(1).name + ": DMG: " + 
                    std::to_string(attacks.at(1).base_dmg) + " + " + std::to_string(attacks.at(1).dmg_die) +
                    ", HIT: " + std::to_string(attacks.at(1).base_hit) + " + " + std::to_string(attacks.at(1).hit_die));
            }

            stat.SetParagraph(para);

            panel.SetWidget(0, stat);

            // output
            ttui::Text output;
            output.SetBorder(ttui::Border::Single());
            output.SetHorizAlign(ttui::Align::Left);
            output.SetVertAlign(ttui::Align::Top);
            output.SetWrap(ttui::Wrap::Word);
            output.SetParagraph(ttui::Paragraph("", ttui::Appearance()));

            panel.SetWidget(1, output);

            layout.SetWidget(1, std::move(panel));
        }

        *draw_required = true;
    }

    void Map::UpdateSize()
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& output = static_cast<ttui::Text&>(panel.GetWidget(panel.GetWidgetCount() - 1));

        auto height = output.GetWrappedParagraph(static_cast<uint16_t>(handle->GetWidth() * 0.4f)).GetHeight();
        panel.GetDefinition(panel.GetWidgetCount() - 1) = ttui::Definition::Absolute(height + 2);

        *draw_required = true;
    }

    void Map::Output(const std::string& text)
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& output = static_cast<ttui::Text&>(panel.GetWidget(panel.GetWidgetCount() - 1));

        output.SetParagraph(ttui::Paragraph(text, ttui::Appearance()));
        UpdateSize();
        *draw_required = true;
    }

    void Map::PollEvent(Game& game, ttui::Handle& handle)
    {
        auto& res = game.GetResources();
        ttui::Event event;

        input = std::nullopt;

        while (handle.PollEvent(event))
        {
            if (event.type == ttui::Event::Exit)
            {
                game.Exit();
                break;
            }
            else if (event.type == ttui::Event::Resize)
            {
                UpdateSize();
                static_cast<ttui::Text&>(layout.GetWidget(0)).SetParagraph(GenerateMapStrPara());
                *draw_required = true;
            }
            else if (event.type == ttui::Event::Key)
            {
                if (event.key.is_esc)
                {
                    bool dir_selected_changed = false;
                    auto has_neighbours = res.level.HasNeighbours(res.player.room_pos);

                    if (has_neighbours.test(0) && event.key.code == ttui::KeyEvent::Up)
                    {
                        dir_selected = Room::North;
                        dir_selected_changed = true;
                    }
                    else if (has_neighbours.test(1) && event.key.code == ttui::KeyEvent::Down)
                    {
                        dir_selected = Room::South;
                        dir_selected_changed = true;
                    }
                    else if (has_neighbours.test(2) && event.key.code == ttui::KeyEvent::Right)
                    {
                        dir_selected = Room::East;
                        dir_selected_changed = true;
                    }
                    else if (has_neighbours.test(3) && event.key.code == ttui::KeyEvent::Left)
                    {
                        dir_selected = Room::West;
                        dir_selected_changed = true;
                    }

                    if (dir_selected_changed)
                    {
                        Output(Room::DirectionToString(dir_selected) + " room selected.");
                        static_cast<ttui::Text&>(layout.GetWidget(0)).SetParagraph(GenerateMapStrPara());
                        *draw_required = true;
                    }
                }
                else if (event.key.code == ttui::KeyEvent::Escape)
                {
                    input = Room::Pause;
                }
                else if (
                    static_cast<uint8_t>(dir_selected) < Room::Pause &&
                    (event.key.code == '\n' || event.key.code == '\r')
                    )
                {
                    input = dir_selected;
                }
            }
        }
    }

    ttui::Paragraph Map::GenerateMapStrPara()
    {
        auto& res = map->game.GetResources();
        auto map_str = res.level.GetMapString(res.player.room_pos, dir_selected);

        size_t handle_width = handle->GetWidth() * 0.6f;
        size_t map_str_actual_width = map_str.width * Level::room_str_width - 1;
        size_t x_align_offset = handle_width > map_str_actual_width ? (handle_width - map_str_actual_width) / 2 : 0;
        for (size_t i = 0; i != std::string::npos; i = map_str.str.find('\n', i + 1))
        {
            if (i == 0)
            {
                map_str.str.insert(i, std::string(x_align_offset, ' '));
                continue;
            }

            map_str.str.insert(i + 1, std::string(x_align_offset, ' '));
        }

        return ttui::Paragraph(map_str.str, ttui::Appearance());
    }
}