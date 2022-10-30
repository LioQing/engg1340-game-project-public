#include <TicTacToe.h>
#include <Random.h>
#include <algorithm>
#include <iostream>


void TicTacToe::Init()
{
    players_turn = true;
}

void TicTacToe::PrintHowToPlay()
{
    std::cout << "In case you don't know how to play Tic-Tac-Toe, here is the rules. \n\n";

    std::cout << "RULES FOR TIC-TAC-TOE\n";

    std::cout << "1. The game is played on a grid that's 3 squares by 3 squares.\n";
    
    std::cout << "2. You are X, your friend (or the computer in this case) is O. Players take turns putting their marks in empty squares.\n";

    std::cout << "3. The first player to get 3 of her marks in a row (up, down, across, or diagonally) is the winner.\n";

    std::cout << "4. When all 9 squares are full, the game is over. If no player has 3 marks in a row, the game ends in a tie.\n";

    std::cout << "Try your best to beat AI! Good luck!";

}

void TicTacToe::AlternateTurns()
{
    players_turn = !players_turn;
}

/**
 * @brief to print the board out
 * 
 * 9 3x3 boxes to contain a character / number representing the name of box
 * 
 */
void TicTacToe::PrintBoard()
{
    std::cout << "---------------" << std::endl;
    for (int i = 0; i < 3; i++)
    {
        for (int n = 0; n < 3; n++)
        {
            std::cout << "| " <<' '<< " |";
        }

        std::cout << "\n";

        for (int j = 0; j < 3; j++)
        {   
            std::cout << "| " << ttt_board[i][j] << " |";
        }

        std::cout << "\n";

        for (int n = 0; n < 3; n++)
        {
            std::cout << "| " <<' '<< " |";
        }

        std::cout << "\n";

        std::cout << "---------------" << std::endl;
    }
}
/**
 * Reminder
 * 
 * @param input 
 * @return true means it is available for not occupied
 * @return false 
 */
bool TicTacToe::Exists(const char input)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (input == ttt_board[i][j])
            {
                return true;
            }
            else
            {
                continue;
            }
        }
    }
    return false;
}

void TicTacToe::ChangeBoard(const char input)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if (ttt_board[i][j] == input)
            {
                if (players_turn)
                {
                    ttt_board[i][j] = 'X';
                }
                else
                {
                    ttt_board[i][j] = 'O';
                }
            }
        }
    }
}

char TicTacToe::GetInput()
{
    std::string input;
    std::cout << "Input the number of box for your move: ";
    std::cin >> input;

    while (input.length() > 1)
    {
        std::cout << "Sorry, the number of box you attempted does not match the table.";
        std::cout << "Input the number of box for your move: ";
        std::cin >> input;       
    }
   while (!Exists(input[0]) )
   {
        std::cout << "Sorry, the number of box is already occupied.\n";
        std::cout << "Input the number of box for your move: ";
        std::cin >> input;
   }

    return input[0];
}


void TicTacToe::AIMove()
{
    std::cout << "AI's Move: ";
    char input;
    input = '0' + ( Random().GetInt(1, 9) );
    while (!Exists(input))
    {
        input = '0' + ( Random().GetInt(1, 9) );
    }

    std::cout << input << std::endl;
    ai_pos.push_back(input);

    ChangeBoard(input);

    AlternateTurns();
}

void TicTacToe::PlayersMove(const char input)
{
    ChangeBoard(input);
    AlternateTurns();
    players_pos.push_back(input);
}

void TicTacToe::GameLoop()
{

    while (!GameEnd())
    {
        PrintBoard();

        if (players_turn)
        {
            PlayersMove(GetInput());
        }
        else
        {
            AIMove();
        }

        total_moves++;
    }

    PrintResult();
}

bool TicTacToe::Win(std::vector<char> positions)
{
    if ((int) positions.size() < 3)
    {
        return false;
    }
    std::sort(positions.begin(), positions.end());

    char temp1, temp2, temp3;

    for (int i = 0; i < (int) positions.size(); i++)
    {
        temp1 = positions[i];

        for (int j = i; j < (int) positions.size(); j++)
        {
            temp2 = positions[j];
            for (int k = i; k < (int) positions.size(); k++)
            {
                temp3 = positions[k];

                if ( (temp1 == '1' && temp2 == '2' && temp3 == '3') || //1
                (temp1 == '1' && temp2 == '4' && temp3 == '7') || //2
                (temp1 == '1' && temp2 == '5' && temp3 == '9') || //3
                (temp1 == '2' && temp2 == '5' && temp3 == '8') || //4
                (temp1 == '3' && temp2 == '3' && temp3 == '7') || //5
                (temp1 == '3' && temp2 == '6' && temp3 == '9') || //6
                (temp1 == '4' && temp2 == '5' && temp3 == '6') || //7
                (temp1 == '7' && temp2 == '8' && temp3 == '9') )  //8
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool TicTacToe::GameEnd()
{
    return (total_moves >= 9 || Win(ai_pos) == true || Win(players_pos) == true);
}

bool TicTacToe::Draw()
{
    if (Win(ai_pos) == false && Win(players_pos) == false)
    { 
        return false;
    }
    
    return total_moves >= 9;
}

void TicTacToe::PrintResult()
{
    if (Win(ai_pos) == true)
    {
        std::cout << "AI has won the game\n";
    }
    else if (Win(players_pos) == true)
    {
        std::cout << "Congrats! You have won the game!\n";
    }
    else if (Draw())
    {
        std::cout << "Draw game!\n";
    }
}