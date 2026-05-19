#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int secretNumber, guess, attempts = 0;
    int maxAttempts = 10;
    
    // Seed the random number generator
    srand(time(0));
    
    // Generate random number between 1 and 100
    secretNumber = rand() % 100 + 1;
    
    printf("====================================\n");
    printf("   Welcome to the Number Guessing Game!\n");
    printf("====================================\n");
    printf("I have chosen a number between 1 and 100.\n");
    printf("You have %d attempts to guess it.\n\n", maxAttempts);
    
    // Game loop
    while (attempts < maxAttempts) {
        printf("Attempt %d/%d - Enter your guess: ", attempts + 1, maxAttempts);
        scanf("%d", &guess);
        
        attempts++;
        
        if (guess == secretNumber) {
            printf("\n🎉 Congratulations! You guessed it in %d attempts! 🎉\n", attempts);
            printf("The secret number was %d.\n", secretNumber);
            break;
        } else if (guess < secretNumber) {
            printf("Too low! Try a higher number.\n\n");
        } else {
            printf("Too high! Try a lower number.\n\n");
        }
        
        if (attempts == maxAttempts) {
            printf("\n❌ Game Over! You've used all %d attempts.\n", maxAttempts);
            printf("The secret number was %d. Better luck next time!\n", secretNumber);
        }
    }
    
    return 0;
}