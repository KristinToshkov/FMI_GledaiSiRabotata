#include <iostream>
#include <vector>
#include <string>
//#include <algorithm>
#include <ctime>

using namespace std;
bool playersTurn = true;
vector<string> generateDeck() {
    vector<string> deck;
    vector<string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
    vector<string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };

    for (const auto& rank : ranks) {
        for (const auto& suit : suits) {
            deck.push_back(rank + " of " + suit);
        }
    }
    return deck;
}

// Manual swap function (since <algorithm> is not allowed)
void manual_swap(std::string& a, std::string& b) {
    std::string temp = a;
    a = b;
    b = temp;
}

// Custom Fisher–Yates shuffle without <algorithm>
void custom_shuffle(std::vector<std::string>& deck) {
    std::srand(static_cast<unsigned int>(std::time(0)));  // Seed random generator

    for (int i = deck.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);  // Random index between 0 and i
        manual_swap(deck[i], deck[j]);  // Swap manually
    }
}

vector<string> draw_6_random_cards(vector<string>& deck) {
    vector<string> drawn_cards;
    custom_shuffle(deck);
    for (int i = 0; i < 6; ++i) {
        drawn_cards.push_back(deck.back());
        deck.pop_back();
    }
    return drawn_cards;
}

void display_cards(const vector<string>& cards) {
    for (const auto& card : cards) {
        cout << card << ", ";
    }
    cout << endl;
}

void display_all_cards(const vector<string>& players_cards, const vector<string>& computers_cards, const vector<vector<string>>& players_booked, const vector<vector<string>>& computers_booked) {
    cout << "\nYour cards: ";
    display_cards(players_cards);

    cout << "\nComputer's cards: ";
    display_cards(computers_cards);

    cout << "\nYour booked cards:\n";
    for (const auto& coup : players_booked)
        display_cards(coup);
    cout << "\nComputers booked cards:\n";
    for (const auto& coup : computers_booked)
        display_cards(coup);


}

bool ask_for_rank(const vector<string>& players_cards, string& player_ask) {
    bool valid = false;
    while (!valid) {
        cout << "\nAsk for a rank (2-9, T, J, Q, K, A): ";
        cin >> player_ask;
        for (const auto& card : players_cards) {
            if (player_ask[0] == card[0]) {
                valid = true;
            }
        }
        if (!valid) {
            cout << "\nInvalid card! You must only ask for ranks you already have!" << endl;
        }
    }
    return valid;
}

bool process_ask(const string& player_ask, vector<string>& computers_cards, vector<string>& players_cards) {
    bool hasCards = false;
    for (const auto& card : computers_cards) {
        if (card[0] == player_ask[0]) {
            hasCards = true;
            players_cards.push_back(card);
        }
    }
    if(hasCards)
        // Remove all cards starting with
        computers_cards.erase(
            remove_if(computers_cards.begin(), computers_cards.end(), [player_ask](const string& card) {
                return card[0] == player_ask[0];  // Check first character
                }),
            computers_cards.end()
        );

    return hasCards;
}

void draw_card(vector<string>& deck, vector<string>& players_cards, const string& player_ask) {
    vector<string> drawn_card(deck.begin(), deck.begin() + 1);
    deck.erase(deck.begin(), deck.begin() + 1);
    players_cards.push_back(drawn_card[0]);
    cout << "\nYou've drawn " << drawn_card[0] << endl;

    if (drawn_card[0][0] == player_ask[0]) {
        cout << "You keep your turn!\n";
        //Player asks again...
    }
    else {
        cout << "\nYour turn ends. Computer's turn.\n";
        // Computer asks...
        playersTurn = false;
    }
}

void book_cards_player(vector<string>& players_cards, vector<vector<string>>& players_booked)
{
    char book = players_cards[players_cards.size() - 1][0];
    int count = 0;
    if(players_cards.size() > 3)
    for (const auto& card : players_cards) {
        if (card[0] == book)
            count++;
    }
    if (count >= 4) {
        string input;
        while (input != "Book") {
            cout << "You have 4 of " << book << "'s, type \"Book\" too book them:";
            cin >> input;
        }
        vector<string> temp_deck;
        for (const auto& card : players_cards) {
            if (card[0] == book) {
                temp_deck.push_back(card);
            }
        }
        players_booked.push_back(temp_deck);
        players_cards.erase(
            remove_if(players_cards.begin(), players_cards.end(), [book](const string& card) {
                return card[0] == book;  // Compare first character
                }),
            players_cards.end()
        );
        cout << "\nYou have booked " << book << "'s!";
    }

}

