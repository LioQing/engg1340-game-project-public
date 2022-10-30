#include <tui_data/Menu.h>

#include <numeric>
#include <Game.h>

namespace tui_data
{
    namespace
    {
        void UpdateSelectionWidget(const widget::Selection& selection, ttui::Text& text);
    }

    const std::string Menu::tooltip = "Up/Down arrow to select; ";

    const std::string Menu::menu_tooltip = "Enter to confirm selection; ";
    const std::string Menu::selection_tooltip = "Left/Right arrow to change option; Enter to cycle through options; ";
    const std::string Menu::list_tooltip = "Enter to confirm selection; ";
    const std::string Menu::point_allocation_tooltip = "Left/Right arrow to decrease/increase pt allocated; ";
    const std::string Menu::text_box_tooltip = "Type to input value; ";
    const std::string Menu::text_box_enter_tooltip = "Type to input value; Enter to confirm; ";

    const ttui::Appearance Menu::curr_appear = ttui::Appearance(ttui::Color::White(), ttui::Color::BrightBlack());
    const ttui::Appearance Menu::list_selected_appear = ttui::Appearance(ttui::Color::Black(), ttui::Color::White());

    void Menu::InitWidgets()
    {
        auto& menu = *this->menu;

        input = std::nullopt;

        grid.SetDefinitions(
            // column
            {
                ttui::Definition::Fit(),
                ttui::Definition::Absolute(column_width),
                ttui::Definition::Fit(),
            },
            // row
            {
                ttui::Definition::Fit(),
                ttui::Definition::Relative(1.f),
                ttui::Definition::Fit(),
            }
        );

        // menu
        {
            // layout
            ttui::Layout layout;
            layout.SetDirection(ttui::Direction::Vertical);

            // definitions
            layout.SetDefinitions(std::vector<ttui::Definition>(menu.widgets.size() + 4, ttui::Definition::Fit()));

            // menu list
            ttui::List list;
            list.SetBorder(ttui::Border::Single(menu.title));
            list.SetHorizAlign(ttui::Align::Center);
            list.SetWrap(ttui::Wrap::Word);
            list.SetEnableAutoAppearance(true);
            list.SetAutoAppearance(ttui::Appearance(), curr_appear);

            for (const auto& option : menu.options)
            {
                list.PushBack(ttui::Paragraph(option, ttui::Appearance()));
            }
            list.SetSelectedItemIndex(0);

            // text widget
            ttui::Text text;
            text.SetBorder(ttui::Border::Single());
            text.SetHorizAlign(ttui::Align::Center);
            text.SetWrap(ttui::Wrap::Word);
            text.SetParagraph(ttui::Paragraph("", ttui::Appearance()));

            // widgets
            for (size_t i = 0; i < menu.widgets.size(); ++i)
            {
                const auto& w = *menu.widgets.at(i);

                if (w.GetType() == widget::Type::Selection)
                {
                    const auto& selection = static_cast<const widget::Selection&>(w);

                    auto text = ttui::Text(
                        ttui::Paragraph(selection.options.at(selection.curr_option), ttui::Appearance()),
                        ttui::Border::Single(selection.title),
                        ttui::Wrap::Word
                    );

                    text.SetHorizAlign(ttui::Align::Center);

                    layout.GetDefinition(i + 2) = ttui::Definition::Absolute(text.GetWrappedParagraph(column_width).GetHeight() + 2);
                    layout.SetWidget(i + 2, text);
                }
                else if (w.GetType() == widget::Type::List)
                {
                    const auto& list = static_cast<const widget::List&>(w);

                    std::vector<ttui::Paragraph> options(list.options.size());
                    for (size_t i = 0; i < list.options.size(); ++i)
                    {
                        options.at(i) = ttui::Paragraph(list.options.at(i), ttui::Appearance());
                    }

                    auto ui_list = ttui::List(options, ttui::Border::Single(list.title), ttui::Wrap::Word);
                    ui_list.SetHorizAlign(ttui::Align::Center);
                    ui_list.SetEnableAutoAppearance(false);
                    ui_list.SetFillSpanBackgroundColor(true);

                    size_t height = std::accumulate(ui_list.GetItems().begin(), ui_list.GetItems().end(), 0,
                    [](size_t height, const ttui::Paragraph& item){ return height + item.GetHeight(); });

                    layout.GetDefinition(i + 2) = ttui::Definition::Absolute(height + 2);
                    layout.SetWidget(i + 2, ui_list);

                    // current item
                    list_widget_curr_item.emplace(i, 0);
                }
                else if (w.GetType() == widget::Type::PointAllocation)
                {
                    const auto& pt_alloc = static_cast<const widget::PointAllocation&>(w);

                    std::vector<ttui::Paragraph> options(pt_alloc.options.size());
                    for (size_t i = 0; i < pt_alloc.options.size(); ++i)
                    {
                        options.at(i) = ttui::Paragraph(pt_alloc.GetOptionStr(i), ttui::Appearance());
                    }

                    auto ui_list = ttui::List(options, ttui::Border::Single(
                        pt_alloc.title + " (Remaining pts: " + std::to_string(pt_alloc.remaining_pts) + ")"), ttui::Wrap::Word
                    );
                    ui_list.SetHorizAlign(ttui::Align::Center);
                    ui_list.SetEnableAutoAppearance(false);
                    ui_list.SetFillSpanBackgroundColor(true);

                    const auto& wrapped_items = ui_list.GetWrappedItems(column_width);
                    size_t height = std::accumulate(wrapped_items.begin(), wrapped_items.end(), 0,
                    [](size_t height, const ttui::Paragraph& item){ return height + item.GetHeight(); });

                    layout.GetDefinition(i + 2) = ttui::Definition::Absolute(height + 2);
                    layout.SetWidget(i + 2, ui_list);

                    // current item
                    pt_alloc_widget_curr_item.emplace(i, 0);
                }
                else if (w.GetType() == widget::Type::TextBox)
                {
                    const auto& text = static_cast<const widget::TextBox&>(w);

                    auto ui_text = ttui::Text(
                        ttui::Paragraph(text.text, ttui::Appearance()),
                        ttui::Border::Single(text.title)
                    );
                    ui_text.SetHorizAlign(ttui::Align::Center);

                    layout.GetDefinition(i + 2) = ttui::Definition::Absolute(ui_text.GetParagraph().GetHeight() + 2);
                    layout.SetWidget(i + 2, ui_text);

                    // current char
                    text_box_widget_curr_idx.emplace(i, text.text.size());
                }
            }

            // height of menu list
            size_t height = std::accumulate(list.GetItems().begin(), list.GetItems().end(), 0,
            [](size_t height, const ttui::Paragraph& item){ return height + item.GetHeight(); });

            layout.GetDefinition(1) = ttui::Definition::Absolute(height + 2);
            layout.SetWidget(1, list);

            // text widget
            layout.GetDefinition(menu.widgets.size() + 2) = ttui::Definition::Absolute(0);
            layout.SetWidget(menu.widgets.size() + 2, text);

            grid.SetWidget(1, 1, std::move(layout));
            curr_widget = menu.widgets.size();
        }

        UpdateWidgets();
        *draw_required = true;
    }

