#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

const char *words[] = {"programming", "computer", "keyboard", "monitor", "algorithm", "function", "variable"};
#define NUM_WORDS 7
#define MAX_TRIES 6

void draw_hangman(int tries) {
    const char *stages[] = {
        "\n   +---+\n       |\n       |\n       |\n      ===",
        "\n   +---+\n   O   |\n       |\n       |\n      ===",
        "\n   +---+\n   O   |\n   |   |\n       |\n      ===",
        "\n   +---+\n   O   |\n  /|   |\n       |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n       |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n  /    |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n  / \\  |\n      ==="
    };
    printf("%s\n", stages[tries]);
}

int main() {
    srand(time(NULL));
    int index = rand() % NUM_WORDS;
    const char *word = words[index];
    int word_len = strlen(word);
    
    char guessed[word_len + 1];
    for (int i = 0; i < word_len; i++) guessed[i] = '_';
    guessed[word_len] = '\0';
    
    int tries = 0;
    int guessed_chars[26] = {0};
    
    printf("=== HANGMAN ===\n");
    
    while (tries < MAX_TRIES) {
        draw_hangman(tries);
        printf("\nWord: %s\n", guessed);
        printf("Guessed letters: ");
        for (int i = 0; i < 26; i++) {
            if (guessed_chars[i]) printf("%c ", 'a' + i);
        }
        printf("\nTries left: %d\n", MAX_TRIES - tries);
        
        char guess;
        printf("Guess a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);
        
        if (guessed_chars[guess - 'a']) {
            printf("Already guessed that letter!\n");
            continue;
        }
        guessed_chars[guess - 'a'] = 1;
        
        int found = 0;
        for (int i = 0; i < word_len; i++) {
            if (word[i] == guess) {
                guessed[i] = guess;
                found = 1;
            }
        }
        
        if (!found) {
            tries++;
            printf("Wrong guess! '%c' is not in the word.\n", guess);
        }
        
        if (strcmp(guessed, word) == 0) {
            printf("\nCongratulations! You guessed the word: %s\n", word);
            return 0;
        }
    }
    
    draw_hangman(tries);
    printf("\nGame Over! The word was: %s\n", word);
    return 0;
}