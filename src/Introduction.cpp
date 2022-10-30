#include <Introduction.h>

#include <Game.h>
#include <InMap.h>

void Introduction::Start(Game& game)
{
    game.GetUI().SetDisplayText(
        R"(One day the City of Birds got invaded by the Sugar army.
The birds became overweighted and lost the ability to fly.

The only bird that can save the city is you, Zion!
You remain the ability to fly
because of your sugar deficiency.

Venture into the Castle of Sugar
and get the Insulin to save your city.)"
    );
}

void Introduction::Update(Game& game)
{
    if (auto choice = game.GetUI().Input())
    {
        if (choice == 0)
        {
            game.ChangeScene<InMap>();
        }
    }
}