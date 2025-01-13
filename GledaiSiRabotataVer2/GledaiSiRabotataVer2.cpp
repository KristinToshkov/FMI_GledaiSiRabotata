#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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

vector<string> draw_6_random_cards(vector<string>& deck) {
    vector<string> drawn_cards;
    random_shuffle(deck.begin(), deck.end());
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

void display_all_cards(const vector<string>& players_cards, const vector<string>& computers_cards) {
    cout << "\nYour cards: ";
    display_cards(players_cards);

    cout << "\nComputer's cards: ";
    display_cards(computers_cards);
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
            computers_cards.erase(remove(computers_cards.begin(), computers_cards.end(), card), computers_cards.end());
        }
    }
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
        cout << "Your turn ends. Computer's turn.\n";
        // Computer asks...
        playersTurn = false;
    }
}

// Function for handling the player's turn
void player_turn(vector<string>& deck, vector<string>& players_cards, vector<string>& computers_cards) {
    string player_ask = "";

    // Ask for a valid rank
    bool valid = ask_for_rank(players_cards, player_ask);

    // Process the ask and check if the computer has the cards
    bool hasCards = process_ask(player_ask, computers_cards, players_cards);

    if (hasCards) {
        cout << "\nComputer has cards of that rank!" << endl;
        //Players asks again...
    }
    else {
        cout << "\nComputer has no cards of that rank.";
        // Player draws a card from the deck
        draw_card(deck, players_cards, player_ask);
    }

    
}

void computer_turn(vector<string>& deck, vector<string>& players_cards, vector<string>& computers_cards)
{
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
        if (command == "Give") {
            for (const auto& card : players_cards) {
                if (card[0] == computer_ask) {
                    computers_cards.push_back(card);
                    players_cards.erase(remove(players_cards.begin(), players_cards.end(), card), players_cards.end());
                    cout << "Computer took " << card << " from your deck!";
                }
            }
        }
        else if (command == "Fish")
        {
            vector<string> drawn_card(deck.begin(), deck.begin() + 1);
            deck.erase(deck.begin(), deck.begin() + 1);
            computers_cards.push_back(drawn_card[0]);
            cout << "Computer has drawn " << drawn_card[0] << endl;

            if (drawn_card[0][0] == computer_ask) {
                cout << "\nComputer keeps his turn!";
                //Computer asks again...
            }
            else {
                cout << "\nComputer's turn ends. Player's turn.";
                // Player asks...
                playersTurn = true;
            }
        
    }

}

int main() {
    srand(time(0));
    vector<string> deck = generateDeck();
    vector<string> players_cards = draw_6_random_cards(deck);
    vector<string> computers_cards = draw_6_random_cards(deck);

    display_all_cards(players_cards, computers_cards);

    // Call the player's turn
    while (true) {
        while (playersTurn) {
            player_turn(deck, players_cards, computers_cards);
            display_all_cards(players_cards, computers_cards);
        }
        while (!playersTurn) {
            computer_turn(deck, players_cards, computers_cards);
            display_all_cards(players_cards, computers_cards);
        }
        if (players_cards.size() >= 52)
            cout << "Player wins!!!";
        else if (computers_cards.size() >= 52)
            cout << "Computer wins!!!";
        

        cout << "\nRemaining cards in deck: " << deck.size() << endl;
    }
    return 0;
}
