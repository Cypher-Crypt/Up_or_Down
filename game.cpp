#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <cmath>
#include <limits>       // required for std::numeric_limits in cin.ignore()
#include <unordered_set> // for O(1) input validation on prediction and play again
#include <algorithm>    // for std::transform (case-insensitive input)
#include <cctype>       // for ::tolower

// ---------------------------------------------------------------
// STRUCT — holds all player-related data in one place
// ---------------------------------------------------------------
struct {
    std::string playerName;
    float accountBal;
    std::string playerPrediction;
    float lastBet;
} playerInfo;

std::vector<std::string> outcomes = {"up", "down"};
int prediction;
float currentBet;

// ---------------------------------------------------------------
// Forward declarations for upTrend() and downTrend()
// ---------------------------------------------------------------
float upTrend();
float downTrend();

// ---------------------------------------------------------------
// toLowerCase() converts any string to lowercase
// Takes by value intentionally — we want to modify and return a copy
// without touching the original string passed in
// ---------------------------------------------------------------
std::string toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// ---------------------------------------------------------------
// outcomesIndex() — randomly returns 0 or 1
// static engine ensures it is seeded only once across all calls
// ---------------------------------------------------------------
int outcomesIndex() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 1);
    return dist(gen);
}

// ---------------------------------------------------------------
// validatePrediction() checks if the player's prediction matches the outcome
// checks to prevent playing when balance is 0
// accepts a bool 'useMultiplier' passed in from main()
// If true  — WIN uses upTrend(), LOSS uses abs(downTrend())
// If false — WIN and LOSS both use flat bet amount only
// ---------------------------------------------------------------
void validatePrediction(bool useMultiplier) {
    std::string outcome = outcomes[outcomesIndex()];
    std::cout << "The outcome is: " << outcome << "\n";

    if ((prediction == 1 && outcome == "up") || (prediction == 2 && outcome == "down")) {
        float finalWin;

        if (useMultiplier) {
            // Multiplier opted in — win is scaled by upTrend()
            float multiplier = upTrend();
            finalWin = std::round((playerInfo.lastBet * multiplier) * 100.0f) / 100.0f;
            std::cout << "\nMultiplier:  x" << multiplier << " (Up Trend)\n";
        } else {
            // Multiplier opted out — flat bet returned as winnings
            finalWin = playerInfo.lastBet;
        }

        playerInfo.accountBal += finalWin;
        playerInfo.accountBal = std::round(playerInfo.accountBal * 100.0f) / 100.0f;

        std::cout << "Bet placed:  $" << playerInfo.lastBet << " USD\n";
        std::cout << "You won:     $" << finalWin << " USD\n";
        std::cout << "Congratulations, " << playerInfo.playerName << "!\n";
    } else {
        float finalLoss;

        if (useMultiplier) {
            // Multiplier opted in — loss is scaled by abs(downTrend())
            // std::abs() used because downTrend() returns a negative float
            float multiplier = std::abs(downTrend());
            finalLoss = std::round((playerInfo.lastBet * multiplier) * 100.0f) / 100.0f;
            std::cout << "\nMultiplier:  x" << multiplier << " (Down Trend)\n";
        } else {
            // Multiplier opted out — only flat bet is deducted
            finalLoss = playerInfo.lastBet;
        }

        playerInfo.accountBal -= finalLoss;
        playerInfo.accountBal = std::round(playerInfo.accountBal * 100.0f) / 100.0f;

        std::cout << "Bet placed:  $" << playerInfo.lastBet << " USD\n";
        std::cout << "You lost:    $" << finalLoss << " USD\n";
        std::cout << "Better luck next time, " << playerInfo.playerName << "!\n";
    }

    std::cout << "\nName:        " << playerInfo.playerName << "\n";
    std::cout << "Balance:     $" << playerInfo.accountBal << " USD\n\n";
}

// ---------------------------------------------------------------
// validateBet() detailed error feedback
// distinguishes between a zero bet, negative bet, and
// a bet exceeding the balance 
// accepts and forwards 'useMultiplier' to validatePrediction()
// ---------------------------------------------------------------
bool validateBet(bool useMultiplier) {
    if (currentBet < 0.00f) {
        std::cout << "✗ Bet cannot be negative. Please enter a positive amount.\n\n";
        return false;
    } else if (currentBet == 0.00f) {
        std::cout << "✗ Bet cannot be zero. You must wager something!\n\n";
        return false;
    } else if (currentBet > playerInfo.accountBal) {
        std::cout << "✗ Bet of $" << currentBet
                  << " exceeds your balance of $" << playerInfo.accountBal
                  << ". Please bet within your balance.\n\n";
        return false;
    } else {
        playerInfo.lastBet = currentBet;
        validatePrediction(useMultiplier); // forward the flag down
        return true;
    }
}

// ---------------------------------------------------------------
// upTrend() and downTrend() acts as optional multipliers for
// both wins and losses — a high risk, high reward system
// ---------------------------------------------------------------
float upTrend() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(1.0f, 10.0f);
    float value = dist(gen);
    return std::round(value * 100.0f) / 100.0f;
}

float downTrend() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-10.0f, -1.0f);
    float value = dist(gen);
    return std::round(value * 100.0f) / 100.0f;
}

