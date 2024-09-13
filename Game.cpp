#include "game.h"
#include "player.h"
#include <vector>
#include <cstdlib>

Game::Game(vector<Candy> candies, vector<Character> character) // constructor to start the game
{
    _candies = candies;
    _characters = character;
    loadCandies("candy.txt");
    loadCharacters("characters.txt");
}

void Game::setCandies(vector<Candy> candy)
{
    _candies = candy;
}

vector<Candy> Game::getCandies() const
{
    return _candies;
}

void Game::setCharacters(vector<Character> characters)
{
    _characters = characters;
}

vector<Character> Game::getCharacters() const
{
    return _characters;
}

void Game::loadCandies(string filename) // load all the candies into a vector
{
    // cout << "Start of loadCandies" << endl;
    string candyName;
    string description;
    string effectType;
    int effectValue;
    double price;
    string type;
    Candy temp;

    ifstream list(filename);

    if (list.fail())
    {
        cout << "Failed to open file" << endl;
        return;
    }

    string line;
    getline(list, line); // skip first line
    while (getline(list, line))
    {
        if (line.length() > 0)
        {

            stringstream ss(line);
            string token;
            for (int i = 0; i < 6; i++)
            {
                getline(ss, token, '|');
                if (i == 0)
                {
                    candyName = token;
                }
                else if (i == 1)
                {
                    description = token;
                }
                else if (i == 2)
                {
                    effectType = token;
                }
                else if (i == 3)
                {
                    effectValue = stoi(token);
                }
                else if (i == 4)
                {
                    type = token;
                }
                else if (i == 5)
                {
                    price = stod(token);
                }
            }

            temp.name = candyName;
            temp.description = description;
            temp.effect_type = effectType;
            temp.effect_value = effectValue;
            temp.price = price;
            temp.candy_type = type;

            _candies.push_back(temp);
        }
    }
}

void Game::loadCharacters(string filename) // load all characters into a vector
{
    string characterName;
    int stamina;
    int gold;
    Character temp;

    ifstream list(filename);

    if (list.fail())
    {
        cout << "Failed to open file" << endl;
        return;
    }

    string line;
    getline(list, line); // skip first line
    while (getline(list, line))
    {
        if (line.length() > 0)
        {

            stringstream ss(line);
            string token;
            for (int i = 0; i < 3; i++)
            {
                getline(ss, token, '|');
                if (i == 0)
                {
                    characterName = token;
                }
                if (i == 1)
                {
                    stamina = stoi(token);
                }
                if (i == 2)
                {
                    gold = stoi(token);
                }
            }

            temp.character_name = characterName;
            temp.stamina = stamina;
            temp.gold = gold;
            for (int i = 0; i < 9; i++)
            {
                getline(ss, token, ',');
                temp.inventory[i] = token;
            }

            _characters.push_back(temp);
        }
    }
}

/*
1. prompt user to enter their name and set the player name
2. print out the list of characters from the _characters vector in an easy to read format
3. take in the name of the character the player wants to use, and remove that from the _characters vector
4. ask the second player and repeat steps 2 and 3 for that player, and continue for any other players
        - this is completed with a for loop based on the number that the user gives when asked how many people are playing*/
Character Game::pickCharacter()
{
    Character temp;
    cout << "Awesome! Here is a list of characters a player can select from:" << endl;
    cout << "-----------------------------------------------------------" << endl;
    for (int i = 0; i < (int)_characters.size(); i++)
    {
        cout << "Name: " << _characters.at(i).character_name << endl;
        cout << "Stamina: " << _characters.at(i).stamina << endl;
        cout << "Gold: " << _characters.at(i).gold << endl;
        cout << "Candies: ";
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
            {
                cout << endl;
                cout << "[" << _characters.at(i).inventory[j] << "]";
            }

            else
            {
                cout << "[" << _characters.at(i).inventory[j] << "]";
            }
        }
        cout << endl
             << "-----------------------------------------------------------" << endl;
    }

    bool characterSet = false;
    do
    {
        cout << "Type the name of the character you want" << endl;
        string choice;
        getline(cin, choice);
        string playerChoice = "";
        for (int i = 0; i < (int)choice.length(); i++) // makes it case insensitive
        {
            char lower = tolower(choice[i]);
            playerChoice += lower;
        }

        for (int i = 0; i < (int)_characters.size(); i++)
        {
            string currentCharName = _characters[i].character_name;
            string currentChar_lower = "";
            for (int j = 0; j < (int)currentCharName.length(); j++)
            {
                currentChar_lower += tolower(currentCharName[j]);
            }

            if (currentChar_lower == playerChoice)
            {
                temp = _characters[i];
                characterSet = true;
            }
        }

        if (!characterSet)
        {
            cout << "Invalid name. Please try again." << endl;
        }
    } while (!characterSet);

    vector<Character> newList; // removes the chosen character from the list so they can't have the same character
    bool found = false;
    for (int i = 0; i < (int)_characters.size(); i++)
    {
        if ((_characters[i].character_name == temp.character_name) && !found)
        {
            found = true;
        }
        else
        {
            newList.push_back(_characters[i]);
        }
    }

    setCharacters(newList);

    return temp;
}
