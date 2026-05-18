#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to give hints based on how close the guess is
void giveHint(int guess, int secret) {
    int difference = abs(secret - guess);
    
    if (difference == 0) {
        return;
    } else if (difference <= 3) {
        printf("🔥 You're boiling hot! ");
    } else if (difference <= 7) {
        printf("🥵 Very close! ");
    } else if (difference <= 15) {
        printf("👍 Getting warmer... ");
    } else if (difference <= 30) {
        printf("😐 You're lukewarm. ");
    } else {
        printf("❄️ You're ice cold! ");
    }
    
    // Additional directional hint
    if (guess < secret) {
        printf("Go higher! 🚀\n");
    } else {
        printf("Go lower! 📉\n");
    }
}

// Function to display previous guess history
void showHistory(int history[], int count) {
    if (count == 0) return;
    
    printf("\n📜 Your guess history: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", history[i]);
        if (i < count - 1) printf("→ ");
    }
    printf("\n");
}

int main() {
    int secretNumber, guess, attempts = 0;
    int maxAttempts = 10;
    int guessHistory[10]; // Store all guesses
    int playAgain = 1;
    int totalGames = 0, totalAttempts = 0, bestGame = 999;
    
    // Seed the random number generator
    srand(time(0));
    
    while (playAgain) {
        // Generate random number between 1 and 100
        secretNumber = rand() % 100 + 1;
        attempts = 0;
        
        printf("\n====================================\n");
        printf("   Welcome to the Number Guessing Game!\n");
        printf("====================================\n");
        printf("I have chosen a number between 1 and 100.\n");
        printf("You have %d attempts to guess it.\n\n", maxAttempts);
        
        // Game loop
        while (attempts < maxAttempts) {
            printf("Attempt %d/%d - Enter your guess: ", attempts + 1, maxAttempts);
            
            // Input validation
            if (scanf("%d", &guess) != 1) {
                printf("❌ Invalid input! Please enter a number between 1 and 100.\n");
                // Clear the input buffer
                while (getchar() != '\n');
                continue;
            }
            
            // Check if guess is within range
            if (guess < 1 || guess > 100) {
                printf("❌ Please guess a number between 1 and 100!\n\n");
                continue;
            }
            
            guessHistory[attempts] = guess;
            attempts++;
            
            if (guess == secretNumber) {
                printf("\n🎉 🎉 🎉 CONGRATULATIONS! 🎉 🎉 🎉\n");
                printf("You guessed it in %d %s!\n", attempts, attempts == 1 ? "attempt" : "attempts");
                printf("The secret number was %d.\n", secretNumber);
                
                // Update statistics
                totalGames++;
                totalAttempts += attempts;
                if (attempts < bestGame) {
                    bestGame = attempts;
                }
                break;
            } else {
                printf("❌ Wrong guess! ");
                if (guess < secretNumber) {
                    printf("Too low!\n");
                } else {
                    printf("Too high!\n");
                }
                
                // Give proximity hint
                giveHint(guess, secretNumber);
                
                // Show guess history every 3 attempts
                if (attempts % 3 == 0) {
                    showHistory(guessHistory, attempts);
                }
                
                // Show remaining attempts
                printf("💪 Attempts remaining: %d\n\n", maxAttempts - attempts);
            }
            
            if (attempts == maxAttempts) {
                printf("\n❌ ❌ ❌ GAME OVER! ❌ ❌ ❌\n");
                printf("You've used all %d attempts.\n", maxAttempts);
                printf("The secret number was %d.\n", secretNumber);
                printf("Better luck next time! 🍀\n");
                
                totalGames++;
                totalAttempts += attempts;
            }
        }
        
        // Show game statistics
        if (totalGames > 0) {
            printf("\n📊 STATISTICS 📊\n");
            printf("Games played: %d\n", totalGames);
            printf("Average attempts: %.1f\n", (float)totalAttempts / totalGames);
            if (bestGame != 999) {
                printf("Best game: %d attempts\n", bestGame);
            }
        }
        
        // Ask to play again
        printf("\n🎮 Would you like to play again? (1 for Yes / 0 for No): ");
        scanf("%d", &playAgain);
        
        while (playAgain != 0 && playAgain != 1) {
            printf("Please enter 1 (Yes) or 0 (No): ");
            scanf("%d", &playAgain);
        }
    }
    
    printf("\n✨ Thanks for playing! Come back soon! ✨\n");
    return 0;
}
