#include "board.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

Board::Board()
{
    resetBoard();
}

void Board::resetBoard() // resets the board to have all the tiles display and have a regular tile type
{
    const int COLOR_COUNT = 3;
    const string COLORS[COLOR_COUNT] = {MAGENTA, GREEN, BLUE};
    Tile new_tile;
    string current_color;
    for (int i = 0; i < _BOARD_SIZE - 1; i++)
    {
        current_color = COLORS[i % COLOR_COUNT];
        new_tile = {current_color, "regular tile"};
        _tiles[i] = new_tile;
    }
    new_tile = {ORANGE, "regular tile"};
    _tiles[_BOARD_SIZE - 1] = new_tile;

    _candy_store_count = 0;
    for (int i = 0; i < _MAX_CANDY_STORE; i++)
    {
        _candy_store_position[i] = -1;
    }

    // sets the current positions to zero
    _player_position[0] = 0;
    _player_position[1] = 0;
}

void Board::displayTile(int position) // displays the tiles and the players at their positions
{
    if ((position < 0) || (position >= _BOARD_SIZE))
    {
        return;
    }
    Tile target = _tiles[position];
    cout << target.color << " ";
    bool has_player = false;
    for (int i = 0; i < 2 && !has_player; i++) // displays both positions of each player and shows 1 and 2. When they are at the same position and one isn't pushed back, they are on the same spot and only player 1 shows.
    {
        if (position == _player_position[i])
        {

            cout << i + 1;
            has_player = true;
        }
    }
    if (!has_player)
    {
        cout << " ";
    }

    cout << " " << RESET;
}

void Board::displayBoard() // displays the board and all its tiles
{
    // First horizontal segment
    for (int i = 0; i <= 23; i++)
    {
        displayTile(i);
    }
    cout << endl;
    // First vertical segment
    for (int i = 24; i <= 28; i++)
    {
        for (int j = 0; j < 23; j++)
        {
            cout << "   ";
        }
        displayTile(i);
        cout << endl;
    }
    // Second horizontal segment
    for (int i = 52; i > 28; i--)
    {
        displayTile(i);
    }
    cout << endl;
    // Second vertical segment
    for (int i = 53; i <= 57; i++)
    {
        displayTile(i);
        for (int j = 0; j < 23; j++)
        {
            cout << "   ";
        }
        cout << endl;
    }
    // Third horizontal segment
    for (int i = 58; i < _BOARD_SIZE; i++)
    {
        displayTile(i);
    }
    cout << ORANGE << "Castle" << RESET << endl;
}

string Board::getTileColor(int position) const // returns the color of the tile at a specified position
{
    if (position >= 0 && position < _BOARD_SIZE)
    {
        return _tiles[position].color;
    }

    return "";
}

int Board::getBoardSize() const
{
    return _BOARD_SIZE;
}

bool Board::addCandyStore() // adds 3 candystores randomly on each of the segments for positions 0-27 on a magenta tile, 28-54 on a green tile, and 55-83 on a blue tile
{

    vector<int> magentaPositions;
    vector<int> greenPositions;
    vector<int> bluePositions;

    for (int i = 0; i <= 82; i++)
    {
        if (i % 3 == 0)
        {
            if (i <= 27)
                magentaPositions.push_back(i);
        }
        else if ((i & 3) == 1)
        {
            if (i >= 28 && i <= 54)
                greenPositions.push_back(i);
        }
        else if (i % 3 == 2)
        {
            if (i >= 55)
                bluePositions.push_back(i);
        }
    }

    int position1 = magentaPositions.at(rand() % magentaPositions.size());
    int position2 = greenPositions.at(rand() % greenPositions.size());
    int position3 = bluePositions.at(rand() % bluePositions.size());

    _candy_store_position[0] = position1;
    _candy_store_position[1] = position2;
    _candy_store_position[2] = position3;

    _candystores[0].setStoreName("Sugar Rush");
    _candystores[1].setStoreName("Candy Craze");
    _candystores[2].setStoreName("Tempting Treats");

    _candy_store_count = 3;

    return true;
}

