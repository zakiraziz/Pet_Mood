#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_ATTEMPTS 10
#define RANGE_MIN 1
#define RANGE_MAX 100

void print_welcome() {
    printf("\n====================================\n");
    printf("   WELCOME TO NUMBER GUESSING GAME   \n");
    printf("====================================\n");
    printf("I'm thinking of a number between %d and %d.\n", RANGE_MIN, RANGE_MAX);
    printf("You have %d attempts to guess it!\n", MAX_ATTEMPTS);
    printf("Type 'quit' to exit the game.\n\n");
}

void print_hint(int guess, int secret, int attempts_left) {
    int diff = abs(guess - secret);
    
    if (diff <= 3) {
        printf("🔥 You're extremely close! ");
    } else if (diff <= 10) {
        printf("💪 You're getting warm! ");
    } else if (diff <= 25) {
        printf("🌡️ You're lukewarm... ");
    } else {
        printf("❄️ You're cold. ");
    }
    
    if (guess < secret) {
        printf("Guess HIGHER");
    } else {
        printf("Guess LOWER");
    }
    
    printf(" (Attempts left: %d)\n", attempts_left);
}

void show_statistics(int attempts, int best_score) {
    printf("\n--- GAME STATISTICS ---\n");
    printf("Attempts used: %d\n", attempts);
    
    if (attempts <= 3) {
        printf("Rating: 🌟🌟🌟 AMAZING! You're a mind reader!\n");
    } else if (attempts <= 5) {
        printf("Rating: 🌟🌟 Great! Very impressive!\n");
    } else if (attempts <= 7) {
        printf("Rating: 🌟 Good job! Keep practicing!\n");
    } else {
        printf("Rating: 👍 Nice try! You'll do better next time!\n");
    }
    
    if (best_score == 0 || attempts < best_score) {
        printf("🎉 NEW HIGH SCORE! 🎉\n");
    }
}

void play_again_prompt() {
    char response;
    printf("\nPlay again? (y/n): ");
    scanf(" %c", &response);
    if (tolower(response) == 'y') {
        printf("\n\n");
        main(); // Recursive call for new game (use with caution)
        exit(0);
    } else {
        printf("Thanks for playing! Goodbye!\n");
        exit(0);
    }
}

int get_valid_input(int *guess) {
    char input[10];
    scanf("%s", input);
    
    if (strcmp(input, "quit") == 0 || strcmp(input, "q") == 0) {
        return -1; // Quit signal
    }
    
    if (!isdigit(input[0]) && input[0] != '-') {
        return 0; // Invalid input
    }
    
    *guess = atoi(input);
    
    if (*guess < RANGE_MIN || *guess > RANGE_MAX) {
        return 0; // Out of range
    }
    
    return 1; // Valid input
}

int main() {
    static int best_score = 0;
    int secret, guess, attempts = 0;
    char play_again = 1;
    
    srand(time(NULL));
    print_welcome();
    
    while (play_again) {
        secret = rand() % (RANGE_MAX - RANGE_MIN + 1) + RANGE_MIN;
        attempts = 0;
        int game_won = 0;
        
        printf("🎲 New game started! 🎲\n");
        printf("Enter your guess: ");
        
        while (attempts < MAX_ATTEMPTS && !game_won) {
            int input_result = get_valid_input(&guess);
            
            if (input_result == -1) { // Quit
                printf("\nYou quit the game. The number was %d.\n", secret);
                play_again = 0;
                break;
            } else if (input_result == 0) { // Invalid input
                printf("❌ Invalid input! Please enter a number between %d-%d or type 'quit': ", 
                       RANGE_MIN, RANGE_MAX);
                continue;
            }
            
            attempts++;
            
            if (guess == secret) {
                game_won = 1;
                printf("\n🎉 CORRECT! 🎉\n");
                printf("You guessed it in %d attempt%s!\n", attempts, (attempts == 1) ? "" : "s");
                show_statistics(attempts, best_score);
                if (best_score == 0 || attempts < best_score) {
                    best_score = attempts;
                }
                play_again_prompt();
            } else {
                if (attempts < MAX_ATTEMPTS) {
                    print_hint(guess, secret, MAX_ATTEMPTS - attempts);
                    printf("Your next guess: ");
                }
            }
        }
        
        if (!game_won && play_again) {
            printf("\n💀 GAME OVER! 💀\n");
            printf("You've used all %d attempts.\n", MAX_ATTEMPTS);
            printf("The secret number was: %d\n", secret);
            play_again_prompt();
        }
    }
    
    return 0;
}