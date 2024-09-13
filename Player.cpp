#include "player.h"
#include "game.h"
#include <string>
#include <cstdlib>

using namespace std;

Player::Player() // default constructor
{
    _stamina = 0;
    _gold = 0;
    _effect = "";
    _positions.push_back(0);

    Candy emptyCandy = {"", "", "", 0, ""};
    for (int i = 0; i < _max_candy_amount; i++)
    {
        _inventory[i] = emptyCandy;
    }
    _candy_amount = 0;
    loadAllCandies();
}

Player::Player(string name, int stamina, double gold, string effect, Candy candy[], const int size) // parameterized constructor
{
    _name = name;
    _stamina = stamina;
    _gold = gold;
    _effect = effect;
    _positions.push_back(0);

    for (int i = 0; i < _max_candy_amount && i < size; i++)
    {
        _inventory[_candy_amount] = candy[i];
        if (candy[i].name != "" && candy[i].description != "" && candy[i].price != 0.0 && candy[i].candy_type != "")
        {
            _candy_amount++;
        }
    }
    loadAllCandies();
}

void Player::setName(string name)
{
    _name = name;
}

string Player::getName()
{
    return _name;
}

int Player::getCandyAmount() const
{
    return _candy_amount;
}

void Player::setStamina(int stamina)
{
    _stamina = stamina;
}

int Player::getStamina() const
{
    return _stamina;
}

void Player::setGold(int gold)
{
    if (gold < 0)
    {
        _gold = 0;
    }
    else
    {
        _gold = gold;
    }
}

double Player::getGold() const
{
    return _gold;
}

void Player::setEffect(string effect)
{
    _effect = effect;
}

string Player::getEffect() const
{
    return _effect;
}

void Player::setCharacter(Character c) // set the player's character
{
    _character = c;
    setGold(_character.gold);
    setStamina(_character.stamina);
    setInitialInventory();
}

Character Player::getCharacter() const
{
    return _character;
}

void Player::setPosition(int position) // adds positions to a position vector
{
    _positions.push_back(position);
}

int Player::getPosition() const // gets the last position at the last index in the positions vector
{
    return _positions.at(_positions.size() - 1);
}

void Player::removeLastPos() // removes the position at the end
{
    _positions.pop_back();
}

void Player::setShield(bool b) // sets whether they have the shield or not for when players land on the same tile
{
    hasShield = b;
}

bool Player::checkShield() // checks whether they have a shield
{
    return hasShield;
}

Candy Player::getCandyAtIndex(int index) // gets the candy in a spot in their inventory
{
    return _inventory[index];
}

void Player::printInventory() // prints out the inventory
{
    for (int i = 0; i < _max_candy_amount; i++)
    {
        string candyName = _inventory[i].name;
        if (candyName == "")
        {
            candyName = "Empty";
        }

        if (i % 3 == 0)
        {
            cout << "[" << candyName << "]";
        }
        else if (i % 3 == 1)
        {
            cout << "[" << candyName << "]";
        }
        else
        {
            cout << "[" << candyName << "]" << endl;
        }
    }
    // cout << _inventory[0].effect_type << " " << _inventory[0].effect_value << endl;
    // cout << _inventory[8].effect_type << " " << _inventory[8].effect_value << endl;
}

Candy Player::findCandy(string candy_name) // finds a candy in their inventory
{
    string candy_lower = "";
    for (int i = 0; i < (int)candy_name.length(); i++)
    {
        char lower = tolower(candy_name[i]);
        candy_lower = candy_lower + lower;
    }

    Candy temp;

    for (int i = 0; i < _max_candy_amount; i++)
    {
        string currentCandyName = _inventory[i].name;
        string current_candy_lower = "";
        for (int j = 0; j < (int)currentCandyName.length(); j++)
        {
            current_candy_lower += tolower(currentCandyName[j]);
        }

        if (current_candy_lower == candy_lower)
        {
            temp = _inventory[i];
            return temp;
        }
    }

    return temp;
}

