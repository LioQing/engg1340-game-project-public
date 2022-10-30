#pragma once

#include <iostream>
#include <vector>
#include <Random.h>
#include <string>

/**
 * @brief a hangman-like game
 * @brief class created for Minigame
 */

class Hangman
{

private:

    std::string word_tbg; //solution/ answer for minigame
    
    int tries_left = 8;

    std::vector<char> guessed_letters;

    std::string word_guessed;

    std::ostream &output = std::cout;

public:
    /**
     * @brief Print out how to play hangman minigame
     * 
     */

    void PrintHowToPlay();

    /**
     * @brief determined has player used up their guessing tries already
     * 
     * @return true 
     * @return false 
     */

    bool HasTryLeft() const;

    /**
     * @brief Get the Answer Word object
     * 
     * @return const std::string& 
     */
    const std::string& GetAnswerWord() const;

    /**
     * @brief Initialize minigame
     * getting words from Hangman.txt to run the game 
     * 
     * @param file_path the path to the Hangman.txt
     */

    void Init(const std::string& file_path);

    /**
     * @brief Check if the letter has been guessed previously or not
     * 
     * @param temp 
     * trying to match temp with character stored in vector guessed_letters
     * @return true 
     * @return false 
     */
    bool Run(char temp);

    /**
     * @brief to run the game by calling Gameloop() after initializing
     * 
     */
    void GameLoop();

    /**
     * @brief to check if the word is successfully guessed or not
     * 
     * @return true 
     * @return false 
     */
    bool Result();

    /**
     * @brief to print out message to player to show the letter has been guessed previously
     * 
     * @param os 
     */
    void OutputAlreadyGuessed(std::ostream& os);

    /**
     * @brief To overload << operator for outputting Minigame object
     * 
     * @param output 
     * @param m 
     * @return std::ostream& 
     */
    friend std::ostream &operator<<(std::ostream &output, const Hangman &m);


};