    void Menu::UpdateWidgets()
    {
        auto& menu = *this->menu;
        auto& layout = static_cast<ttui::Layout&>(grid.GetWidget(1, 1));

        // menu list
        auto& list = static_cast<ttui::List&>(layout.GetWidget(1));
        list.SetEnableAutoAppearance(curr_widget == menu.widgets.size());

        // tooltip
        auto para = tooltip_text->GetParagraph();
        auto span = para.GetSpan(0, 0);
        span.str = tooltip;

        if (curr_widget != menu.widgets.size())
        {
            auto& curr_w = *menu.widgets.at(curr_widget);
            if (curr_w.GetType() == widget::Type::Selection)
            {
                span.str += selection_tooltip;
            }
            else if (curr_w.GetType() == widget::Type::List)
            {
                span.str += list_tooltip;
            }
            else if (curr_w.GetType() == widget::Type::PointAllocation)
            {
                span.str += point_allocation_tooltip;
            }
            else if (curr_w.GetType() == widget::Type::TextBox)
            {
                if (static_cast<const widget::TextBox&>(curr_w).is_minigame)
                {
                    span.str += text_box_enter_tooltip;
                }
                else
                {
                    span.str += text_box_tooltip;
                }
            }
        }
        else
        {
            span.str += menu_tooltip;
        }

        para.SetSpan(0, 0, span);
        tooltip_text->SetParagraph(para);

        // widgets
        for (size_t i = 0; i < menu.widgets.size(); ++i)
        {
            auto& w = *menu.widgets.at(i);

            if (w.GetType() == widget::Type::Selection)
            {
                auto& tui_w = static_cast<ttui::Text&>(layout.GetWidget(i + 2));

                if (i == curr_widget)
                {
                    auto para = tui_w.GetParagraph();
                    para.SetAppearance(curr_appear);
                    tui_w.SetParagraph(para);
                    tui_w.SetBackgroundColor(curr_appear.bg_color);
                }
                else
                {
                    auto para = tui_w.GetParagraph();
                    para.SetAppearance(ttui::Appearance());
                    tui_w.SetParagraph(para);
                    tui_w.SetBackgroundColor(ttui::Color::Reset());
                }
            }
            else if (w.GetType() == widget::Type::List)
            {
                auto& tui_w = static_cast<ttui::List&>(layout.GetWidget(i + 2));
                auto& list = static_cast<widget::List&>(w);

                for (size_t j = 0; j < list.options.size(); ++j)
                {
                    auto item = tui_w.GetItemAt(j);

                    if (j == list.curr_option)
                    {
                        item.SetAppearance(list_selected_appear);
                    }
                    else if (i == curr_widget && j == list_widget_curr_item.at(i))
                    {
                        item.SetAppearance(curr_appear);
                    }
                    else
                    {
                        item.SetAppearance(ttui::Appearance());
                    }

                    tui_w.SetItemAt(j, item);
                }
            }
            else if (w.GetType() == widget::Type::PointAllocation)
            {
                auto& tui_w = static_cast<ttui::List&>(layout.GetWidget(i + 2));
                auto& pt_alloc = static_cast<widget::PointAllocation&>(w);

                for (size_t j = 0; j < pt_alloc.options.size(); ++j)
                {
                    auto item = tui_w.GetItemAt(j);
                    
                    if (i == curr_widget && j == pt_alloc_widget_curr_item.at(i))
                    {
                        item.SetAppearance(curr_appear);
                    }
                    else
                    {
                        item.SetAppearance(ttui::Appearance());
                    }

                    tui_w.SetItemAt(j, item);
                }
            }
            else if (w.GetType() == widget::Type::TextBox)
            {
                auto& tui_w = static_cast<ttui::Text&>(layout.GetWidget(i + 2));

                if (i == curr_widget)
                {
                    auto para = tui_w.GetParagraph();
                    para.SetAppearance(curr_appear);
                    tui_w.SetParagraph(para);
                    tui_w.SetBackgroundColor(curr_appear.bg_color);
                }
                else
                {
                    auto para = tui_w.GetParagraph();
                    para.SetAppearance(ttui::Appearance());
                    tui_w.SetParagraph(para);
                    tui_w.SetBackgroundColor(ttui::Color::Reset());
                }
            }
        }
    }

