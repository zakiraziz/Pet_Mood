#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int number, guess, attempts = 0;
    
    // Seed random number generator
    srand(time(0));
    number = rand() % 100 + 1; // 1 to 100
    
    printf("\n==================================\n");
    printf("   🎯 NUMBER GUESSING GAME\n");
    printf("==================================\n");
    printf("I've picked a number between 1-100!\n");
    printf("Can you guess it? 🤔\n\n");
    
    do {
        printf("Enter your guess: ");
        scanf("%d", &guess);
        attempts++;
        
        if (guess < number) {
            printf("📈 Too LOW! Try again.\n\n");
        } else if (guess > number) {
            printf("📉 Too HIGH! Try again.\n\n");
        } else {
            printf("\n🎉 CONGRATULATIONS! You got it!\n");
            printf("✅ The number was %d\n", number);
            printf("📊 You took %d attempts.\n", attempts);
            
            if (attempts <= 3) {
                printf("🏆 AMAZING! You're a genius!\n");
            } else if (attempts <= 7) {
                printf("👏 Great job! You're pretty good!\n");
            } else {
                printf("💪 Not bad! Practice makes perfect!\n");
            }
        }
    } while (guess != number);
    
    return 0;
}