int main() {
    // ADDED: valid sets for prediction and play-again responses
    // unordered_set gives O(1) [time complexity] lookup — efficient even if expanded later
    std::unordered_set<std::string> validPlayAgain = {"yes", "no", "y", "n"};

    std::cout << "======= UP OR DOWN ======\n";
    std::cout << "Welcome to Up or Down!\n\n";
    std::cout << "Enter what you would like to be known as (username): ";

    // std::getline() fetches the player's name, captures full line
    std::getline(std::cin, playerInfo.playerName);

    // username cannot be empty — loop until a non-empty name is given
    while (playerInfo.playerName.empty()) {
        std::cout << "✗ Username cannot be empty. Please enter a username: ";
        std::getline(std::cin, playerInfo.playerName);
    }

    std::cout << "\nUp or Down is a guessing game.\n";
    std::cout << "You bet virtual currency (USD) on whether the outcome is going to be 'Up' or 'Down'\n";
    std::cout << "You'll be given a default account balance of $1000.00 USD. Spend wisely.\n\n";

    playerInfo.accountBal = 1000.00f;

    std::cout << "Name: " << playerInfo.playerName << "\n";
    std::cout << "Balance: $" << playerInfo.accountBal << " USD\n\n";

    // ---------------------------------------------------------------
    // OUTER LOOP — drives the entire game across multiple rounds
    // Only exits when the player chooses not to play again
    // or when their balance hits zero (bankruptcy)
    // ---------------------------------------------------------------
    while (true) {
s
        // bankruptcy check at the start of every round
        // If balance is 0 or below, the player cannot bet — game over
        if (playerInfo.accountBal <= 0.00f) {
            std::cout << "==============================\n";
            std::cout << "You've run out of funds! \n";
            std::cout << "Game over, " << playerInfo.playerName << ".\n";
            std::cout << "==============================\n";
            break; // exits the outer game loop entirely
        }

        std::cout << "Predict the outcome!\n";
        std::cout << "1. Up\n";
        std::cout << "2. Down\n\n";

        // -----------------------------------------------------------
        // INNER LOOP 1 — prediction input validation
        // validates that the input is strictly 1 or 2
        // -----------------------------------------------------------
        while (true) {
            std::cout << "Enter your prediction (1 or 2): ";
            std::cin >> prediction;

            if (std::cin.fail()) {
                // cin.fail() is true when input type doesn't match — e.g. user types "abc"
                std::cout << "✗ Invalid input! Please enter 1 or 2.\n\n";
                std::cin.clear();  // clears the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flushes bad input from buffer
                continue;
            }

            // range check — only 1 or 2 are valid predictions
            if (prediction != 1 && prediction != 2) {
                std::cout << "✗ Invalid choice! Please enter 1 for Up or 2 for Down.\n\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            // Valid prediction received — flush buffer and exit this loop
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }

        // -----------------------------------------------------------
        // INNER LOOP 2 — bet input validation
        // validateBet() returns false with specific
        // error messages for negative, zero, and over-balance bets
        // Loop keeps asking until a valid bet is placed
        // multiplier opt-in prompt — appears after a valid bet
        // is confirmed. Uses the existing validPlayAgain unordered_set
        // {"yes", "no", "y", "n"} so no new validation set needed
        // -----------------------------------------------------------
        bool useMultiplier = false;
        while (true) {
            std::cout << "⚠ Implement multipliers? (High risk, High reward) (yes/no): ";
            std::string multiplierChoice;
            std::getline(std::cin, multiplierChoice);

            std::string lowered = toLowerCase(multiplierChoice);

            if (validPlayAgain.count(lowered) > 0) {
                useMultiplier = (lowered == "yes" || lowered == "y");
                if (useMultiplier) {
                    std::cout << "Multipliers enabled! Up Trend on win, Down Trend on loss.\n\n";
                } else {
                    std::cout << "Multipliers disabled. Flat bet applies.\n\n";
                }
                break;
            } else {
                std::cout << "✗ Please enter yes or no.\n\n";
            }
        }

        // Pass useMultiplier into the bet validation loop
        while (true) {
            std::cout << "Place your bet (Available: $" << playerInfo.accountBal << " USD): $";
            std::cin >> currentBet;

            if (std::cin.fail()) {
                std::cout << "✗ Invalid input! Please enter a valid numeric bet.\n\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (validateBet(useMultiplier)) {
                break;
            }
        }

        // -----------------------------------------------------------
        // play again prompt with full input validation
        // Accepts yes/no/y/n (case-insensitive)
        // Loops until a valid response is given
        // -----------------------------------------------------------
        std::string playAgain;
        while (true) {
            std::cout << "Would you like to play again? (yes/no): ";
            std::getline(std::cin, playAgain);

            std::string lowered = toLowerCase(playAgain);

            if (validPlayAgain.count(lowered) > 0) {
                // Valid response received
                if (lowered == "no" || lowered == "n") {
                    std::cout << "\nThanks for playing, " << playerInfo.playerName << "!\n";
                    std::cout << "Final balance: $" << playerInfo.accountBal << " USD. Goodbye!\n" << endl;
                    return 0; // cleanly exits main() and ends the program
                }
                break; // "yes" or "y" — break out of play again loop, outer loop repeats
            } else {
                std::cout << "✗ Please enter yes or no.\n\n";
            }
        }

        // Outer loop repeats here for a new round — fresh prediction and bet
    }

    return 0;
}