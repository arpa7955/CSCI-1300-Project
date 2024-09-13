#ifndef CANDYSTORE_H
#define CANDYSTORE_H

#include <iostream>
#include <vector>
#include "player.h"
#define RESET "\033[0m"

using namespace std;

class Candystore
{
private:
    const static int _MAX_CANDY = 3;
    Candy _candies[_MAX_CANDY];
    string _store_name;

public:
    Candystore();
    Candystore(string store_name);
    void setStoreName(string name);
    string getStoreName() const;
    vector<Candy> readCandiesFromFile();
    void pickRandomCandies(const vector<Candy> allCandies);

    Candy findCandy(string name);

    void displayCandy();
};

#endif