    void Menu::UpdateCursor()
    {
        auto& menu = *this->menu;
        auto& layout = static_cast<ttui::Layout&>(grid.GetWidget(1, 1));

        if (curr_widget == menu.widgets.size())
            return;

        auto& w = *menu.widgets.at(curr_widget);
        
        if (w.GetType() == widget::Type::TextBox)
        {
            auto& tui_w = static_cast<ttui::Text&>(layout.GetWidget(curr_widget + 2));

            auto pos = (column_width - 
                tui_w.GetParagraph().GetSpan(0, 0).str.size()) / 2 + 
                text_box_widget_curr_idx.at(curr_widget) - 1;
            auto rect = layout.GetWidgetRect(curr_widget + 2);

            ttui::cursor::SetPos(rect.x + 1 + pos, rect.y + 1);
            ttui::cursor::SetVisible(true);
        }
    }

    void Menu::Output(const std::string& text)
    {
        auto& menu = *this->menu;
        auto& layout = static_cast<ttui::Layout&>(grid.GetWidget(1, 1));
        auto& text_w = static_cast<ttui::Text&>(layout.GetWidget(menu.widgets.size() + 2));

        text_w.SetParagraph(ttui::Paragraph(text, ttui::Appearance()));

        auto height = text_w.GetWrappedParagraph(column_width).GetHeight();
        layout.GetDefinition(menu.widgets.size() + 2) = ttui::Definition::Absolute(height + 2);
        *draw_required = true;
    }

