#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "board.h"
#include "game.h"
#include <cassert>

using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "board.h"
#include "game.h"
#include <cassert>

using namespace std;

void displayPlayerStats(Player player);

int main()
{
    srand(time(0));
    vector<Candy> candies;
    vector<Character> character;
    Game game(candies, character);
    candies = game.getCandies();
    character = game.getCharacters();

    Board board;
    board.addCandyStore();
    board.addHiddenTreasure();

    Player player1;
    Player player2;
    bool playerWon = false;
    bool player1Turn = true;
    bool player2Turn = false;
    int menuChoice = 0;
    Candystore c;

    // get player names and characters
    cout << "Welcome to Candy Land!" << endl;
    string name1;
    cout << "Enter player 1's name:" << endl;
    getline(cin, name1);
    player1.setName(name1);
    Character char1 = game.pickCharacter();
    player1.setCharacter(char1);
    char storeChoice;
    cout << "Do you want to visit a candystore(y/n)?" << endl;
    cin >> storeChoice;
    cin.ignore(10000, '\n');
    cin.clear();

    while (cin.fail() || (storeChoice != 'y' && storeChoice != 'Y' && storeChoice != 'n' && storeChoice != 'N'))
    {
        cout << "Invalid input. Please try again." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }
    if (storeChoice == 'y' || storeChoice == 'Y')
    {
        c.displayCandy();
        cout << "What candy do you want to buy? " << endl;
        string candyChosen;
        getline(cin, candyChosen);

        Candy candyBought;
        candyBought = c.findCandy(candyChosen);
        player1.addCandy(candyBought);
    }

    string name2;
    cout << "Enter player 2's name:" << endl;
    getline(cin, name2);
    player2.setName(name2);
    Character char2 = game.pickCharacter();
    player2.setCharacter(char2);
    char storeChoice2;
    cout << "Do you want to visit a candystore(y/n)?" << endl;
    cin >> storeChoice2;
    cin.ignore(10000, '\n');
    cin.clear();

    while (cin.fail() || (storeChoice2 != 'y' && storeChoice2 != 'Y' && storeChoice2 != 'n' && storeChoice2 != 'N'))
    {
        cout << "Invalid input. Please try again." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }
    if (storeChoice == 'y' || storeChoice == 'Y')
    {
        c.displayCandy();
        cout << "What candy do you want to buy? " << endl;
        string candyChosen;
        getline(cin, candyChosen);

        Candy candyBought;
        candyBought = c.findCandy(candyChosen);
        player2.addCandy(candyBought);
    }

    cout << "Let's begin the game. Here is the board:" << endl;
    board.displayBoard();

    while (playerWon == false) // while no one has won
    {
        if (player1Turn == true) // if it is player 1's turn
        {
            // cout << endl << "CHECKING SKIP AMOUNT FOR PLAYER 1 = " << player1.getSkipAmount() << endl;
            if (player1.getSkipAmount() > 0) // if >0, then skip turn and decrease the amount
            {
                cout << "Player " << player1.getName() << "'s turn is skipped." << endl;
                player1.decreaseSkipAmount();
                player1Turn = false;
                player2Turn = true;
                continue;
            }
            else
            {
                cout << "It's " << player1.getName() << "'s turn" << endl;
                cout << "Please select a menu option:" << endl;
                cout << "1. Draw a card" << endl;
                cout << "2. Use candy" << endl;
                cout << "3. Show player stats" << endl;
                cin >> menuChoice;

                while (cin.fail() || menuChoice > 3 || menuChoice < 1)
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please make another selection" << endl;
                    cin >> menuChoice;
                }

                cin.ignore();

                string candyName;
                Candy temp;
                int numMove;
                int repeat;
                string candy_name_lower = "";

                switch (menuChoice) // after each decrease stamina
                {
                case 1:
                    numMove = board.drawCard(player1);
                    // cout << "Number of moves: " << numMove << endl;
                    repeat = board.movePlayer(numMove, player1, player2, 1);
                    if (repeat == -2)
                    {
                        playerWon = true;
                        player1Turn = false;
                        player2Turn = false;
                    }
                    // cout << "After moving, the current position of the player: " << player1.getPosition() << endl;
                    if (repeat == 1) // if draw in moveplayer is one, then that means they get antoher turn
                    {
                        numMove = board.drawCard(player1);
                        // cout << "Player needs to move by: " << numMove << endl;
                        repeat = board.movePlayer(numMove, player1, player2, 1);
                    }
                    board.displayBoard();
                    player2Turn = true;
                    player1Turn = false;
                    player1.setStamina(player1.getStamina() - 1);
                    break;
                case 2:
                    cout << "Here is a list of your candies:" << endl;
                    player1.printInventory();
                    cout << "Enter the candy you wish to use:" << endl;
                    getline(cin, candyName);

                    temp = player1.findCandy(candyName);
                    // cout << "CANDY FOUND: " << temp.name << endl;
                    while (temp.name == "")
                    {
                        candyName = "";
                        cout << "Invalid candy. Enter the name of a candy in your inventory." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                        getline(cin, candyName);
                    }

                    for (int i = 0; i < (int)candyName.length(); i++) // set candy name to lowercase
                    {
                        char lower = tolower(candyName[i]);
                        candy_name_lower = candy_name_lower + lower;
                    }

                    // check which candy it is and use it
                    if (candy_name_lower == "frosty fizz")
                    {
                        cout << "You use Frosty Fizz. Your stamina increases by 10." << endl;
                        player1.setStamina(player1.getStamina() + 10);
                    }
                    else if (candy_name_lower == "lucky licorice")
                    {
                        cout << "You try to poison the other player with Lucky Licorice.";
                        if (player2.findImmunityCandy("bubblegum blast"))
                        {
                            cout << " They have Bubblegum Blast that stops your attack." << endl;
                            player2.removeCandy("bubblegum blast");
                        }
                        else if (player2.findImmunityCandy("caramel comet"))
                        {
                            cout << " They have Caramel Comet that stops your attack." << endl;
                            player2.removeCandy("caramel comet");
                        }
                        else if (player2.findImmunityCandy("sparkling sapphire"))
                        {
                            cout << " They have Sparkling Sapphire that stops your attack." << endl;
                            player2.removeCandy("sparklling sapphire");
                        }
                        else
                        {
                            cout << " They lose 10 stamina." << endl;
                            player2.setStamina(player2.getStamina() - 10);
                        }
                    }
                    else if (candy_name_lower == "ruby rapture")
                    {
                        cout << "You use Ruby Rapture, which makes the other player lose 2 turns." << endl;
                        player2.increaseSkipAmount();
                        player2.increaseSkipAmount();
                    }
                    else if (candy_name_lower == "crimson crystal")
                    {
                        cout << "You use Crimson Crystal, which increases your stamina by 15." << endl;
                        player1.setStamina(player1.getStamina() + 15);
                    }
                    else if (candy_name_lower == "fearsome fudge")
                    {
                        cout << "You use Fearsome Fudge, which makes the other player lose a turn." << endl;
                        player2.increaseSkipAmount();
                    }
                    else if (candy_name_lower == "venomous vortex")
                    {
                        cout << "You try to poison the other player with Venomous Vortex";
                        if (player2.findImmunityCandy("bubblegum blast"))
                        {
                            cout << " They have Bubblegum Blast that stops your attack." << endl;
                            player2.removeCandy("bubblegum blast");
                        }
                        else if (player2.findImmunityCandy("caramel comet"))
                        {
                            cout << " They have Caramel Comet that stops your attack." << endl;
                            player2.removeCandy("caramel comet");
                        }
                        else if (player2.findImmunityCandy("sparkling sapphire"))
                        {
                            cout << " They have Sparkling Sapphire that stops your attack." << endl;
                            player2.removeCandy("sparklling sapphire");
                        }
                        else
                        {
                            cout << " They lose 15 stamina." << endl;
                            player2.setStamina(player2.getStamina() - 15);
                        }
                    }
                    else if (candy_name_lower == "mystic marshmallow")
                    {
                        cout << "You use Mystic Marshmallow, which increases your stamina by 20" << endl;
                        player1.setStamina(player1.getStamina() + 20);
                    }
                    else if (candy_name_lower == "toxic taffy")
                    {
                        cout << "You try to poison the other player with Toxic Taffy";
                        if (player2.findImmunityCandy("bubblegum blast"))
                        {
                            cout << " They have Bubblegum Blast that stops your attack." << endl;
                            player2.removeCandy("bubblegum blast");
                        }
                        else if (player2.findImmunityCandy("caramel comet"))
                        {
                            cout << " They have Caramel Comet that stops your attack." << endl;
                            player2.removeCandy("caramel comet");
                        }
                        else if (player2.findImmunityCandy("sparkling sapphire"))
                        {
                            cout << " They have Sparkling Sapphire that stops your attack." << endl;
                            player2.removeCandy("sparklling sapphire");
                        }
                        else
                        {
                            cout << " They lose 20 stamina." << endl;
                            player2.setStamina(player2.getStamina() - 20);
                        }
                    }

                    player1.removeCandy(candyName); // remove that candy
                    player2Turn = true;
                    player1Turn = false; // switch turns
                    player1.setStamina(player1.getStamina() - 1);
                    break;
                    // break;
                case 3:
                    displayPlayerStats(player1); // displays the stats
                    player1Turn = false;
                    player2Turn = true;
                    player1.setStamina(player1.getStamina() - 1);
                    break;
                }
            }
        }
        else if (player2Turn == true && !playerWon) // player 2's turn
        {
            // cout << endl << "CHECKING SKIP AMOUNT FOR PLAYER 2= " << player2.getSkipAmount() << endl;
            if (player2.getSkipAmount() > 0)
            {
                cout << "Player " << player2.getName() << "'s turn is skipped." << endl;
                player2.decreaseSkipAmount();
                player1Turn = true;
                player2Turn = false;
                continue;
            }
            else
            {
                cout << "It's " << player2.getName() << "'s turn" << endl;
                cout << "Please select a menu option:" << endl;
                cout << "1. Draw a card" << endl;
                cout << "2. Use candy" << endl;
                cout << "3. Show player stats" << endl;
                cin >> menuChoice;

                while (cin.fail() || menuChoice > 3 || menuChoice < 1)
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please make another selection" << endl;
                    cin >> menuChoice;
                }

                cin.ignore();

                string candyName;
                Candy temp;
                int numMove;
                int repeat;
                string candy_name_lower = "";

                switch (menuChoice)
                {
                case 1:
                    numMove = board.drawCard(player2);

                    repeat = 1;
                    repeat = board.movePlayer(numMove, player2, player1, 2);
                    if (repeat == -2)
                    {
                        playerWon = true;
                    }
                    if (repeat == 1)
                    {
                        numMove = board.drawCard(player2);
                        repeat = board.movePlayer(numMove, player2, player1, 2);
                    }
                    board.displayBoard();
                    player2.setStamina(player2.getStamina() - 1);
                    break;
                case 2:
                    cout << "Here is a list of your candies:" << endl;
                    player2.printInventory();
                    cout << "Enter the candy you wish to use:" << endl;
                    getline(cin, candyName);

                    temp = player2.findCandy(candyName);
                    while (temp.name == "")
                    {
                        candyName = "";
                        cout << "Invalid candy. Enter the name of a candy in your inventory." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                        getline(cin, candyName);
                    }

                    for (int i = 0; i < (int)candyName.length(); i++)
                    {
                        char lower = tolower(candyName[i]);
                        candy_name_lower = candy_name_lower + lower;
                    }

                    if (candy_name_lower == "frosty fizz")
                    {
                        cout << "You use Frosty Fizz. Your stamina increases by 10." << endl;
                        player2.setStamina(player2.getStamina() + 10);
                    }
                    else if (candy_name_lower == "lucky licorice")
                    {
                        cout << "You try to poison the other player with Lucky Licorice.";
                        if (player1.findImmunityCandy("bubblegum blast"))
                        {
                            cout << " They have Bubblegum Blast that stops your attack." << endl;
                            player1.removeCandy("bubblegum blast");
                        }
                        else if (player1.findImmunityCandy("caramel comet"))
                        {
                            cout << " They have Caramel Comet that stops your attack." << endl;
                            player1.removeCandy("caramel comet");
                        }
                        else if (player1.findImmunityCandy("sparkling sapphire"))
                        {
                            cout << " They have Sparkling Sapphire that stops your attack." << endl;
                            player1.removeCandy("sparklling sapphire");
                        }
                        else
                        {
                            cout << " They lose 10 stamina." << endl;
                            player1.setStamina(player1.getStamina() - 10);
                        }
                    }
                    else if (candy_name_lower == "ruby rapture")
                    {
                        cout << "You use Ruby Rapture, which makes the other player lose 2 turns." << endl;
                        player1.increaseSkipAmount();
                        player1.increaseSkipAmount();
                    }
                    else if (candy_name_lower == "crimson crystal")
                    {
                        cout << "You use Crimson Crystal, which increases your stamina by 15." << endl;
                        player2.setStamina(player2.getStamina() + 15);
                    }
                    else if (candy_name_lower == "fearsome fudge")
                    {
                        cout << "You use Fearsome Fudge, which makes the other player lose a turn." << endl;
                        player1.increaseSkipAmount();
                    }
                    else if (candy_name_lower == "venomous vortex")
                    {
                        cout << "You try to poison the other player with Venomous Vortex";
                        if (player1.findImmunityCandy("bubblegum blast"))
                        {
                            cout << " They have Bubblegum Blast that stops your attack." << endl;
                            player1.removeCandy("bubblegum blast");
                        }
                        else if (player1.findImmunityCandy("caramel comet"))
                        {
                            cout << " They have Caramel Comet that stops your attack." << endl;
                            player1.removeCandy("caramel comet");
                        }
                        else if (player1.findImmunityCandy("sparkling sapphire"))
                        {
                            cout << " They have Sparkling Sapphire that stops your attack." << endl;
                            player1.removeCandy("sparklling sapphire");
                        }
                        else
                        {
                            cout << " They lose 15 stamina." << endl;
                            player1.setStamina(player1.getStamina() - 15);
                        }
                    }
                    else if (candy_name_lower == "mystic marshmallow")
                    {
                        cout << "You use Mystic Marshmallow, which increases your stamina by 20" << endl;
                        player2.setStamina(player2.getStamina() + 20);
                    }
                    else if (candy_name_lower == "toxic taffy")
                    {
                        cout << "You try to poison the other player with Toxic Taffy";
                        if (player1.findImmunityCandy("bubblegum blast"))
                        {
                            cout << " They have Bubblegum Blast that stops your attack." << endl;
                            player1.removeCandy("bubblegum blast");
                        }
                        else if (player1.findImmunityCandy("caramel comet"))
                        {
                            cout << " They have Caramel Comet that stops your attack." << endl;
                            player1.removeCandy("caramel comet");
                        }
                        else if (player1.findImmunityCandy("sparkling sapphire"))
                        {
                            cout << " They have Sparkling Sapphire that stops your attack." << endl;
                            player1.removeCandy("sparklling sapphire");
                        }
                        else
                        {
                            cout << " They lose 20 stamina." << endl;
                            player1.setStamina(player1.getStamina() - 20);
                        }
                    }

                    player2.removeCandy(candyName);
                    player1Turn = true; // switch turns again
                    player2Turn = false;
                    player2.setStamina(player1.getStamina() - 1);
                    break;
                case 3:
                    displayPlayerStats(player2);
                    player2.setStamina(player2.getStamina() - 1);
                    break;
                }
                player1Turn = true;
                player2Turn = false;
            }
        }
    }
    // once there is a winner, the results file is written to with both player end stats
    ofstream os;
    os.open("results.txt");
    if (os.fail())
    {
        cout << "Something is wrong with opening the file" << endl;
    }
    else
    {
        os << "Here are the stats from the game!" << endl;
        os << "--------------------------- " << player1.getName() << "'s Stats ---------------------------" << endl;
        os << "Character: " << player1.getCharacter().character_name << endl;
        os << "Gold: " << player1.getGold() << endl;
        os << "Stamina: " << player1.getStamina() << endl;
        os << "Candies in Possession:" << endl;
        os << player1.getCandyNames() << endl;
        os << "--------------------------- " << player2.getName() << "'s Stats ---------------------------" << endl;
        os << "Character: " << player2.getCharacter().character_name << endl;
        os << "Gold: " << player2.getGold() << endl;
        os << "Stamina: " << player2.getStamina() << endl;
        os << "Candies in Possession:" << endl;
        os << player2.getCandyNames() << endl;
    }
    os.close();

    // read out the results
    ifstream is("results.txt");
    string line;
    while (getline(is, line))
    {
        cout << line << endl;
    }

    return 0;
}

void displayPlayerStats(Player player)
{
    cout << "Here are your stats:" << endl;
    cout << "Player name: " << player.getName() << endl;
    cout << "Character: " << player.getCharacter().character_name << endl;
    cout << "Stamina: " << player.getStamina() << endl;
    cout << "Gold: " << player.getGold() << endl;
    cout << "Candies:" << endl;
    player.printInventory();
}