void book_cards_computer(vector<string>& computers_cards, vector<vector<string>>& computers_booked)
{
    char book = computers_cards[computers_cards.size() - 1][0];
    int count = 0;
    if (computers_cards.size() > 3)
        for (const auto& card : computers_cards) {
            if (card[0] == book)
                count++;
        }
    if (count >= 4) {
        vector<string> temp_deck;
        for (const auto& card : computers_cards) {
            if (card[0] == book) {
                temp_deck.push_back(card);
            }
        }
        computers_booked.push_back(temp_deck);
        computers_cards.erase(
            std::remove_if(computers_cards.begin(), computers_cards.end(), [book](const std::string& card) {
                return card[0] == book;  // Compare first character
                }),
            computers_cards.end()
        );
        cout << "\nComputer has booked " << book << "'s!\n";
    }

}

// Function for handling the player's turn in the first phase of the game
void player_turn_1(vector<string>& deck, vector<string>& players_cards, vector<string>& computers_cards, vector<vector<string>>& players_booked) {

    //If no cards in hand forced draw
    if (players_cards.size() < 1 && deck.size() > 0)
    {
        vector<string> drawn_card(deck.begin(), deck.begin() + 1);
        deck.erase(deck.begin(), deck.begin() + 1);
        players_cards.push_back(drawn_card[0]);
        cout << "\nYou have no cards and have drawn " << drawn_card[0] << endl;
    }

    string player_ask = "";
    // Ask for a valid rank
    bool valid = ask_for_rank(players_cards, player_ask);

    // Process the ask and check if the computer has the cards
    bool hasCards = process_ask(player_ask, computers_cards, players_cards);
    system("cls");
    if (hasCards) {
        cout << "\nComputer has cards of that rank!" << endl;
        book_cards_player(players_cards, players_booked);
        //Player asks again...
    }
    else {
        cout << "\nComputer has no cards of that rank.\n";
        // Player draws a card from the deck
        draw_card(deck, players_cards, player_ask);
        book_cards_player(players_cards, players_booked);
    }
}
// Computers turn in the first phase of the game
void computer_turn_1(vector<string>& deck, vector<string>& players_cards, vector<string>& computers_cards, vector<vector<string>>& computers_booked)
{
    //If no cards in hand forced draw
    if (computers_cards.size() < 1 && deck.size() > 0)
    {
        vector<string> drawn_card(deck.begin(), deck.begin() + 1);
        deck.erase(deck.begin(), deck.begin() + 1);
        computers_cards.push_back(drawn_card[0]);
        cout << "\nComputer has no cards and has drawn " << drawn_card[0] << endl;
    }
    int random_index = rand() % computers_cards.size();
    char computer_ask = computers_cards[random_index][0];
    cout << "\nComputer asks for " << computer_ask << 
        endl << "Type \"Give\" to give or \"Fish\" if you don't have any cards of that rank:";
    string command = "";
    cin >> command;
    while (command != "Give" && command != "Fish") {
        cout << "\nInvalid command!" << "\nType \"Give\" to give or \"Fish\" if you don't have any cards of that rank:";
        cin >> command;
    }
    system("cls");
        if (command == "Give") {
            
            for (const auto& card : players_cards) {
                if (card[0] == computer_ask) {
                    computers_cards.push_back(card);
                    cout << "Computer took " << card << " from your deck!\n";
                }
            }
            // Remove all cards starting with
            players_cards.erase(
                std::remove_if(players_cards.begin(), players_cards.end(), [computer_ask](const std::string& card) {
                    return card[0] == computer_ask;  // Check first character
                    }),
                players_cards.end()
            );
            book_cards_computer(computers_cards, computers_booked);
        }
        else if (command == "Fish")
        {
            
            vector<string> drawn_card(deck.begin(), deck.begin() + 1);
            deck.erase(deck.begin(), deck.begin() + 1);
            computers_cards.push_back(drawn_card[0]);
            cout << "Computer has drawn " << drawn_card[0] << endl;
            book_cards_computer(computers_cards, computers_booked);

            if (drawn_card[0][0] == computer_ask) {
                cout << "\nComputer keeps his turn!\n";
                //Computer asks again...
            }
            else {
                cout << "\nComputer's turn ends. Player's turn.\n";
                // Player asks...
                playersTurn = true;
            }
        
    }

}

void sort_cards(vector<string>& cards) {
    int n = cards.size();

    // Bubble Sort based on the first character
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (cards[j][0] > cards[j + 1][0]) {
                // Manual swap without <algorithm>
                string temp = cards[j];
                cards[j] = cards[j + 1];
                cards[j + 1] = temp;
            }
        }
    }
}