    void Menu::PollEvent(Game& game, ttui::Handle& handle)
    {
        auto& menu = *this->menu;
        auto& layout = static_cast<ttui::Layout&>(grid.GetWidget(1, 1));
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
                *draw_required = true;
            }
            else if (event.type == ttui::Event::Key)
            {
                if (curr_widget == menu.widgets.size())
                {
                    KeyEvent(event.key);
                }
                else
                {
                    auto& w = *menu.widgets.at(curr_widget);

                    if (w.GetType() == widget::Type::Selection)
                    {
                        auto& selection = static_cast<widget::Selection&>(w);
                        auto& tui_w = static_cast<ttui::Text&>(layout.GetWidget(curr_widget + 2));
                        SelectionKeyEvent(selection, tui_w, event.key);
                    }
                    else if (w.GetType() == widget::Type::List)
                    {
                        auto& list = static_cast<widget::List&>(w);
                        auto& tui_w = static_cast<ttui::List&>(layout.GetWidget(curr_widget + 2));
                        ListKeyEvent(list, tui_w, event.key);
                    }
                    else if (w.GetType() == widget::Type::PointAllocation)
                    {
                        auto& pt_alloc = static_cast<widget::PointAllocation&>(w);
                        auto& tui_w = static_cast<ttui::List&>(layout.GetWidget(curr_widget + 2));
                        PointAllocationKeyEvent(pt_alloc, tui_w, event.key);
                    }
                    else if (w.GetType() == widget::Type::TextBox)
                    {
                        auto& text = static_cast<widget::TextBox&>(w);
                        auto& tui_w = static_cast<ttui::Text&>(layout.GetWidget(curr_widget + 2));
                        TextBoxKeyEvent(text, tui_w, event.key);
                    }
                }
            }
        }
    }

    void Menu::KeyEvent(const ttui::KeyEvent& event)
    {
        auto& layout = static_cast<ttui::Layout&>(grid.GetWidget(1, 1));
        auto& tui_widget = static_cast<ttui::List&>(layout.GetWidget(1));

        if (event.is_esc)
        {
            if (event.code == ttui::KeyEvent::Up)
            {
                auto idx = tui_widget.GetSelectedItemIndex();
                if (idx > 0)
                {
                    tui_widget.SetSelectedItemIndex(idx - 1);
                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Down)
            {
                auto idx = tui_widget.GetSelectedItemIndex();
                if (idx < tui_widget.GetSize() - 1)
                {
                    tui_widget.SetSelectedItemIndex(idx + 1);
                    *draw_required = true;
                }
                else if (curr_widget != 0)
                {
                    auto& menu = *this->menu;
                    if (menu.widgets.size() > 0)
                    {
                        auto& w = *menu.widgets.at(0);
                        if (w.GetType() == widget::Type::List)
                        {
                            auto& list = static_cast<widget::List&>(w);
                            if (list.options.size() == 0)
                                return;
                        } 
                    }

                    curr_widget = 0;
                    UpdateWidgets();
                    *draw_required = true;
                }
            }
        }
        else if (event.code == '\n' || event.code == '\r')
        {
            input = tui_widget.GetSelectedItemIndex();
            *draw_required = true;
        }
    }

    void Menu::SelectionKeyEvent(widget::Selection& selection, ttui::Text& tui_widget, const ttui::KeyEvent& event)
    {
        auto& menu = *this->menu;

        if (event.is_esc)
        {
            if (event.code == ttui::KeyEvent::Up)
            {
                if (curr_widget > 0)
                {
                    --curr_widget;
                }
                else
                {
                    curr_widget = menu.widgets.size();
                }

                UpdateWidgets();
                *draw_required = true;
            }
            else if (event.code == ttui::KeyEvent::Down)
            {
                if (curr_widget < menu.widgets.size() - 1)
                {
                    ++curr_widget;
                    UpdateWidgets();
                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Left)
            {
                int32_t value = static_cast<int32_t>(selection.curr_option) - 1;
                while (value < 0) value += selection.options.size();
                selection.curr_option = static_cast<size_t>(value);

                UpdateSelectionWidget(selection, tui_widget);
                *draw_required = true;
            }
            else if (event.code == ttui::KeyEvent::Right)
            {
                selection.curr_option = (selection.curr_option + 1) % selection.options.size();
                UpdateSelectionWidget(selection, tui_widget);
                *draw_required = true;
            }
        }
        else if (event.code == '\n' || event.code == '\r')
        {
            selection.curr_option = (selection.curr_option + 1) % selection.options.size();
            UpdateSelectionWidget(selection, tui_widget);
            *draw_required = true;
        }
    }

    void Menu::ListKeyEvent(widget::List& list, ttui::List& tui_widget, const ttui::KeyEvent& event)
    {
        auto& menu = *this->menu;
        auto& curr_item = list_widget_curr_item.at(curr_widget);

        if (event.is_esc)
        {
            if (event.code == ttui::KeyEvent::Up)
            {
                if (curr_item == 0)
                {
                    if (curr_widget > 0)
                    {
                        --curr_widget;
                    }
                    else
                    {
                        curr_widget = menu.widgets.size();
                    }
                }
                else
                {
                    --curr_item;
                }

                UpdateWidgets();
                *draw_required = true;
            }
            else if (event.code == ttui::KeyEvent::Down)
            {
                if (curr_item == list.options.size() - 1)
                {
                    if (curr_widget < menu.widgets.size() - 1)
                    {
                        ++curr_widget;
                        UpdateWidgets();
                        *draw_required = true;
                    }
                }
                else
                {
                    ++curr_item;
                    UpdateWidgets();
                    *draw_required = true;
                }
            }
        }
        else if (event.code == '\n' || event.code == '\r')
        {
            list.curr_option = curr_item;
            UpdateWidgets();
            *draw_required = true;
        }
    }

    void Menu::PointAllocationKeyEvent(widget::PointAllocation& pt_alloc, ttui::List& tui_widget, const ttui::KeyEvent& event)
    {
        auto& menu = *this->menu;
        auto& curr_item = pt_alloc_widget_curr_item.at(curr_widget);

        if (event.is_esc)
        {
            if (event.code == ttui::KeyEvent::Up)
            {
                if (curr_item == 0)
                {
                    if (curr_widget > 0)
                    {
                        --curr_widget;
                    }
                    else
                    {
                        curr_widget = menu.widgets.size();
                    }
                }
                else
                {
                    --curr_item;
                }

                UpdateWidgets();
                *draw_required = true;
            }
            else if (event.code == ttui::KeyEvent::Down)
            {
                if (curr_item == pt_alloc.options.size() - 1)
                {
                    if (curr_widget < menu.widgets.size() - 1)
                    {
                        ++curr_widget;
                        UpdateWidgets();
                        *draw_required = true;
                    }
                }
                else
                {
                    ++curr_item;
                    UpdateWidgets();
                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Left)
            {
                auto& opt = pt_alloc.options.at(curr_item);
                if (opt.pts > opt.min)
                {
                    --opt.pts;
                    ++pt_alloc.remaining_pts;
                    tui_widget.SetItemAt(curr_item, ttui::Paragraph(pt_alloc.GetOptionStr(curr_item), curr_appear));

                    auto border = tui_widget.GetBorder();
                    border.title = pt_alloc.title + " (Remaining pts: " + std::to_string(pt_alloc.remaining_pts) + ")";
                    tui_widget.SetBorder(border);

                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Right)
            {
                auto& opt = pt_alloc.options.at(curr_item);
                if (opt.pts < opt.max && pt_alloc.remaining_pts > 0)
                {
                    ++opt.pts;
                    --pt_alloc.remaining_pts;
                    tui_widget.SetItemAt(curr_item, ttui::Paragraph(pt_alloc.GetOptionStr(curr_item), curr_appear));

                    auto border = tui_widget.GetBorder();
                    border.title = pt_alloc.title + " (Remaining pts: " + std::to_string(pt_alloc.remaining_pts) + ")";
                    tui_widget.SetBorder(border);

                    *draw_required = true;
                }
            }
        }
    }

    void Menu::TextBoxKeyEvent(widget::TextBox& text_box, ttui::Text& tui_widget, const ttui::KeyEvent& event)
    {
        auto& menu = *this->menu;

        if (event.is_esc)
        {
            if (event.code == ttui::KeyEvent::Up)
            {
                auto para = tui_widget.GetParagraph();
                auto span = para.GetSpan(0, 0);

                if (!text_box.is_minigame && text_box.validator(span.str))
                {
                    text_box.text = span.str;
                }
                else
                {
                    span.str = text_box.text;
                    text_box_widget_curr_idx.at(curr_widget) = span.str.size();
                }

                para.SetSpan(0, 0, span);
                tui_widget.SetParagraph(para);

                if (curr_widget > 0)
                {
                    --curr_widget;
                }
                else
                {
                    curr_widget = menu.widgets.size();
                }

                UpdateWidgets();
                *draw_required = true;
            }
            else if (event.code == ttui::KeyEvent::Down)
            {
                if (curr_widget < menu.widgets.size() - 1)
                {
                    auto para = tui_widget.GetParagraph();
                    auto span = para.GetSpan(0, 0);

                    if (!text_box.is_minigame && text_box.validator(span.str))
                    {
                        text_box.text = span.str;
                    }
                    else
                    {
                        span.str = text_box.text;
                        text_box_widget_curr_idx.at(curr_widget) = span.str.size();
                    }

                    para.SetSpan(0, 0, span);
                    tui_widget.SetParagraph(para);

                    ++curr_widget;
                    UpdateWidgets();
                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Left)
            {
                auto& idx = text_box_widget_curr_idx.at(curr_widget);

                if (idx > 0)
                {
                    --idx;
                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Right)
            {
                auto& idx = text_box_widget_curr_idx.at(curr_widget);

                if (idx < tui_widget.GetParagraph().GetSpan(0, 0).str.size())
                {
                    ++idx;
                    *draw_required = true;
                }
            }
            else if (event.code == ttui::KeyEvent::Delete)
            {
                auto& idx = text_box_widget_curr_idx.at(curr_widget);
                
                if (idx == tui_widget.GetParagraph().GetSpan(0, 0).str.size())
                    return;

                auto para = tui_widget.GetParagraph();
                auto span = para.GetSpan(0, 0);
                span.str.erase(idx, 1);
                para.SetSpan(0, 0, span);
                tui_widget.SetParagraph(para);

                *draw_required = true;
            }
        }
        else if (std::isprint(event.code))
        {
            auto& idx = text_box_widget_curr_idx.at(curr_widget);
            
            auto para = tui_widget.GetParagraph();
            auto span = para.GetSpan(0, 0);
            span.str.insert(idx, 1, event.code);
            para.SetSpan(0, 0, span);
            tui_widget.SetParagraph(para);

            ++idx;
            *draw_required = true;
        }
        else if (event.code == ttui::KeyEvent::Backspace)
        {
            auto& idx = text_box_widget_curr_idx.at(curr_widget);
            
            if (idx == 0)
                return;

            auto para = tui_widget.GetParagraph();
            auto span = para.GetSpan(0, 0);
            span.str.erase(idx - 1, 1);
            para.SetSpan(0, 0, span);
            tui_widget.SetParagraph(para);

            --idx;
            *draw_required = true;
        }
        else if (text_box.is_minigame && (event.code == '\n' || event.code == '\r'))
        {
            text_box_widget_curr_idx.at(curr_widget) = 0;

            auto para = tui_widget.GetParagraph();
            auto span = para.GetSpan(0, 0);

            if (text_box.validator(span.str))
                text_box.text = span.str;

            span.str = "";

            para.SetSpan(0, 0, span);
            tui_widget.SetParagraph(para);

            *draw_required = true;
        }
    }

    namespace
    {
        void UpdateSelectionWidget(const widget::Selection& selection, ttui::Text& text)
        {
            auto para = text.GetParagraph();
            auto span = para.GetSpan(0, 0);
            span.str = selection.options.at(selection.curr_option);
            para.SetSpan(0, 0, span);
            text.SetParagraph(para);
        }
    }
}