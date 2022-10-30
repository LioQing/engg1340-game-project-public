/**
 * Run the minigame by using GameLoop()
 * 
 */

#include <Hangman.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <Random.h>
#include <string>

void Hangman::PrintHowToPlay()
{
    std::cout << "In case you don't know how to play Hangman, here is the rules \n\n";

    std::cout << "RULES OF HANGMAN: \n\n";

    std::cout << "1. A secret word is randomly selected by AI system. In this game, you will have to guess the secret word letter-by-letter. \n";

    std::cout << "2. You have a total of 8 quotas to guess the secret word.\n";

    std::cout << "3. For each wrong guess, you will consume a quota for guessing letters. For each correct guess, your quota is retained. \n";

    std::cout << "4. After you have successfully guessed a letter, you can also see the position of the letter in the word. \n";
    
    std::cout << "5. There are some hints you can follow, for instance, number of letters and the position of letter guessed.\n";

    std::cout << "6. You lose when you have used up all your quotas for guessing letters. \n";

    std::cout << "7. You win when you have succesfully guessed all the letters in the word. \n";

    std::cout << "Good luck and have fun guessing!\n";
}

/**
 * @brief to determine whether the minigame should be ended or not
 * 
 * 
 * @return true (to continue the game) 
 * @return false  (to break loop)
 */
bool Hangman::HasTryLeft() const
{
    return tries_left <= 8 && tries_left > 0;
}

/**
 * @brief get answer
 * 
 * @return const std::string& 
 */

const std::string& Hangman::GetAnswerWord() const
{
    return word_tbg;
}


void Hangman::OutputAlreadyGuessed(std::ostream& output)
{
    output << "You have guessed this letter already. Please try again." << std::endl;
}

std::ostream &operator<<(std::ostream &output, const Hangman &m)
{
    output << "You have " << m.tries_left << " tries left." << std::endl;
    output << "The word consists of " << m.word_tbg.length() << " letters." << std::endl;
    output << "Your guess: " << m.word_guessed << std::endl;
    output << "Letters you have guessed:" << std::endl;
    for (int i = 0; i < (int) m.guessed_letters.size(); i++)
    {
        output << m.guessed_letters[i] << " ";
    }
    output << std::endl;

    output << "Make your guess (Input a letter)";
    return output;
}

void Hangman::Init(const std::string& file_path)
{

    std::ifstream file;

    file.open(file_path);

    int line_no = Random().GetInt(0, 2443);

    std::string temp;

    int i = 0;

    if (file.fail())
    {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    while (file >> temp)
    {
        if (i == line_no)
        {
            word_tbg = temp;
            break;
        }
        else
        {
            i++;
            continue;
        }
    }

    /**
     * @brief Get a string to store what is being correctly guessed (as a word)
     * @brief Replace every character with asterisk
     * 
     */

    for (int i = 0; i < (int) word_tbg.length(); i++)
    {
        word_tbg[i] = tolower(word_tbg[i]);

        word_guessed += '*';
    }
}

/**
 * @brief 
 * 
 * @param temp 
 * @return true 
 * @return false 
 */

bool Hangman::Run(char temp)
{

    bool flag = true;
    bool flag2 = false;

    for (int i = 0; i < (int) guessed_letters.size(); i++)
    {
        if (temp == guessed_letters[i])
        {
            flag = false;
            return false;
        }
    }
    if (flag == true)
    {
        guessed_letters.push_back(temp);
    }
    if (flag)
    {
        for (int i = 0; i < (int) word_tbg.length(); i++)
        {
            if (temp == word_tbg[i])
            {
                word_guessed[i] = temp;
                flag2 = true;
            }
        }

        if (flag2 == false)
        {
            tries_left -= 1;
        }
    }

    return true;
}

/**
 * @brief To determine whether the word has been successfully guessed
 * 
 * @return true 
 * @return false 
 */

bool Hangman::Result()
{
    return (word_guessed == word_tbg);
}

void Hangman::GameLoop()
{
    Hangman minigame;
    minigame.Init("Hangman.txt");

    minigame.PrintHowToPlay();

    while (minigame.HasTryLeft())
    {
        // print out "How many tries left"
        // print out "How much letter in the word"
        // print out "*" if the letter is not yet guessed else the letter
        // print out the letters guessed <vector is used to store the guesses>
        // avoid uppercase or lowercase letters 
        
        std::cout << minigame << std::endl;

        char temp;
        std::cin >> temp;
        if (!minigame.Run(temp))
        {
            minigame.OutputAlreadyGuessed(std::cout);
        }

        if (minigame.Result() == true)
        {
            break;
        }
    }

    //print out the result of game

    if (minigame.Result() == false)
    {
        std::cout << "Too bad you didn't guessed the word." << std::endl;
        std::cout << "The answer is " << minigame.GetAnswerWord() << "." << std::endl;
    }
    else
    {
        std::cout << "Congrats! You have successfully guessed the word" << minigame.GetAnswerWord()  << "."<< std::endl;
    }
}