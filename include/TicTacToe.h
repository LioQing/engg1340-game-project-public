#pragma once
#include <iostream>
#include <vector>

class TicTacToe
{
    private:

    
    char ttt_board[3][3] = {{'1', '2', '3'},
                            {'4', '5', '6'},
                            {'7', '8', '9'}};

    bool players_turn;

    int total_moves = 0;

    std::vector<char> players_pos;

    std::vector<char> ai_pos;

    public:

    /**
     * @brief To print board
     * 
     */
    void PrintBoard(); //done

    /**
     * @brief To change the turns
     * 
     */
    void AlternateTurns(); //done

    /**
     * @brief Make changes onto the Board
     * 
     * @param input 
     */
    void ChangeBoard(const char input); //done

    /**
     * @brief Allocate players' move
     * 
     * @param input 
     */


    void PlayersMove(const char input); //done

    /**
     * @brief Get the Input object
     * 
     * @return char 
     */
    char GetInput(); //done

    /**
     * @brief AI's move
     * 
     */
    void AIMove(); //done

    /**
     * @brief Initialize game
     * 
     */
    void Init(); //done

    /**
     * @brief Check is the input available
     * 
     * @param input 
     * @return true 
     * @return false 
     */
    bool Exists(const char input); //done

    /**
     * @brief Run the game with Gameloop
     * 
     */
    void GameLoop(); //done

    /**
     * @brief check whether the game should be stopped
     * 
     * @return true 
     * @return false 
     */
    bool GameEnd(); //done

    /**
     * @brief Check if the player/AI has won the game
     * 
     * @param positions 
     * @return true 
     * @return false 
     */
    bool Win(std::vector<char> positions); //done

    /**
     * @brief check to see if the game is a draw
     * 
     * @return true 
     * @return false 
     */
    bool Draw(); //done

    /**
     * @brief Print Result of the game
     * 
     */
    void PrintResult(); //done

    void PrintHowToPlay();

};