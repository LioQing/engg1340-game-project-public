#include <tui_data/ChestRoom.h>

#include <Game.h>
#include <Visitor.h>
#include <InChestRoom.h>

namespace tui_data
{
    void ChestRoom::InitWidgets()
    {
        auto& game = chest_room->game;
        auto& res = game.GetResources();

        // layout
        layout.SetDirection(ttui::Direction::Horizontal);
        layout.SetDefinitions(
            {
                ttui::Definition::Relative(0.6f),
                ttui::Definition::Absolute(0.4f),
            }
        );

        // loots view frame
        {
            ttui::Empty loots_view_frame;
            loots_view_frame.border = ttui::Border::Single("Level " + std::to_string(res.level.GetLevel()) + 
                " - Room (" + std::to_string(res.player.room_pos.x) + ", " + std::to_string(res.player.room_pos.y) + ")");

            layout.SetWidget(0, loots_view_frame);
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

        UpdateLootView();

        auto para = tooltip_text->GetParagraph();
        auto span = para.GetSpan(0, 0);
        span.str = "Left/Right/Down arrow to select loot; Enter to confirm selection; Esc to open pause menu; ";
        para.SetSpan(0, 0, span);
        tooltip_text->SetParagraph(para);

        Output("You found loots! You can only choose one.");
        *draw_required = true;
    }

    void ChestRoom::UpdateSize() 
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));

        // output
        auto& output = static_cast<ttui::Text&>(panel.GetWidget(panel.GetWidgetCount() - 1));
        auto height = output.GetWrappedParagraph(static_cast<uint16_t>(handle->GetWidth() * 0.4f)).GetHeight();
        panel.GetDefinition(panel.GetWidgetCount() - 1) = ttui::Definition::Absolute(height + 2);

        UpdateLootView();
        
        *draw_required = true;
    }

    void ChestRoom::UpdateLootView()
    {
        auto& game = chest_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Chest>(res.level.GetRoom(res.player.room_pos).room);
        auto& player = res.player;

        // definitions
        loots_layout.SetDirection(ttui::Direction::Horizontal);

        if (tmp_input)
        {
            loots_layout.SetDefinitions(std::vector<ttui::Definition>(player.weapons.size(), ttui::Definition::Fit()));

            for (size_t i = 0; i < player.weapons.size(); ++i)
            {
                ttui::Text text;
                text.SetBorder(ttui::Border::Single());
                text.SetHorizAlign(ttui::Align::Center);
                text.SetVertAlign(ttui::Align::Center);
                text.SetWrap(ttui::Wrap::Word);

                if (selected_loot == i)
                    text.SetBorder(ttui::Border::Double());

                ttui::Paragraph para;

                auto& weapon = player.weapons.at(i);
                para.AddSpan(0, 0, weapon.name);
                
                para.AddSpan(2, 0, weapon.attacks.at(0).name);
                para.AddSpan(3, 0, "DMG: " + std::to_string(weapon.attacks.at(0).base_dmg) + " + " + std::to_string(weapon.attacks.at(0).dmg_die));
                para.AddSpan(4, 0, "HIT: " + std::to_string(weapon.attacks.at(0).base_hit) + " + " + std::to_string(weapon.attacks.at(0).hit_die));
                
                para.AddSpan(6, 0, weapon.attacks.at(1).name);
                para.AddSpan(7, 0, "DMG: " + std::to_string(weapon.attacks.at(1).base_dmg) + " + " + std::to_string(weapon.attacks.at(1).dmg_die));
                para.AddSpan(8, 0, "HIT: " + std::to_string(weapon.attacks.at(1).base_hit) + " + " + std::to_string(weapon.attacks.at(1).hit_die));

                text.SetParagraph(para);

                loots_layout.SetWidget(i, text);
            }
        }
        else
        {
            loots_layout.SetDefinitions(std::vector<ttui::Definition>(3, ttui::Definition::Fit()));

            for (size_t i = 0; i < 3; ++i)
            {
                ttui::Text text;
                text.SetBorder(ttui::Border::Single());
                text.SetHorizAlign(ttui::Align::Center);
                text.SetVertAlign(ttui::Align::Center);
                text.SetWrap(ttui::Wrap::Word);

                if (selected_loot == i)
                    text.SetBorder(ttui::Border::Double());

                ttui::Paragraph para;

                std::visit(Visitor {
                    [&](const std::array<Weapon, 3>& weapons)
                    {
                        auto& weapon = weapons.at(i);
                        para.AddSpan(0, 0, weapon.name);
                        
                        para.AddSpan(2, 0, weapon.attacks.at(0).name);
                        para.AddSpan(3, 0, "DMG: " + std::to_string(weapon.attacks.at(0).base_dmg) + " + " + std::to_string(weapon.attacks.at(0).dmg_die));
                        para.AddSpan(4, 0, "HIT: " + std::to_string(weapon.attacks.at(0).base_hit) + " + " + std::to_string(weapon.attacks.at(0).hit_die));
                        
                        para.AddSpan(6, 0, weapon.attacks.at(1).name);
                        para.AddSpan(7, 0, "DMG: " + std::to_string(weapon.attacks.at(1).base_dmg) + " + " + std::to_string(weapon.attacks.at(1).dmg_die));
                        para.AddSpan(8, 0, "HIT: " + std::to_string(weapon.attacks.at(1).base_hit) + " + " + std::to_string(weapon.attacks.at(1).hit_die));
                    },
                    [&](const std::array<Armor, 3>& armors)
                    {
                        auto& armor = armors.at(i);
                        para.AddSpan(0, 0, armor.name);
                        para.AddSpan(1, 0, "DEF: " + std::to_string(armor.defence));
                        para.AddSpan(2, 0, "AGI: " + std::to_string(armor.agility));
                    },
                }, room.loots);

                text.SetParagraph(para);

                loots_layout.SetWidget(i, text);
            }

            if (selected_loot == 3)
                no_take_text.SetBorder(ttui::Border::Double());
            else
                no_take_text.SetBorder(ttui::Border::Single());

            no_take_text.SetHorizAlign(ttui::Align::Center);
            no_take_text.SetVertAlign(ttui::Align::Center);
            no_take_text.SetWrap(ttui::Wrap::Word);

            ttui::Paragraph para;
            para.AddSpan(0, 0, std::string("Do not take any loot"));

            no_take_text.SetParagraph(para);
        }
    }

    void ChestRoom::Output(const std::string& text)
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& output = static_cast<ttui::Text&>(panel.GetWidget(panel.GetWidgetCount() - 1));

        output.SetParagraph(ttui::Paragraph(text, ttui::Appearance()));
        UpdateSize();
        *draw_required = true;
    }

    void ChestRoom::PollEvent(Game& game, ttui::Handle& handle)
    {
        auto& res = game.GetResources();
        auto& player = res.player;

        ttui::Event event;

        input = std::nullopt;

        while (handle.PollEvent(event))
        {
            *draw_required = true;
            if (event.type == ttui::Event::Exit)
            {
                game.Exit();
                break;
            }
            else if (event.type == ttui::Event::Resize)
            {
                UpdateSize();
                *draw_required = true;
            }
            else if (event.type == ttui::Event::Key)
            {
                if (event.key.is_esc)
                {
                    if (event.key.code == ttui::KeyEvent::Left)
                    {
                        if (selected_loot > 0)
                        {
                            --selected_loot;
                            UpdateLootView();
                            *draw_required = true;
                        }
                    }
                    else if (event.key.code == ttui::KeyEvent::Right)
                    {
                        if (tmp_input)
                        {
                            if (selected_loot < player.weapons.size() - 1)
                            {
                                ++selected_loot;
                                UpdateLootView();
                                *draw_required = true;
                            }
                        }
                        else if (selected_loot < 2)
                        {
                            ++selected_loot;
                            UpdateLootView();
                            *draw_required = true;
                        }
                    }
                    else if (!tmp_input)
                    {
                        if (selected_loot < 3 && event.key.code == ttui::KeyEvent::Down)
                        {
                            selected_loot = 3;
                            UpdateLootView();
                            *draw_required = true;
                        }
                        else if (selected_loot == 3 && event.key.code == ttui::KeyEvent::Up)
                        {
                            selected_loot = 0;
                            UpdateLootView();
                            *draw_required = true;
                        }
                    }
                }
                else if (!tmp_input && event.key.code == ttui::KeyEvent::Escape)
                {
                    input = InChestRoom::Pause;
                }
                else if (event.key.code == '\n' || event.key.code == '\r')
                {
                    auto& game = chest_room->game;
                    auto& res = game.GetResources();
                    auto& room = std::get<Room::Type::Chest>(res.level.GetRoom(res.player.room_pos).room);

                    if (room.loots.index() == 0)
                    {
                        if (tmp_input)
                        {
                            input = tmp_input.value() + selected_loot * 4;
                        }
                        else if (selected_loot == 3)
                        {
                            input = InChestRoom::NoTake;
                        }
                        else
                        {
                            tmp_input = selected_loot;
                            selected_loot = 0;

                            auto para = tooltip_text->GetParagraph();
                            auto span = para.GetSpan(0, 0);
                            span.str = "Left/Right arrow to select weapon to swap out; Enter to confirm selection; ";
                            para.SetSpan(0, 0, span);
                            tooltip_text->SetParagraph(para);
                        }
                    }
                    else
                    {
                        if (selected_loot == 3)
                            input = InChestRoom::NoTake;
                        else
                            input = selected_loot;
                    }

                    UpdateLootView();
                    *draw_required = true;
                }
            }
        }
    }

    ttui::Rect ChestRoom::GetLootsViewRect() const
    {
        auto& game = chest_room->game;
        auto& res = game.GetResources();
        auto& player = res.player;

        auto width = tmp_input ? player.weapons.size() * 20 : 60;
        auto height = 12;

        auto a = ttui::Rect((handle->GetWidth() * 0.6f - width) / 2, (handle->GetHeight() - 4 - height) / 2 + 3, width, height);
        return a;
    }

    ttui::Rect ChestRoom::GetNoTakeTextRect() const
    {
        auto width = 22;
        auto height = 3;

        auto a = ttui::Rect((handle->GetWidth() * 0.6f - width) / 2, (handle->GetHeight() - 4 - height) / 2 + 11, width, height);
        return a;
    }

    ttui::Paragraph ChestRoom::GetPlayerPara() const
    {
        auto& game = chest_room->game;
        auto& res = game.GetResources();
        auto& player = res.player;

        ttui::Paragraph para;
        para.SetSpan(0, 0, "HP: " + std::to_string(player.hp) + " / " + std::to_string(player.max_hp));
        para.SetSpan(1, 0, "HPR: " + std::to_string(player.hp_regen));
        para.SetSpan(2, 0, "SPD: " + std::to_string(player.speed));
        para.SetSpan(4, 0, "Armor: " + player.armor.name);
        para.SetSpan(5, 2, "DEF: " + std::to_string(player.armor.defence));
        para.SetSpan(6, 2, "AGI: " + std::to_string(player.armor.agility));

        return para;
    }
}