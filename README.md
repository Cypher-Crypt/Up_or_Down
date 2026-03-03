# Up_or_Down
A simple game of prediction and betting
A simple command-line betting game written in C++ where players predict the outcome of each round and wager virtual currency.

## Overview

**Up or Down** is a guessing game that tests your prediction skills and risk management. You start with a virtual account balance of $1,000 USD and place bets on whether the outcome of each round will be "Up" or "Down". The game continues until you run out of funds or choose to quit.

## Features

- **Player Account System**: Start with $1,000 USD virtual currency
- **Two Outcomes**: Predict whether the result will be "Up" or "Down"
- **Flexible Betting**: Place any bet up to your current balance
- **Optional Multipliers**: Enable high-risk, high-reward multiplier system
  - **Win with multipliers**: Winnings multiplied by 1.0x - 10.0x (Up Trend)
  - **Loss with multipliers**: Losses multiplied by 1.0x - 10.0x (Down Trend)
  - **Win without multipliers**: Earn your bet amount
  - **Loss without multipliers**: Lose your bet amount
- **Input Validation**: Robust error handling for all user inputs
- **Persistent Balance**: Your balance updates in real-time after each round

## Game Flow

1. Enter your username
2. Review your starting balance ($1,000 USD)
3. **Each round:**
   - Make a prediction (1 = Up, 2 = Down)
   - Choose whether to enable multipliers
   - Place your bet
   - View the outcome and updated balance
   - Play again or exit

4. **Game Over** when:
   - Your balance reaches $0 (bankruptcy)
   - You choose not to play again

## Building

### Debug Build
```bash
mkdir -p build/debug && g++ -g -O0 game.cpp -o build/debug/app
```

### Release Build
```bash
mkdir -p build/release && g++ -O2 -DNDEBUG game.cpp -o build/release/app
```

Alternatively, use the provided VS Code build tasks:
- **Build Debug**: Creates a debug executable with symbols and no optimizations
- **Build Release**: Creates an optimized release executable

## Running

After building, run the executable:

```bash
./build/debug/app    # Debug version
./build/release/app  # Release version
```

## Technical Details

### Architecture

- **Struct-based Data Storage**: Player information (name, balance, prediction, bet) stored in a single `playerInfo` struct
- **Modular Functions**:
  - `toLowerCase()`: Case-insensitive input handling
  - `outcomesIndex()`: Random outcome generation (0 = Up, 1 = Down)
  - `validatePrediction()`: Determines win/loss and applies multipliers
  - `validateBet()`: Validates bet amounts and ensures sufficient balance
  - `upTrend()`: Random multiplier between 1.0x and 10.0x for wins
  - `downTrend()`: Random multiplier between 1.0x and 10.0x for losses

### Key Implementation Details

- **Random Number Generation**: Uses C++11 `<random>` library with Mersenne Twister engine for deterministic seeding
- **Input Validation**: 
  - Handles invalid input types gracefully
  - Uses `unordered_set` for O(1) validation of yes/no responses
  - Provides specific error messages for different failure types
- **Floating-Point Precision**: Rounds all monetary values to 2 decimal places
- **Error Recovery**: `cin.clear()` and `cin.ignore()` properly flush invalid input from the buffer

### Dependencies

- C++11 or later
- Standard Library headers:
  - `<iostream>`: Input/output
  - `<string>`: String handling
  - `<random>`: Random number generation
  - `<vector>`: Dynamic arrays
  - `<cmath>`: Math functions (rounding)
  - `<limits>`: Stream management
  - `<unordered_set>`: Fast input validation
  - `<algorithm>`: String transformation
  - `<cctype>`: Character classification

## Example Gameplay

```
======= UP OR DOWN ======
Welcome to Up or Down!

Enter what you would like to be known as (username): Alice

Name: Alice
Balance: $1000.00 USD

Predict the outcome!
1. Up
2. Down

Enter your prediction (1 or 2): 1
⚠ Implement multipliers? (High risk, High reward) (yes/no): yes
Multipliers enabled! Up Trend on win, Down Trend on loss.

Place your bet (Available: $1000.00 USD): $100
The outcome is: up

Multiplier:  x5.25 (Up Trend)
Bet placed:  $100 USD
You won:     $525.00 USD
Congratulations, Alice!

Name:        Alice
Balance:     $1525.00 USD

Would you like to play again? (yes/no): no

Thanks for playing, Alice!
Final balance: $1525.00 USD. Goodbye!
```

## Notes

- The game uses virtual currency only—no real money is involved
- All outcomes are randomly generated; there is no betting strategy that guarantees wins
- Multipliers add excitement but also increase risk—use responsibly!
