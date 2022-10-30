#include <Game.h>

int main(int argc, char** argv)
{
    bool cli = false;
    std::string exe_path;

    if (argc > 0)
        exe_path = std::string(argv[0]);

    if (argc > 1 && argv[1][0] == 'c')
        cli = true;

    Game game(cli, exe_path);
    game.Run();

    return 0;
}