bool Player::findImmunityCandy(string candy_name) // finds immunity candies for when players use poison candies
{
    string candy_lower = "";
    for (int i = 0; i < (int)candy_name.length(); i++)
    {
        char lower = tolower(candy_name[i]);
        candy_lower = candy_lower + lower;
    }

    for (int i = 0; i < _max_candy_amount; i++)
    {
        string currentCandyName = _inventory[i].name;
        string current_candy_lower = "";
        for (int j = 0; j < (int)currentCandyName.length(); j++)
        {
            current_candy_lower += tolower(currentCandyName[j]);
        }

        if (current_candy_lower == candy_lower)
        {
            return true;
        }
    }

    return false;
}

void Player::loadAllCandies() // loads all the candies in order to match them from the character menu to actual candies
{
    // cout << "Start of loadCandies" << endl;
    string candyName;
    string description;
    string effectType;
    int effectValue;
    double price;
    string type;
    Candy temp;

    ifstream list("candy.txt");

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

            everyCandy.push_back(temp);
        }
    }
}

Candy Player::findCandyForInitial(string name) // finds the candies from the entire list of candies
{

    string candy_lower = "";
    for (int i = 0; i < (int)name.length(); i++)
    {
        char lower = tolower(name[i]);
        candy_lower += lower;
    }

    Candy temp;

    for (int i = 0; i < (int)everyCandy.size(); i++)
    {
        string currentCandyName = everyCandy[i].name;
        string current_candy_lower = "";
        for (int j = 0; j < (int)currentCandyName.length(); j++)
        {
            current_candy_lower += tolower(currentCandyName[j]);
        }

        if (current_candy_lower == candy_lower)
        {
            temp = everyCandy[i];
            return temp;
        }
    }

    return temp;
}

void Player::setInitialInventory() // sets the inventory for the character at the start by finding the candies from the character's inventory
{
    _candy_amount = 0;
    for (int i = 0; i < 6; i++)
    {
        string candyName = _character.inventory[i];
        addCandy(findCandyForInitial(candyName));
    }
}

bool Player::addCandy(Candy candy) // adds candy to the player's inventory
{
    if (_candy_amount == 9)
    {
        cout << "Inventory full. Cannot add more candies" << endl;
        return false;
    }
    else
    {
        _inventory[_candy_amount] = candy;
        _candy_amount++;
        return true;
    }
}

bool Player::removeCandy(string candy_name) // removes candy from a player's inventory
{
    string candy_name_lower = "";
    for (int i = 0; i < (int)candy_name.length(); i++)
    {
        char lower = tolower(candy_name[i]);
        candy_name_lower = candy_name_lower + lower;
    }
    int indexCandy = -1;
    for (int i = 0; i < _max_candy_amount; i++)
    {
        string currentCandyName = _inventory[i].name;
        string current_candy_lower = "";
        for (int j = 0; j < (int)currentCandyName.length(); j++)
        {
            current_candy_lower += tolower(currentCandyName[j]);
        }

        if (current_candy_lower == candy_name_lower)
        {
            indexCandy = i;
            break;
        }
    }

    if (indexCandy == -1)
    {
        return false;
    }
    else
    {
        for (int i = indexCandy; i < _max_candy_amount - 1; i++)
        {
            _inventory[i] = _inventory[i + 1];
        }

        _inventory[_max_candy_amount - 1] = {"", "", "", 0, ""};
        _candy_amount--;
        return true;
    }
}

void Player::increaseSkipAmount() // increases number of skip turns
{
    _skipAmount++;
}

void Player::decreaseSkipAmount() // decreases number of skip turns
{
    _skipAmount--;
}

int Player::getSkipAmount() const // returns the skip amount
{
    return _skipAmount;
}

string Player::getCandyNames() // returns the player's candy names
{
    string candies = "";
    for (int i = 0; i < _max_candy_amount; i++)
    {
        string candyName = _inventory[i].name;
        if (candyName == "")
        {
            candyName = "Empty";
        }
        candies = candies + "[" + candyName + "] ";
    }
    return candies;
}

void Player::printPositions() // prints out the positions of the player (for debugging)
{
    for (int i = 0; i < (int)_positions.size(); i++)
    {
        cout << _positions.at(i) << " ";
    }
}