// Function for handling the players turn in the second phase of the game
void player_turn_2(vector<vector<string>>& players_booked, vector<vector<string>>& computers_booked) {
    vector<string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
    string player_ask = "";
    // Ask for a valid rank
    while (find(ranks.begin(), ranks.end(), player_ask) == ranks.end()) {
        cout << "\nAsk for a rank (2-9, T, J, Q, K, A): ";
        cin >> player_ask;
    }

    // Process the ask and check if the computer has the cards
    bool hasCards = false;
    for (const auto& card : computers_booked) {
        if (card[0][0] == player_ask[0]) {
            hasCards = true;
            players_booked.push_back(card);
        }
    }
    system("cls");
    if (hasCards)
        // Remove all cards starting with
        computers_booked.erase(
            remove_if(computers_booked.begin(), computers_booked.end(), [player_ask](const vector<string>& card) {
                // Assuming you want to check the first string in the vector (card[0])
                return !card.empty() && card[0][0] == player_ask[0];  // Check first character of the first string in each vector
                }),
            computers_booked.end()
        );

    if (hasCards) {
        cout << "\nComputer has cards of that rank!" << endl;
        //Players asks again...
    }
    else {
        cout << "\nComputer has no cards of that rank.";
        playersTurn = false;
    }
}


// Computers turn in the second phase of the game
void computer_turn_2(vector<vector<string>>& computers_booked, vector<vector<string>>& players_booked)
{
    vector<string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };

    // Seed the random number generator
    srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate a random index between 0 and ranks.size() - 1
    int random_index = rand() % ranks.size();


    char computer_ask = ranks[random_index][0];


    cout << "\nComputer asks for " << computer_ask <<
        endl << "Type \"Give\" to give or \"Fish\" if you don't have any cards of that rank:";
    string command = "";
    cin >> command;
    while (command != "Give" && command != "Fish") {
        cout << "\nInvalid command!" << "\nType \"Give\" to give or \"Fish\" if you don't have any cards of that rank:";
        cin >> command;
    }
    if (command == "Give") {
        system("cls");
        for (const auto& card : players_booked) {
            if (card[0][0] == computer_ask) {
                computers_booked.push_back(card);
            }
        }
            // Remove all cards starting with
            players_booked.erase(
                remove_if(players_booked.begin(), players_booked.end(), [computer_ask](const vector<string>& card) {
                    return card[0][0] == computer_ask;  // Check first character
                    }),
                players_booked.end()
            );
    }
    else if (command == "Fish")
    {
        system("cls");
        cout << "\nComputer's turn ends. Player's turn.";
            // Player asks...
            playersTurn = true;
        

    }

}



int main() {
    srand(time(0));
    vector<string> deck = generateDeck();
    vector<string> players_cards = draw_6_random_cards(deck);
    vector<string> computers_cards = draw_6_random_cards(deck);
    vector<vector<string>> players_booked = {};
    vector<vector<string>> computers_booked = {};

    display_all_cards(players_cards, computers_cards, players_booked, computers_booked);

    // Game loop
    // Phase 1
    while (deck.size() > 0 && players_cards.size() > 0 && computers_cards.size() > 0) {
        /*if (deck.size() + players_cards.size() == 4 && computers_cards.size() == 0)
        {
            for (const auto& card : deck)
                players_cards.push_back(card);
            book_cards_player(players_cards, players_booked);
            break;

        }
        else if (deck.size() + computers_cards.size() == 4 && players_cards.size() == 0) {
            for (const auto& card : deck)
                computers_cards.push_back(card);
            book_cards_computer(computers_cards, computers_booked);
            break;
        }*/



        while (playersTurn) {
            sort_cards(players_cards);
            player_turn_1(deck, players_cards, computers_cards, players_booked);
         //   system("cls");
            display_all_cards(players_cards, computers_cards, players_booked, computers_booked);
            if (deck.size() < 1 && players_cards.size() < 1)
                break;
        }
        cout << "\nRemaining cards in deck: " << deck.size() << endl;
        while (!playersTurn) {
            sort_cards(computers_cards);
            computer_turn_1(deck, players_cards, computers_cards, computers_booked);
         //   system("cls");
            display_all_cards(players_cards, computers_cards, players_booked, computers_booked);
            if (deck.size() < 1 && computers_cards.size() < 1)
                break;
        }
        cout << "\nRemaining cards in deck: " << deck.size() << endl;
    }
    system("cls");
    cout << "Phase 2 starts now!";
    display_all_cards(players_cards, computers_cards, players_booked, computers_booked);
    // Phase 2
    while (players_booked.size() < 13 && computers_booked.size() < 13) {
        while (playersTurn && computers_booked.size() > 0) {
            player_turn_2(players_booked, computers_booked);
        //    system("cls");
            display_all_cards(players_cards, computers_cards, players_booked, computers_booked);
        }
        while (!playersTurn && players_booked.size() > 0) {
            computer_turn_2(computers_booked, players_booked);
      //      system("cls");
            display_all_cards(players_cards, computers_cards, players_booked, computers_booked);
        }
    }



    if (players_booked.size() >= 13)
        cout << "Player wins!!!";
    else if (computers_booked.size() >= 13)
        cout << "Computer wins!!!";
    return 0;
}
