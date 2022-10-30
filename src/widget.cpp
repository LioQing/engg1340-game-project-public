#include <widget.h>

namespace widget
{
    PointAllocation::Option::Option(const std::string& name, size_t& pts, size_t min, size_t max, size_t multiplier)
        : name(name), pts(pts), min(min), max(max), multiplier(multiplier)
    {
    }

    PointAllocation::PointAllocation(const std::string& title, const std::vector<Option>& options, size_t remaining_pts)
        : options(options), remaining_pts(remaining_pts)
    {
        this->title = title;
    }

    std::string PointAllocation::GetOptionStr(size_t i) const
    {
        auto& opt = options.at(i);
        return opt.name + " (" + std::to_string(opt.multiplier) + " per pt, min: " +
            std::to_string(opt.min) + ", max: " + std::to_string(opt.max) + "): " +
            std::to_string(opt.pts * opt.multiplier) + " (" + std::to_string(opt.pts) + " pts)";
    }

    Type PointAllocation::GetType() const
    {
        return Type::PointAllocation;
    }

    std::pair<bool, std::string> PointAllocation::Update(std::istream& input)
    {
        std::string str;
        std::getline(input, str);

        if (input.fail() || str.empty())
        {
            return std::make_pair(false, std::string("Invalid input"));
        }

        std::stringstream iss(str);

        int choice;
        int new_value;
        iss >> choice;

        // check if input is valid
        if (iss.fail())
            return std::make_pair(false, std::string("Invalid input"));

        if (choice == 0)
            return std::make_pair(true, std::string());

        choice -= 1;

        if (choice < 0 || choice >= static_cast<int>(options.size()))
            return std::make_pair(false, std::string("Option out of range"));
        
        iss >> new_value;

        if (iss.fail() || iss.peek() != EOF)
            return std::make_pair(false, std::string("Invalid input"));

        if (new_value < static_cast<int>(options.at(choice).min))
            return std::make_pair(false, std::string("Value less than allowed minimum"));

        if (new_value > static_cast<int>(options.at(choice).max))
            return std::make_pair(false, std::string("Value greater than allowed maximum"));

        if (new_value - options.at(choice).pts > remaining_pts)
            return std::make_pair(false, std::string("Not enough points left"));
        
        remaining_pts -= new_value - options.at(choice).pts;
        options.at(choice).pts = new_value;

        return std::make_pair(true, std::string());
    }

    TextBox::TextBox(const std::string& title, std::string& text)
        : text(text)
    {
        this->title = title;
        validator = [](const std::string& str) { return true; };
    }

    TextBox::TextBox(const std::string& title, std::string& text, std::function<bool(const std::string&)> validator, bool is_minigame)
        : text(text), validator(validator), is_minigame(is_minigame)
    {
        this->title = title;
    }

    Type TextBox::GetType() const
    {
        return Type::TextBox;
    }

    std::pair<bool, std::string> TextBox::Update(std::istream& input)
    {
        std::string str;
        std::getline(input, str);

        if (input.fail() || str.empty())
        {
            return std::make_pair(false, std::string("Invalid input"));
        }

        if (str == ":c")
        {
            return std::make_pair(true, std::string());
        }

        if (!validator(str))
        {
            return std::make_pair(false, std::string("Invalid input"));
        }

        text = str;
        return std::make_pair(true, std::string());
    }
}