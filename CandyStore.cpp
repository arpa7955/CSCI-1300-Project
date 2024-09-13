#include "candyStore.h"
#include <cstdlib>
#include <fstream>
#include <sstream>

Candystore::Candystore() // default constructor
{
    _store_name = "Cool Candies Store";
    vector<Candy> allCandies = readCandiesFromFile();
    pickRandomCandies(allCandies);
}
Candystore::Candystore(string name) // parameterized constructor
{
    _store_name = name;
    vector<Candy> allCandies = readCandiesFromFile();
    pickRandomCandies(allCandies);
}
void Candystore::setStoreName(string name)
{
    _store_name = name;
}

string Candystore::getStoreName() const
{
    return _store_name;
}

vector<Candy> Candystore::readCandiesFromFile() // store all candies in candy.txt to a vector of candies
{
    vector<Candy> candies;
    ifstream file("candy.txt");

    if (file.is_open())
    {
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            Candy candy;

            getline(iss, candy.name, '|');
            getline(iss, candy.description, '|');
            getline(iss, candy.effect_type, '|');
            string effectVal;
            getline(iss, effectVal, '|');
            candy.effect_value = stoi(effectVal);
            getline(iss, candy.candy_type, '|');
            string price;
            getline(iss, price, '|');
            candy.price = stoi(price);

            candies.push_back(candy);
        }
        file.close();
    }
    return candies;
}

void Candystore::pickRandomCandies(const vector<Candy> allCandies) // select random candies from the candies vector to store in the candystore
{
    vector<Candy> shuffledCandies = allCandies;

    int indexCandy1;
    int indexCandy2;
    int indexCandy3;

    indexCandy1 = rand() % shuffledCandies.size(); // sets the first index for a candy in the vector
    do                                             // gets the other 2 indexes
    {
        indexCandy2 = rand() % shuffledCandies.size();
        indexCandy3 = rand() % shuffledCandies.size();
    } while (indexCandy2 == indexCandy1 || indexCandy3 == indexCandy1 || indexCandy3 == indexCandy2); // keep on getting new ones while any are equal

    // set the candies of the candystore to the candies in the randomized indexes
    _candies[0] = shuffledCandies[indexCandy1];
    _candies[1] = shuffledCandies[indexCandy2];
    _candies[2] = shuffledCandies[indexCandy3];
}

void Candystore::displayCandy() // displays the candy store's name and its candies.
{

    cout << "Name: " << getStoreName() << endl;
    cout << "Candies For Sale:" << endl;
    pickRandomCandies(readCandiesFromFile());
    for (int i = 0; i < 3; i++)
    {
        cout << _candies[i].name << "|Description: " << _candies[i].description << "|Effect type and Value: " << _candies[i].effect_type << " " << _candies[i].effect_value << "|Candy type: " << _candies[i].candy_type << "|Price: " << _candies[i].price << endl;
    }
}

Candy Candystore::findCandy(string candy_name) // finds candy in the candystore for when players want to buy a certain candy.
{
    string candy_lower = "";
    for (int i = 0; i < (int)candy_name.length(); i++)
    {
        char lower = tolower(candy_name[i]);
        candy_lower = candy_lower + lower;
    }

    Candy temp;

    for (int i = 0; i < 3; i++)
    {
        string currentCandyName = _candies[i].name;
        string current_candy_lower = "";
        for (int j = 0; j < (int)currentCandyName.length(); j++)
        {
            current_candy_lower += tolower(currentCandyName[j]);
        }

        if (current_candy_lower == candy_lower)
        {
            temp = _candies[i];
            return temp;
        }
    }

    return temp;
}