#include <tui_data/EnemyRoom.h>

#include <Game.h>
#include <Combat.h>
#include <Random.h>

namespace tui_data
{
    void EnemyRoom::InitWidgets()
    {
        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        input = std::nullopt;

        // turn gauge
        turn_gauge.SetBorder(ttui::Border::Single("Turn Gauge"));
        
        // layout
        layout.SetDirection(ttui::Direction::Horizontal);
        layout.SetDefinitions(
            {
                ttui::Definition::Relative(0.6f),
                ttui::Definition::Absolute(0.4f),
            }
        );

        // enemy view frame
        {
            ttui::Empty enemy_view_frame;
            enemy_view_frame.border = ttui::Border::Single("Level " + std::to_string(res.level.GetLevel()) + 
                " - Room (" + std::to_string(res.player.room_pos.x) + ", " + std::to_string(res.player.room_pos.y) + ")");

            layout.SetWidget(0, enemy_view_frame);
        }

        // panel
        {
            ttui::Layout panel;
            panel.SetDirection(ttui::Direction::Vertical);

            panel.SetDefinitions(
                {
                    ttui::Definition::Absolute(0),
                    ttui::Definition::Fit(),
                    ttui::Definition::Absolute(0),
                }
            );

            // stats
            ttui::Text stat;
            stat.SetBorder(ttui::Border::Single("Player Stats"));
            stat.SetHorizAlign(ttui::Align::Left);
            stat.SetVertAlign(ttui::Align::Top);
            stat.SetWrap(ttui::Wrap::Word);

            stat.SetParagraph(GetPlayerPara());

            panel.SetWidget(0, stat);

            // actions
            ttui::List action_list;
            action_list.SetBorder(ttui::Border::Single("Actions"));
            action_list.SetHorizAlign(ttui::Align::Left);
            action_list.SetWrap(ttui::Wrap::Word);
            action_list.SetEnableAutoAppearance(true);

            {
                std::vector<ttui::Paragraph> para_list;

                if (res.player.weapons.size() > 0)
                {
                    auto& weapon = res.player.weapons.at(0);
                    auto& attacks = weapon.attacks;

                    para_list.emplace_back(ttui::Paragraph(weapon.name + " - " + attacks.at(0).name + ":\nDMG: " + 
                        std::to_string(attacks.at(0).base_dmg) + " + " + std::to_string(attacks.at(0).dmg_die) +
                        ", HIT: " + std::to_string(attacks.at(0).base_hit) + " + " + std::to_string(attacks.at(0).hit_die),
                        ttui::Appearance()));

                    para_list.emplace_back(ttui::Paragraph(weapon.name + " - " + attacks.at(1).name + ":\nDMG: " + 
                        std::to_string(attacks.at(1).base_dmg) + " + " + std::to_string(attacks.at(1).dmg_die) +
                        ", HIT: " + std::to_string(attacks.at(1).base_hit) + " + " + std::to_string(attacks.at(1).hit_die),
                        ttui::Appearance()));
                }

                if (res.player.weapons.size() > 1)
                {
                    auto& weapon = res.player.weapons.at(1);
                    auto& attacks = weapon.attacks;

                    para_list.emplace_back(ttui::Paragraph(weapon.name + " - " + attacks.at(0).name + ":\nDMG: " + 
                        std::to_string(attacks.at(0).base_dmg) + " + " + std::to_string(attacks.at(0).dmg_die) +
                        ", HIT: " + std::to_string(attacks.at(0).base_hit) + " + " + std::to_string(attacks.at(0).hit_die),
                        ttui::Appearance()));

                    para_list.emplace_back(ttui::Paragraph(weapon.name + " - " + attacks.at(1).name + ":\nDMG: " + 
                        std::to_string(attacks.at(1).base_dmg) + " + " + std::to_string(attacks.at(1).dmg_die) +
                        ", HIT: " + std::to_string(attacks.at(1).base_hit) + " + " + std::to_string(attacks.at(1).hit_die),
                        ttui::Appearance()));
                }

                action_list.SetItems(para_list);
            }

            panel.SetWidget(1, action_list);

            // output
            ttui::Text output;
            output.SetBorder(ttui::Border::Single());
            output.SetHorizAlign(ttui::Align::Left);
            output.SetVertAlign(ttui::Align::Top);
            output.SetWrap(ttui::Wrap::Word);
            output.SetParagraph(ttui::Paragraph("", ttui::Appearance()));

            panel.SetWidget(2, output);

            layout.SetWidget(1, std::move(panel));
        }

        // dice text
        {
            fullscreen_text.SetBorder(ttui::Border::Single());
        }

        UpdateTurnGauge();

        Output("You encountered enemy!");
        selected_item = 0;
        stage = Stage::Attack;
        UpdateSelectedItem();

        *draw_required = true;
    }