void Board::addHiddenTreasure() // adds hidden treasures at random positions one at each segment (0-27, 28=54, 55-83)
{
    int pos1, pos2, pos3;

    pos1 = rand() % 27;
    pos2 = rand() % 26 + 27;
    pos3 = rand() % 28 + 54;

    _hidden_treasure_position[0] = pos1;
    _hidden_treasure_position[1] = pos2;
    _hidden_treasure_position[2] = pos3;
}

bool Board::isPositionCandyStore(int board_position) // checks if position is a candystore
{
    for (int i = 0; i < _candy_store_count; i++)
    {
        if (_candy_store_position[i] == board_position)
        {
            return true;
        }
    }
    return false;
}

void Board::displayCandyOptions(int storeIndex) // displayes the candystore at a certain index in the candystores vector
{
    _candystores[storeIndex].displayCandy();
}

int Board::movePlayer(int tile_to_move_forward, Player &playerToMove, Player &playerToCheck, int playerNum)
{ // two seeds
  // first gets whether tile will be regular or special
  // if regular, just do normal set player position
  // is special, call special tiles function
  // do another seed that does 25 % chance for each type and will do certain case depending on the random num

    int new_player_position = tile_to_move_forward + _player_position[playerNum - 1];
    if (new_player_position < 0)
    {
        return -1;
    }
    if (new_player_position >= _BOARD_SIZE - 1) // if it is greater than the board size, then they get to the castle and the game ends
    {
        _player_position[playerNum - 1] = _BOARD_SIZE - 1;
        displayBoard();
        cout << "Player " << playerToMove.getName() << " has gotten to the Castle! They win!" << endl;
        cout << "-----------------------------------------------------------------------------------------" << endl;
        return -2;
    }

    if (isPositionCandyStore(new_player_position)) // if it is a candystore, ask if they want to enter and what they want to buy
    {
        int storeIndex = -1;
        for (int i = 0; i < 3; i++)
        {
            if (_candy_store_position[i] == new_player_position)
            {
                storeIndex = i;
            }
        }
        // displayCandyOptions(storeIndex);
        cout << "You have arrived at the " << _candystores[storeIndex].getStoreName() << " candy store!" << endl;
        char choice;

        cout << "Do you want to enter the candy store? (y/n)" << endl;
        cin >> choice;

        cin.ignore(10000, '\n');
        cin.clear();

        while (cin.fail() || (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N'))
        {
            cout << "Invalid input. Please try again." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }

        if (choice == 'y' || choice == 'Y')
        {
            _candystores[storeIndex].displayCandy();
            cout << "What candy do you want to buy? " << endl;
            string candyChosen;
            getline(cin, candyChosen);

            Candy candyBought;
            candyBought = _candystores[storeIndex].findCandy(candyChosen);
            playerToMove.addCandy(candyBought);
        }
    }

    if (new_player_position == playerToCheck.getPosition()) // checks if there is another player on the tile
    {
        playerToMove.setPosition(new_player_position);
        // if there is a player already on the tile, they can take from 5-30 coins randomly from the other player's gold stock and move it to theirs
        // if the player has a shield however, they can avoid the attack (no gold taken), and the first player on the tile moves back one tile
        cout << "There is a player already here. Since you stole their spot, they might steal some gold!!" << endl;
        if (!playerToMove.checkShield()) // if they don't have a shield, the other player takes their gold
        {
            int tax = rand() % 26 + 5;
            playerToMove.setGold(playerToMove.getGold() - tax);
            playerToCheck.setGold(playerToCheck.getGold() + tax);
            cout << "You lost " << tax << " coins. The other player moves back" << endl;
            playerToCheck.setPosition(playerToCheck.getPosition() - 1);

            if (playerNum == 1)
            {
                _player_position[playerNum] = playerToCheck.getPosition();
            }
            else if (playerNum == 2)
            {
                _player_position[playerNum - 2] = playerToCheck.getPosition();
            }
        }
        else // if they have a shield, they don't lose coins
        {
            cout << "You used your shield. No gold was lost. The other player moves back." << endl;
            playerToMove.setShield(false);
            playerToCheck.setPosition(playerToCheck.getPosition() - 1);
            if (playerNum == 1)
            {
                _player_position[1] = playerToCheck.getPosition();
            }
            else if (playerNum == 2)
            {
                _player_position[0] = playerToCheck.getPosition();
            }
        }
        _player_position[playerNum - 1] = new_player_position; // sets the new position in the array
        return 3;                                              // stops the move player code from continuing and having other special occurrances
    }
    _player_position[playerNum - 1] = new_player_position;

    int isRegularOrSpecial = rand() % 100 + 1;
    if (isRegularOrSpecial <= 25) // checks if the tile is regular or special, and if it is, then run special tiles and if it is equal to one, then that means they get to draw again
    {
        int draw = specialTiles(playerToMove, playerToCheck, new_player_position, playerNum);
        if (draw == 1)
        {
            return draw;
        }
    }
    else // if not a special tile, then check if it is a hidden treasure position and if it isn't, then check if there is a calamity.
    {
        playerToMove.setPosition(new_player_position);
        bool isHiddenTreasure = false;
        for (int i = 0; i < 3; i++)
        {
            if (new_player_position == _hidden_treasure_position[i])
            {
                handleHiddenTreasure(playerToMove);
                isHiddenTreasure = true;
            }
        }
        if (isHiddenTreasure == false)
        {
            int randomCalamity = rand() % 100 + 1;
            if (randomCalamity <= 40)
            {
                calamities(playerToMove);
            }
        }
    }

    return 0;
}

int Board::specialTiles(Player &p, Player &z, int new_player_position, int playerNum) // called in move player if it is 25% chance
{
    int typeOfTile = rand() % 5 + 1;
    if (typeOfTile == 1)
    { // Shortcut tile --> moves the player 4 tiles up
        p.setPosition(new_player_position);
        // Handle shortcut logic
        cout << "You landed on a speed boost shortcut tile, encroaching 4 tiles closer the castle." << endl;
        if (p.getPosition() + 4 >= _BOARD_SIZE)
        {
            // If less than 4 tiles from the castle, move to the castle
            p.setPosition(_BOARD_SIZE - 1);
            _player_position[playerNum - 1] = p.getPosition();
        }
        else
        {
            p.setPosition(p.getPosition() + 4);
            _player_position[playerNum - 1] = p.getPosition();
        }
        return 0;
    }
    else if (typeOfTile == 2)
    { 
        // Gumdrop forest --> takes the player 4 positions back and takes a random amount (5-10) of gold
        cout << "You get lost in the Gumdrop forest. You end up walking off 4 places back" << endl;
        if (new_player_position - 4 <= 0)
        {
            p.removeLastPos();
            _player_position[playerNum - 1] = 0;
        }
        else
        {
            // p.removeLastPos();
            p.setPosition(new_player_position - 4);
            _player_position[playerNum - 1] = p.getPosition();
        }

        int randomGold = rand() % 6+5;

        if (p.getGold() - randomGold <= 0)
        {
            p.setGold(0);
        }
        else
        {
            p.setGold(p.getGold() - randomGold);
        }
        cout << " and you dropped " << p.getGold() << " gold." << endl;
        return 0;
    }
    else if (typeOfTile == 3)
    { 
        // Ice Cream Tile --> gives the player another chance of draw card from their new position

        cout << "You landed on an ice cream stop. You get an extra card draw" << endl;
        p.setPosition(new_player_position);
        _player_position[playerNum - 1] = p.getPosition();
        return 1;
    }
    else if (typeOfTile == 4)
    { 
        // Gingerbread House --> takes the player to their original position at the start of the turn and remove an immunity candy
        cout << "You landed at the Gingerbread House. The witch kicks you back to your last position and stole one of your immunity candies!" << endl;
        p.setPosition(p.getPosition()); // the player's old position is added again since they stayed there'
        _player_position[playerNum - 1] = p.getPosition();

        bool candyFound = false;
        for (int i = 0; i < p.getCandyAmount(); i++)
        {
            if (p.getCandyAtIndex(i).candy_type == "immunity")
            {
                candyFound = p.removeCandy(p.getCandyAtIndex(i).name);
                if (candyFound)
                {
                    break;
                }
            }

            if (candyFound)
            {
                break;
            }
           
        }    
    }
    return 0;
}
void Board::handleHiddenTreasure(Player player)
{
    cout << "Unravel the enigma, claim the treasure: Solve the riddle, seize the pleasure!" << endl;
    bool riddleCorrect = riddles(player);

    if (riddleCorrect)
    {
        cout << "Bravo, brainiac! Your intellect reigns supreme. Behold your bounty, for you have cracked the code! ";
        int random_number = rand() % 100 + 1;
        if (random_number <= 30) // if within 30%, get a stamina boost
        {
            int randomStamina = rand() % 21 + 10;
            int currentStamina = player.getStamina();
            cout << "You got a stamina boost of " << randomStamina << " units!" << endl;
            if (currentStamina + randomStamina > 100)
            {
                player.setStamina(100);
            }
            else
            {
                player.setStamina(currentStamina + randomStamina);
            }
        }
        else if (random_number > 30 && random_number <= 40) // 10% get a random amount of gold from 20-40
        {
            int randomGold = rand() % 21 + 20;
            int currentGold = player.getGold();
            cout << "You received " << randomGold << " gold!" << endl;
            if (currentGold + randomGold > 100)
            {
                player.setGold(100);
            }
            else
            {
                player.setGold(currentGold + randomGold);
            }
        }
        else if (random_number > 40 && random_number <= 70) // 30 % to receive the shield
        {
            cout << "The Robber's Repel shield is now in your posession! Now you can't be robbed by another player." << endl;
            player.setShield(true);
        }
        else // 30% get candy
        {
            int randomNum = rand() % 100 + 1;
            if (randomNum <= 70) // 70% get a stamina boost candy
            {
                cout << "You got the Jellybean of Vigor!!! This candy increases your stamina by 50 units!" << endl;
                int currentStamina = player.getStamina();
                if (currentStamina + 50 > 200)
                {
                    player.setStamina(200);
                }
                else
                {
                    player.setStamina(currentStamina + 50);
                }
            }
            else // 30% try for another treasure
            {
                cout << "You got the Treasure Hunter's Truffle!!! This lets you try for another treasure!" << endl;
                handleHiddenTreasure(player);
            }
        }
    }
    else
    {
        cout << "You answered incorrectly... so you win nothing :/" << endl;
    }
    return;
}

int Board::drawCard(Player playerToMove) // draw card function that gets a random card
{
    Card card;
    int num = rand() % 6;
    if (num == 0)
    {
        card = cards[0];
    }
    else if (num == 1)
    {
        card = cards[1];
    }
    else if (num == 2)
    {
        card = cards[2];
    }
    else if (num >= 3 && num <= 5) // Double card section
    {
        cout << "You drew a double tile card. ";
        int num2 = rand() % 3 + 3; // 3 to 5
        if (num2 == 3)
        {
            card = cards[3];
        }
        else if (num2 == 4)
        {
            card = cards[4];
        }
        else if (num2 == 5)
        {
            card = cards[5];
        }
    }
    cout << "You drew " << card.name << endl;
    string current = getTileColor(playerToMove.getPosition());

    // checks the color of the current plaer position and that determines how much the player moves
    if (current == MAGENTA)
    {
        // cout << "\t" << playerToMove.getPosition() << " : Magenta" << endl;
        if (card.name == "Cotton Candy Magenta")
        {
            return 3;
        }
        else if (card.name == "Minty Green")
        {
            return 1;
        }
        else if (card.name == "Bubblegum Blue")
        {
            return 2;
        }
        else if (card.name == "Double Cotton Candy Magenta")
        {
            return 6;
        }
        else if (card.name == "Double Minty Green")
        {
            return 4;
        }
        else if (card.name == "Double Bubblegum Blue")
        {
            return 5;
        }
    }
    else if (current == GREEN)
    {
        // cout << "\t" << playerToMove.getPosition() << " : Green" << endl;
        if (card.name == "Cotton Candy Magenta")
        {
            return 2;
        }
        else if (card.name == "Minty Green")
        {
            return 3;
        }
        else if (card.name == "Bubblegum Blue")
        {
            return 1;
        }
        else if (card.name == "Double Cotton Candy Magenta")
        {
            return 5;
        }
        else if (card.name == "Double Minty Green")
        {
            return 6;
        }
        else if (card.name == "Double Bubblegum Blue")
        {
            return 4;
        }
    }
    else if (current == BLUE)
    {
        // cout << "\t" << playerToMove.getPosition() << " : Blue" << endl;
        if (card.name == "Cotton Candy Magenta")
        {
            return 1;
        }
        else if (card.name == "Minty Green")
        {
            return 2;
        }
        else if (card.name == "Bubblegum Blue")
        {
            return 3;
        }
        else if (card.name == "Double Cotton Candy Magenta")
        {
            return 4;
        }
        else if (card.name == "Double Minty Green")
        {
            return 5;
        }
        else if (card.name == "Double Bubblegum Blue")
        {
            return 6;
        }
    }

    return 0;
}

bool Board::playRockPaperScissors(Player &player) // rock paper scissors to get our of calamities consequence
{
    bool playerWon = false;
    bool computerWon = false;

    do
    {
        char playerChoice;
        cout << "Enter r, p, or s" << endl;
        cin >> playerChoice;

        while (cin.fail() || (playerChoice != 'r' && playerChoice != 'p' && playerChoice != 's'))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid selection!" << endl;
            cin >> playerChoice;
        }

        cin.ignore();
        // srand(time(0));
        int random_number = rand() % 3;
        char computerChoice;
        if (random_number == 0)
        {
            computerChoice = 'r';
        }
        else if (random_number == 1)
        {
            computerChoice = 'p';
        }
        else
        {
            computerChoice = 's';
        }

        if (playerChoice == computerChoice)
        {
            cout << "Tie! Play again" << endl;
        }
        else if ((playerChoice == 'r' && computerChoice == 'p') || (playerChoice == 'p' && computerChoice == 's') || (playerChoice == 's' && computerChoice == 'r'))
        {
            cout << "COMPUTER WON" << endl;
            computerWon = true;
        }
        else
        {
            cout << "PLAYER WON" << endl;
            playerWon = true;
        }
    } while (playerWon == false && computerWon == false);
    return playerWon;
}

bool Board::riddles(Player &p) // reads the riddles files, stores them in a vector, and outputs a random riddle for the player to answer. Returns a boolean for whether they answered correctly
{
    vector<string> riddles;
    vector<string> answers;

    ifstream is("riddles.txt");

    if (is.fail())
    {
        cout << "Failed to open file" << endl;
        return false;
    }

    string line;

    while (getline(is, line))
    {
        if (line.length() > 0)
        {
            stringstream ss(line);
            string token;
            for (int i = 0; i < 2; i++)
            {
                getline(ss, token, '|');
                if (i == 0)
                {
                    riddles.push_back(token);
                }
                else
                {
                    answers.push_back(token);
                }
            }
        }
    }

    int random = rand() % riddles.size();

    cout << riddles[random] << endl;
    string answer;
    cin >> answer;
    if (answer == answers[random])
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Board::calamities(Player &p) // 40 % chance of calamaties called if not a special tile or a hidden tile
{
    int random = rand() % 100 + 1;

    if (random <= 30) // automatically lose gold 1-10
    {
        cout << "Oh no! Candy Bandits have swiped your gold coins!";
        int randomGold = rand() % 10 + 1;
        cout << " You lost " << randomGold << " gold!" << endl;
        p.setGold(p.getGold() - randomGold);
    }
    else if (random > 30 && random <= 65) // get lost in the labyrinth and chance to not lose a turn if win RPS
    {
        cout << "Oh dear! You got lost in the lollipop labyrinth! Win Rock Paper Scissors... or bear the consequences!" << endl;
        if (!playRockPaperScissors(p))
        {
            cout << "OOF...you lose a turn." << endl;
            p.increaseSkipAmount();
            // cout << "Updated skip amount: " << p.getSkipAmount() << endl;
        }
    }
    else if (random > 65 && random <= 80) // get stuck in candy avalanche, chance to not lose stamina if win RPS
    {
        cout << "Watch out! A candy avalanche has struck! Play rock paper scissors" << endl;
        if (!playRockPaperScissors(p))
        {
            int randomStamina = rand() % 5 + 5;
            cout << "OOF..you lost! Your efforts to escape the avalanche takes " << randomStamina << " units of your stamina and you lose a turn!" << endl;
            p.setStamina(p.getStamina() - randomStamina);
            p.increaseSkipAmount();
            // cout << "Updated skip amount: " << p.getSkipAmount() << endl;
        }
    }
    else
    {
        cout << "Oops! You are stuck in a sticky taffy trap! You lose a turn." << endl;
        p.increaseSkipAmount();
        // cout << "Updated skip amount: " << p.getSkipAmount() << endl;
    }
}