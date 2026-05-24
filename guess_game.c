#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Function to validate if input is a number
int isValidNumber(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isdigit(input[i]) && input[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

// Function to give hints based on how close the guess is
void giveHint(int guess, int secret) {
    int difference = abs(secret - guess);
    
    if(difference <= 5) {
        printf("🔥 You're extremely close! ");
    } else if(difference <= 10) {
        printf("💡 You're very close! ");
    } else if(difference <= 20) {
        printf("📊 You're getting warm! ");
    } else if(difference <= 30) {
        printf("🌡️ You're lukewarm... ");
    } else if(difference <= 50) {
        printf("❄️ You're cold. ");
    } else {
        printf("🥶 You're freezing cold! ");
    }
    
    printf("(Difference: %d)\n", difference);
}

int main() {
    int secretNumber, guess, attempts = 0, score = 0;
    int maxAttempts = 10;
    int playAgain = 1;
    int bestScore = 0;
    char input[10];
    
    // Seed the random number generator
    srand(time(0));
    
    printf("============================================\n");
    printf("      Welcome to the Number Guessing Game!\n");
    printf("============================================\n");
    printf("🎮 Game Features:\n");
    printf("- Guess a number between 1 and 100\n");
    printf("- You have %d attempts\n", maxAttempts);
    printf("- Get proximity hints\n");
    printf("- Score based on remaining attempts\n");
    printf("- Best score tracking\n\n");
    
    while(playAgain) {
        // Generate new random number for each game
        secretNumber = rand() % 100 + 1;
        attempts = 0;
        
        printf("🎯 New Game Started!\n");
        printf("I have chosen a number between 1 and 100.\n");
        printf("You have %d attempts to guess it.\n\n", maxAttempts);
        
        // Game loop
        while (attempts < maxAttempts) {
            printf("Attempt %d/%d - Enter your guess (or 0 to quit): ", attempts + 1, maxAttempts);
            
            // Get input as string to validate
            fgets(input, sizeof(input), stdin);
            
            // Check if user wants to quit
            if(input[0] == '0' && (input[1] == '\n' || input[1] == '\0')) {
                printf("\n👋 Thanks for playing! See you next time!\n");
                return 0;
            }
            
            // Remove newline character
            input[strcspn(input, "\n")] = 0;
            
            // Validate input
            if(!isValidNumber(input)) {
                printf("❌ Invalid input! Please enter a number between 1 and 100.\n\n");
                continue;
            }
            
            guess = atoi(input);
            
            // Validate range
            if(guess < 1 || guess > 100) {
                printf("❌ Please enter a number between 1 and 100!\n\n");
                continue;
            }
            
            attempts++;
            
            if (guess == secretNumber) {
                // Calculate score (10 points per remaining attempt)
                score = (maxAttempts - attempts + 1) * 10;
                printf("\n🎉🎉🎉 CONGRATULATIONS! 🎉🎉🎉\n");
                printf("You guessed it in %d attempts!\n", attempts);
                printf("The secret number was %d.\n", secretNumber);
                printf("🏆 Your score: %d points\n", score);
                
                // Update best score
                if(score > bestScore) {
                    bestScore = score;
                    printf("🌟 NEW BEST SCORE! 🌟\n");
                }
                printf("Best score so far: %d points\n", bestScore);
                break;
            } else {
                // Provide feedback with hints
                if(guess < secretNumber) {
                    printf("📉 Too low! ");
                } else {
                    printf("📈 Too high! ");
                }
                giveHint(guess, secretNumber);
                
                // Additional tip for remaining attempts
                printf("Remaining attempts: %d\n\n", maxAttempts - attempts);
                
                // Special encouragement when attempts are running out
                if(maxAttempts - attempts <= 3 && maxAttempts - attempts > 0) {
                    printf("⚠️ WARNING: Only %d attempts left! ⚠️\n\n", maxAttempts - attempts);
                }
            }
            
            if (attempts == maxAttempts) {
                printf("\n❌❌❌ GAME OVER! ❌❌❌\n");
                printf("You've used all %d attempts.\n", maxAttempts);
                printf("The secret number was %d.\n", secretNumber);
                printf("Score: 0 points\n\n");
            }
        }
        
        // Ask to play again
        char playChoice;
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playChoice);
        getchar(); // Clear input buffer
        
        if(tolower(playChoice) != 'y') {
            playAgain = 0;
            printf("\n============================================\n");
            printf("              GAME STATISTICS\n");
            printf("============================================\n");
            printf("🏆 Your best score: %d points\n", bestScore);
            printf("📊 Thanks for playing!\n");
            printf("============================================\n");
        } else {
            printf("\n🔄 Starting a new round...\n\n");
        }
    }
    
    return 0;
}