    void EnemyRoom::UpdateSize()
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));

        // output
        auto& output = static_cast<ttui::Text&>(panel.GetWidget(panel.GetWidgetCount() - 1));
        auto height = output.GetWrappedParagraph(static_cast<uint16_t>(handle->GetWidth() * 0.4f)).GetHeight();
        panel.GetDefinition(panel.GetWidgetCount() - 1) = ttui::Definition::Absolute(height + 2);

        // player stats
        auto& stats = static_cast<ttui::Text&>(panel.GetWidget(0));
        height = stats.GetWrappedParagraph(static_cast<uint16_t>(handle->GetWidth() * 0.4f)).GetHeight();
        panel.GetDefinition(0) = ttui::Definition::Absolute(height + 2);

        UpdateEnemyView();

        *draw_required = true;
    }

    void EnemyRoom::UpdateEnemyView()
    {
        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);

        // definitions
        enemies_layout.SetDirection(ttui::Direction::Horizontal);
        enemies_layout.SetDefinitions(std::vector<ttui::Definition>(room.enemies.size(), ttui::Definition::Fit()));

        for (size_t i = 0; i < room.enemies.size(); ++i)
        {
            ttui::Text text;
            text.SetBorder(ttui::Border::Single());
            text.SetHorizAlign(ttui::Align::Center);
            text.SetVertAlign(ttui::Align::Center);
            text.SetWrap(ttui::Wrap::Word);

            text.SetParagraph(GetEnemyPara(i));

            enemies_layout.SetWidget(i, text);
        }
    }

    void EnemyRoom::UpdateSelectedItem()
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& list = static_cast<ttui::List&>(panel.GetWidget(1));

        auto para = tooltip_text->GetParagraph();
        auto span = para.GetSpan(0, 0);

        if (stage == Stage::Attack)
        {
            span.str = "Up/Down arrow to select action; Enter to confirm selection; Esc to open pause menu; ";

            list.SetEnableAutoAppearance(true);
            list.SetSelectedItemIndex(selected_item);

            for (size_t i = 0; i < enemies_layout.GetWidgetCount(); ++i)
            {
                auto& text = static_cast<ttui::Text&>(enemies_layout.GetWidget(i));
                text.SetBorder(ttui::Border::Single());
            }
        }
        else if (stage == Stage::Target)
        {
            span.str = "Left/Right arrow to select enemy; Enter to confirm selection; Esc to return to action selection; ";

            for (size_t i = 0; i < enemies_layout.GetWidgetCount(); ++i)
            {
                auto& text = static_cast<ttui::Text&>(enemies_layout.GetWidget(i));
                text.SetBorder(i == selected_item ? ttui::Border::Double() : ttui::Border::Single());
            }
        }
        else if (stage == Stage::HitDie)
        {
            span.str = "Esc to return to enemy selection; ";
        }
        else
        {
            span.str = "";
        }

        para.SetSpan(0, 0, span);
        tooltip_text->SetParagraph(para);
        *draw_required = true;
    }

    void EnemyRoom::Output(const std::string& text)
    {
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& output = static_cast<ttui::Text&>(panel.GetWidget(panel.GetWidgetCount() - 1));

        output.SetParagraph(ttui::Paragraph(text, ttui::Appearance()));
        UpdateSize();
        *draw_required = true;
    }

    void EnemyRoom::PollEvent(Game& game, ttui::Handle& handle)
    {
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);
        auto& enemy_idx = selected_item;
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& list = static_cast<ttui::List&>(panel.GetWidget(1));

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
                if (stage == Stage::HitDie)
                {
                    auto& attack = player.weapons.at(list.GetSelectedItemIndex() / 2).attacks.at(list.GetSelectedItemIndex() % 2);
                    if (!event.key.is_esc)
                    {
                        if (event.key.code == ttui::KeyEvent::Escape)
                        {
                            Output("Returned to enemy selection.");
                            stage = Stage::Target;
                            UpdateSelectedItem();
                            UpdateFullscreenText();
                            *draw_required = true;
                        }
                        else
                        {
                            auto& enemy = room.enemies.at(enemy_idx);
                            stage = Stage::DamageDie;

                            hit_die_no = Random().GetInt(0, attack.hit_die);

                            if (hit_die_no.value() + attack.base_hit <= enemy.armor.agility)
                            {
                                stage = Stage::PostAttack;
                            }
                            
                            UpdateSelectedItem();
                            UpdateFullscreenText();
                            *draw_required = true;
                        }
                    }
                }
                else if (stage == Stage::DamageDie)
                {
                    auto& attack = player.weapons.at(list.GetSelectedItemIndex() / 2).attacks.at(list.GetSelectedItemIndex() % 2);

                    if (!event.key.is_esc)
                    {
                        auto& enemy = room.enemies.at(enemy_idx);
                        stage = Stage::PostAttack;

                        dmg_die_no = Random().GetInt(0, attack.dmg_die);

                        int dmg = attack.base_dmg + dmg_die_no.value() - enemy.armor.defence;
                        if (dmg < 0) dmg = 0;

                        enemy.hp -= dmg;

                        static_cast<ttui::Text&>(enemies_layout.GetWidget(enemy_idx)).SetParagraph(GetEnemyPara(enemy_idx));

                        UpdateSelectedItem();
                        UpdateFullscreenText();
                        *draw_required = true;

                        if (enemy.hp <= 0)
                        {
                            room.enemies.erase(room.enemies.begin() + enemy_idx);
                            if (!room.enemies.empty())
                            {
                                UpdateEnemyView();
                            }
                        }
                    }
                }
                else if (stage == Stage::PostAttack)
                {
                    if (!event.key.is_esc)
                    {
                        if (room.enemies.empty())
                        {
                            input = Room::EnemyRoomInfo::Victory;
                            return;
                        }

                        stage = Stage::EnemyActions;

                        hit_die_no = std::nullopt;
                        dmg_die_no = std::nullopt;

                        auto tmp_room = room;
                        enemy_actions = Combat::TakeTurn(tmp_room, player);

                        if (enemy_actions.empty())
                        {
                            stage = Stage::Attack;
                            selected_item = 0;
                            Combat::TakeTurn(room, player);
                            UpdateSelectedItem();
                        }
                        else
                        {
                            enemy_actions_idx = 0;
                            UpdateSelectedItem();
                            UpdateEnemyActions();
                        }

                        UpdateTurnGauge();
                        UpdateFullscreenText();
                        *draw_required = true;
                    }
                }
                else if (stage == Stage::EnemyActions)
                {
                    if (!event.key.is_esc)
                    {
                        ++enemy_actions_idx;
                        if (player.hp <= 0)
                        {
                            input = Room::EnemyRoomInfo::Defeat;
                        }
                        else if (enemy_actions_idx == enemy_actions.size())
                        {
                            stage = Stage::Attack;
                            selected_item = 0;
                            Combat::TakeTurn(room, player);
                            UpdateSelectedItem();
                        }
                        else
                        {
                            UpdateSelectedItem();
                            UpdateEnemyActions();
                            UpdateFullscreenText();
                        }

                        UpdateTurnGauge();
                        *draw_required = true;
                    }
                }
                else if (event.key.is_esc)
                {
                    if (stage == Stage::Attack)
                    {
                        bool selected_changed = false;

                        if (selected_item > 0 && event.key.code == ttui::KeyEvent::Up)
                        {
                            --selected_item;
                            selected_changed = true;
                        }
                        else if (selected_item < res.player.weapons.size() * 2 - 1 && event.key.code == ttui::KeyEvent::Down)
                        {
                            ++selected_item;
                            selected_changed = true;
                        }
                        
                        if (selected_changed)
                        {
                            Output("Action " + std::to_string(selected_item + 1) + " selected.");
                            UpdateSelectedItem();
                            *draw_required = true;
                        }
                    }
                    else if (stage == Stage::Target)
                    {
                        bool selected_changed = false;

                        if (selected_item > 0 && event.key.code == ttui::KeyEvent::Left)
                        {
                            --selected_item;
                            selected_changed = true;
                        }
                        else if (selected_item < room.enemies.size() - 1 && event.key.code == ttui::KeyEvent::Right)
                        {
                            ++selected_item;
                            selected_changed = true;
                        }

                        if (selected_changed)
                        {
                            Output("Enemy " + room.enemies.at(selected_item).name + " selected.");
                            UpdateSelectedItem();
                            *draw_required = true;
                        }
                    }
                }
                else if (event.key.code == ttui::KeyEvent::Escape)
                {
                    if (stage == Stage::Attack)
                    {
                        input = Room::EnemyRoomInfo::ReturnType::Pause;
                    }
                    else if (stage == Stage::Target)
                    {
                        stage = Stage::Attack;
                        selected_item = 0;
                        Output("Returned to action selection.");
                        UpdateSelectedItem();
                        *draw_required = true;
                    }
                }
                else if (event.key.code == '\n' || event.key.code == '\r')
                {
                    if (stage == Stage::Attack)
                    {
                        stage = Stage::Target;
                        selected_item = 0;
                        UpdateSelectedItem();
                        *draw_required = true;
                    }
                    else if (stage == Stage::Target)
                    {
                        stage = Stage::HitDie;
                        UpdateSelectedItem();
                        UpdateFullscreenText();
                        *draw_required = true;
                    }
                }
            }
        }
    }

    ttui::Rect EnemyRoom::GetEnemyViewRect() const
    {
        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);

        auto width = room.enemies.size() * 20;
        auto height = 12;

        auto a = ttui::Rect((handle->GetWidth() * 0.6f - width) / 2, (handle->GetHeight() - 4 - height) / 2 + 3, width, height);
        return a;
    }

    void EnemyRoom::UpdateFullscreenText()
    {
        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& list = static_cast<ttui::List&>(panel.GetWidget(1));

        auto& player = res.player;

        auto& weapon = res.player.weapons.at(1);
        auto& attack = player.weapons.at(list.GetSelectedItemIndex() / 2).attacks.at(list.GetSelectedItemIndex() % 2);

        fullscreen_text.SetBorder(ttui::Border::Single());
        fullscreen_text.SetHorizAlign(ttui::Align::Center);
        fullscreen_text.SetVertAlign(ttui::Align::Center);
        fullscreen_text.SetWrap(ttui::Wrap::Word);

        // enemy actions
        if (stage == Stage::EnemyActions)
        {
            auto& enemy = room.enemies.at(enemy_actions.at(enemy_actions_idx));

            ttui::Paragraph para;
            para.AddSpan(0, 0, enemy.name + " attacked you!");

            if (!enemy_actions_dmg)
            {
                para.AddSpan(1, 0, std::string("The hit missed!"));
            }
            else
            {
                para.AddSpan(1, 0, "Damage received: " + std::to_string(enemy_actions_dmg.value()));
            }

            para.SetSpan(3, 0, std::string("You"));
            para.SetSpan(4, 0, "HP: " + std::to_string(res.player.hp) + " / " + std::to_string(res.player.max_hp));
            para.SetSpan(5, 0, "HPR: " + std::to_string(res.player.hp_regen));
            para.SetSpan(6, 0, "SPD: " + std::to_string(res.player.speed));
            para.SetSpan(7, 0, "DEF: " + std::to_string(res.player.armor.defence));
            para.SetSpan(8, 0, "AGI: " + std::to_string(res.player.armor.agility));

            para.AddSpan(10, 0, std::string("--Press any key to continue--"));

            fullscreen_text.SetParagraph(para);

            return;
        }

        // enemy stats
        auto& enemy_idx = selected_item;
        auto& enemy = room.enemies.at(enemy_idx);
        ttui::Paragraph para = GetEnemyPara(enemy_idx);

        para.AddSpan(7, 0, std::string("Attacking with: "));
        para.AddSpan(8, 0, weapon.name + " - " + attack.name + ":\nDMG: " + 
            std::to_string(attack.base_dmg) + " + " + std::to_string(attack.dmg_die) +
            ", HIT: " + std::to_string(attack.base_hit) + " + " + std::to_string(attack.hit_die));
        
        if (!hit_die_no)
        {
            para.AddSpan(10, 0, std::string("--Press any key to roll hit die--"));
            para.AddSpan(17, 0, std::string(" "));
        }
        else if (stage == Stage::PostAttack && !dmg_die_no)
        {
            para.AddSpan(10, 0, std::string("Hit die number: ") + std::to_string(hit_die_no.value()));
            para.AddSpan(11, 0, "Hit number: " + std::to_string(attack.base_hit + hit_die_no.value()));

            para.AddSpan(12, 0, std::string("The hit missed!"));
            para.AddSpan(14, 0, std::string("--Press any key to continue--"));
            para.AddSpan(17, 0, std::string(" "));
        }
        else
        {
            para.AddSpan(10, 0, std::string("Hit die number: ") + std::to_string(hit_die_no.value()));
            para.AddSpan(11, 0, "Hit number: " + std::to_string(attack.base_hit + hit_die_no.value()));

            if (!dmg_die_no)
            {
                para.AddSpan(13, 0, std::string("--Press any key to roll damage die--"));
                para.AddSpan(17, 0, std::string(" "));
            }
            else
            {
                para.AddSpan(13, 0, std::string("Damage die number: ") + std::to_string(dmg_die_no.value()));
                para.AddSpan(14, 0, "Damage number: " + std::to_string(attack.base_dmg + dmg_die_no.value()));

                int dmg = attack.base_dmg + dmg_die_no.value() - enemy.armor.defence;
                if (dmg < 0) dmg = 0;

                para.AddSpan(15, 0, "Total damage dealt: " + std::to_string(dmg));
                para.AddSpan(17, 0, std::string("--Press any key to continue--"));
            }
        }

        fullscreen_text.SetParagraph(para);
    }

    void EnemyRoom::UpdateEnemyActions()
    {
        enemy_actions_dmg = std::nullopt;

        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);
        auto& enemy = room.enemies.at(enemy_actions.at(enemy_actions_idx));
        auto& player = res.player;

        auto hit = enemy.attack.base_hit + Random().GetInt(0, enemy.attack.hit_die);

        if (hit <= player.armor.agility)
            return;
        
        int enemy_dmg = enemy.attack.base_dmg + Random().GetInt(0, enemy.attack.dmg_die) - player.armor.defence;
        if (enemy_dmg < 0) enemy_dmg = 0;
        enemy_actions_dmg = enemy_dmg;

        player.hp -= enemy_actions_dmg.value();
        
        auto& panel = static_cast<ttui::Layout&>(layout.GetWidget(1));
        auto& stats = static_cast<ttui::Text&>(panel.GetWidget(0));
        stats.SetParagraph(GetPlayerPara());

        *draw_required = true;
    }

    void EnemyRoom::UpdateTurnGauge()
    {
        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);

        int turn_gauge_size = 5;
        if (stage == Stage::EnemyActions)
        {
            turn_gauge_size += enemy_actions_idx + 1;
        }

        std::string gauge_str;
        auto gauge_vec = Combat::TurnGauge(room, res.player, turn_gauge_size);
        for (size_t i = 0; i < gauge_vec.size(); i++)
        {
            if (stage == Stage::EnemyActions && i < 1 + enemy_actions_idx)
                continue;

            gauge_str += gauge_vec.at(i) + " -> ";
        }
        gauge_str += "...";

        turn_gauge.SetParagraph(ttui::Paragraph(gauge_str, ttui::Appearance()));

        *draw_required = true;
    }

    ttui::Paragraph EnemyRoom::GetEnemyPara(size_t enemy_idx) const
    {
        auto& game = enemy_room->game;
        auto& res = game.GetResources();
        auto& room = std::get<Room::Type::Enemy>(res.level.GetRoom(res.player.room_pos).room);
        auto& enemy = room.enemies.at(enemy_idx);

        ttui::Paragraph para;
        para.AddSpan(0, 0, enemy.name);
        para.AddSpan(2, 0, "HP: " + std::to_string(enemy.hp) + " / " + std::to_string(enemy.max_hp));
        para.AddSpan(3, 0, "SPD: " + std::to_string(enemy.speed));
        para.AddSpan(4, 0, "DEF: " + std::to_string(enemy.armor.defence));
        para.AddSpan(5, 0, "AGI: " + std::to_string(enemy.armor.agility));

        return para;
    }

    ttui::Paragraph EnemyRoom::GetPlayerPara() const
    {
        auto& game = enemy_room->game;
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