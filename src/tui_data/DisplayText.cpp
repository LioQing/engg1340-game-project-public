#include <tui_data/DisplayText.h>

#include <Game.h>

namespace tui_data
{
    const std::string DisplayText::tooltip = "Any key to skip/continue;";

    void DisplayText::InitWidgets()
    {
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
        
        // text
        {
            // layout
            ttui::Layout layout;
            layout.SetDirection(ttui::Direction::Vertical);

            // definitions
            layout.SetDefinitions(std::vector<ttui::Definition>(3, ttui::Definition::Fit()));

            // text
            ttui::Text text_widget;
            text_widget.SetBorder(ttui::Border::Single());
            text_widget.SetHorizAlign(ttui::Align::Center);
            text_widget.SetWrap(ttui::Wrap::Word);
            text_widget.SetParagraph(ttui::Paragraph("", ttui::Appearance()));

            layout.GetDefinition(1) = ttui::Definition::Absolute(3);
            layout.SetWidget(1, text_widget);

            grid.SetWidget(1, 1, std::move(layout));
        }

        // tooltip
        auto para = tooltip_text->GetParagraph();
        auto span = para.GetSpan(0, 0);
        span.str = tooltip;
        para.SetSpan(0, 0, span);
        tooltip_text->SetParagraph(para);

        // timer
        start_time = Clock::now();

        UpdateWidgets();
        
        *draw_required = true;
    }

    void DisplayText::PollEvent(Game& game, ttui::Handle& handle)
    {
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
                if (finished_anim)
                    input = 0;
                else
                    finished_anim = true;
            }
        }

        UpdateWidgets();
    }

    void DisplayText::UpdateWidgets()
    {
        auto& text = *this->text;
        auto& layout = static_cast<ttui::Layout&>(grid.GetWidget(1, 1));
        auto& text_w = static_cast<ttui::Text&>(layout.GetWidget(1));

        if (finished_anim)
            text_w.SetParagraph(ttui::Paragraph(text.text, ttui::Appearance()));
        else
            text_w.SetParagraph(ttui::Paragraph(GetCurrText(), ttui::Appearance()));

        auto height = text_w.GetWrappedParagraph(column_width).GetHeight();
        height = height < 1 ? 1 : height;

        layout.GetDefinition(1) = ttui::Definition::Absolute(height + 2);
        *draw_required = true;
    }

    std::string DisplayText::GetCurrText()
    {
        auto now = Clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
        auto& text = *this->text;
        auto curr_char = elapsed / ms_per_char;

        if (curr_char >= static_cast<int64_t>(text.text.size()))
        {
            finished_anim = true;
            return text.text;
        }

        return text.text.substr(0, curr_char);
    }
}