#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "player.h"

using namespace std;

class Game
{
private:
    vector<Candy> _candies;
    vector<Character> _characters;
    Player p1;
    Player p2;

public:
    Game(vector<Candy>, vector<Character>);
    void loadCandies(string filename);
    void loadCharacters(string filename);
    void setCandies(vector<Candy> candy);
    vector<Candy> getCandies() const;
    void setCharacters(vector<Character> characters);
    vector<Character> getCharacters() const;
    Character pickCharacter();
};

#endif