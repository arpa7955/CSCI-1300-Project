#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <iostream>

using namespace std;

struct Candy // candy struct that is used in all other classes
{
    string name;        // name of candy
    string description; // description of candy
    string effect_type;
    int effect_value;
    string candy_type; // type of candy
    int price;         // price of candy
};

struct Character // character struct for the player's character
{
    string character_name;
    int stamina;
    int gold;
    string inventory[9];
};

class Player
{
private:
    const static int _max_candy_amount = 9;
    Candy _inventory[_max_candy_amount];
    string _name;
    string _effect;
    Character _character;
    int _stamina;
    int _gold;
    int _candy_amount;
    vector<int> _positions;
    bool hasShield = false;
    vector<Candy> everyCandy;
    int _skipAmount;

public:
    Player();
    Player(string name, int stamina, double gold, string effect, Candy candy[], const int size);

    int getCandyAmount() const;
    void setStamina(int stamina);
    void setPosition(int position);
    int getPosition() const;
    void removeLastPos();
    Candy getCandyAtIndex(int index);

    int getStamina() const;
    void setGold(int gold);
    double getGold() const;
    void setName(string name);
    string getName() const;
    void setCharacter(Character c);
    Character getCharacter() const;
    string getName();

    void setShield(bool b);
    bool checkShield();

    void setEffect(string effect);
    string getEffect() const;

    void printInventory();
    Candy findCandy(string candy_name);

    Candy findCandyForInitial(string name);
    bool findImmunityCandy(string candy_name);
    void loadAllCandies();
    void setInitialInventory();

    bool addCandy(Candy candy);
    bool removeCandy(string candy_name);
    void increaseSkipAmount();
    void decreaseSkipAmount();
    int getSkipAmount() const;

    string getCandyNames();
    void printPositions();
};
#endif