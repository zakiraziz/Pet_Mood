#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Expanded word categories
const char *words_programming[] = {"programming", "computer", "keyboard", "monitor", "algorithm", 
                                   "function", "variable", "pointer", "recursion", "iteration"};
const char *words_animals[] = {"elephant", "giraffe", "dolphin", "penguin", "kangaroo", 
                               "octopus", "butterfly", "crocodile", "chimpanzee", "alligator"};
const char *words_countries[] = {"australia", "brazil", "canada", "denmark", "egypt", 
                                 "france", "germany", "hungary", "iceland", "jamaica"};
const char *words_food[] = {"pizza", "burger", "sushi", "pasta", "taco", 
                            "sandwich", "chocolate", "vanilla", "strawberry", "pineapple"};
const char *words_science[] = {"gravity", "molecule", "electron", "neutron", "proton",
                               "quantum", "telescope", "microscope", "photosynthesis", "evolution"};

#define NUM_WORDS_PER_CATEGORY 10
#define CATEGORIES 5
#define MAX_TRIES 7

typedef struct {
    const char **words;
    const char *name;
} Category;

Category categories[] = {
    {words_programming, "💻 Programming"},
    {words_animals, "🐾 Animals"},
    {words_countries, "🌍 Countries"},
    {words_food, "🍕 Food"},
    {words_science, "🔬 Science"}
};

const char *difficulty_levels[] = {"Easy", "Medium", "Hard"};

void draw_hangman(int tries, int max_tries) {
    const char *stages[] = {
        "\n   +---+\n       |\n       |\n       |\n      ===",
        "\n   +---+\n   O   |\n       |\n       |\n      ===",
        "\n   +---+\n   O   |\n   |   |\n       |\n      ===",
        "\n   +---+\n   O   |\n  /|   |\n       |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n       |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n  /    |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n  / \\  |\n      ===",
        "\n   +---+\n   O   |\n  /|\\  |\n  / \\  |\n      ==="
    };
    
    // Color code based on remaining tries
    if (tries <= max_tries * 0.3) {
        printf("\033[31m"); // Red - danger
    } else if (tries <= max_tries * 0.6) {
        printf("\033[33m"); // Yellow - warning
    } else {
        printf("\033[32m"); // Green - safe
    }
    
    printf("%s\n", stages[tries]);
    printf("\033[0m"); // Reset color
}

void display_hint(const char *word, const char *guessed_letters) {
    printf("\n💡 Hint: ");
    int word_len = strlen(word);
    int revealed = 0;
    
    // Try to reveal a random unrevealed letter as hint
    for (int i = 0; i < word_len; i++) {
        if (strchr(guessed_letters, word[i]) != NULL) {
            revealed++;
        }
    }
    
    if (revealed < word_len - 1) {
        // Find a letter to reveal
        for (int i = 0; i < 26; i++) {
            char letter = 'a' + i;
            if (strchr(word, letter) != NULL && strchr(guessed_letters, letter) == NULL) {
                printf("The word contains the letter '%c'\n", letter);
                return;
            }
        }
    } else {
        printf("You've almost got it! Keep going!\n");
    }
}

void display_progress(const char *guessed, const char *word) {
    printf("\n📝 Word: ");
    int word_len = strlen(word);
    for (int i = 0; i < word_len; i++) {
        if (guessed[i] == '_') {
            printf("\033[1;33m_ \033[0m");
        } else {
            printf("\033[1;32m%c \033[0m", guessed[i]);
        }
    }
    printf("\n");
}

void show_leaderboard(int score) {
    static int high_score = 0;
    static char high_score_name[50] = "Player";
    
    if (score > high_score) {
        high_score = score;
        printf("\n🏆 NEW HIGH SCORE! 🏆\n");
        printf("Enter your name: ");
        scanf(" %s", high_score_name);
        printf("✅ Record saved!\n");
    }
    
    printf("\n📊 LEADERBOARD\n");
    printf("━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  🥇 %s: %d points\n", high_score_name, high_score);
    printf("  🥈 You: %d points\n", score);
    printf("━━━━━━━━━━━━━━━━━━━━━\n");
}

void print_ascii_art(const char *word, int won) {
    if (won) {
        printf("\n🎉🎊🎉🎊🎉🎊🎉🎊🎉🎊🎉🎊🎉🎊🎉\n");
        printf("     CONGRATULATIONS! YOU WON!      \n");
        printf("🎉🎊🎉🎊🎉🎊🎉🎊🎉🎊🎉🎊🎉🎊🎉\n");
        printf("\n       ╔═══════════════════╗\n");
        printf("       ║   ★  ★  ★  ★  ★   ║\n");
        printf("       ║    YOU DID IT!     ║\n");
        printf("       ║   ★  ★  ★  ★  ★   ║\n");
        printf("       ╚═══════════════════╝\n");
    } else {
        printf("\n😢💀😢💀😢💀😢💀😢💀😢💀😢💀😢\n");
        printf("        GAME OVER! YOU LOST!        \n");
        printf("😢💀😢💀😢💀😢💀😢💀😢💀😢💀😢\n");
        printf("\n       ╔═══════════════════╗\n");
        printf("       ║   💀  💀  💀  💀   ║\n");
        printf("       ║    BETTER LUCK     ║\n");
        printf("       ║   💀  💀  💀  💀   ║\n");
        printf("       ╚═══════════════════╝\n");
        printf("\nThe word was: \033[1;33m%s\033[0m\n", word);
    }
}

int get_difficulty() {
    int choice;
    printf("\n🎯 Select Difficulty:\n");
    printf("  1. Easy (7 tries)\n");
    printf("  2. Medium (5 tries)\n");
    printf("  3. Hard (3 tries)\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &choice);
    while (choice < 1 || choice > 3) {
        printf("Invalid choice! Enter 1, 2, or 3: ");
        scanf("%d", &choice);
    }
    return choice;
}

int select_category() {
    printf("\n📂 Select Category:\n");
    for (int i = 0; i < CATEGORIES; i++) {
        printf("  %d. %s\n", i + 1, categories[i].name);
    }
    printf("Enter choice (1-%d): ", CATEGORIES);
    int choice;
    scanf("%d", &choice);
    while (choice < 1 || choice > CATEGORIES) {
        printf("Invalid choice! Enter 1-%d: ", CATEGORIES);
        scanf("%d", &choice);
    }
    return choice - 1;
}

int play_again() {
    char response;
    printf("\n🔄 Play again? (y/n): ");
    scanf(" %c", &response);
    return tolower(response) == 'y';
}

int main() {
    srand(time(NULL));
    int total_score = 0;
    int games_played = 0;
    int games_won = 0;
    
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║         HANGMAN DELUXE v3.0                ║\n");
    printf("║         ✨ The Ultimate Word Game ✨        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    do {
        // Game setup
        int category_idx = select_category();
        int difficulty = get_difficulty();
        
        int max_tries;
        switch(difficulty) {
            case 1: max_tries = 7; break;
            case 2: max_tries = 5; break;
            case 3: max_tries = 3; break;
            default: max_tries = 7;
        }
        
        // Select random word from category
        const char **word_list = categories[category_idx].words;
        int index = rand() % NUM_WORDS_PER_CATEGORY;
        const char *word = word_list[index];
        int word_len = strlen(word);
        
        // Initialize game state
        char guessed[word_len + 1];
        for (int i = 0; i < word_len; i++) guessed[i] = '_';
        guessed[word_len] = '\0';
        
        char guessed_letters[27] = "";
        int tries = 0;
        int hints_used = 0;
        int score = 0;
        
        printf("\n🎮 Starting new game...\n");
        printf("Category: %s | Difficulty: %s | Word length: %d letters\n", 
               categories[category_idx].name, difficulty_levels[difficulty-1], word_len);
        printf("Tries: %d\n", max_tries);
        
        // Main game loop
        while (tries < max_tries) {
            draw_hangman(tries, max_tries);
            display_progress(guessed, word);
            
            // Show guessed letters
            printf("\n🔤 Guessed letters: ");
            if (strlen(guessed_letters) == 0) {
                printf("None");
            } else {
                for (int i = 0; i < strlen(guessed_letters); i++) {
                    printf("%c ", guessed_letters[i]);
                }
            }
            printf("\n💖 Tries remaining: %d/%d\n", max_tries - tries, max_tries);
            
            // Show hint option
            printf("\nOptions:\n");
            printf("  [letter] - Guess a letter\n");
            printf("  [h] - Get a hint (costs 1 try)\n");
            printf("  [q] - Quit game\n");
            printf("Your choice: ");
            
            char input[10];
            scanf("%s", input);
            char guess = tolower(input[0]);
            
            // Handle special commands
            if (guess == 'h') {
                if (hints_used < 2) {
                    hints_used++;
                    display_hint(word, guessed_letters);
                    tries++; // Hint costs a try
                    printf("Hint used! (remaining tries: %d)\n", max_tries - tries);
                    continue;
                } else {
                    printf("❌ You've used all hints!\n");
                    continue;
                }
            } else if (guess == 'q') {
                printf("\n👋 Thanks for playing!\n");
                return 0;
            } else if (!isalpha(guess)) {
                printf("❌ Please enter a letter!\n");
                continue;
            }
            
            // Check if letter already guessed
            if (strchr(guessed_letters, guess) != NULL) {
                printf("❌ Already guessed '%c'!\n", guess);
                continue;
            }
            
            // Add to guessed letters
            int len = strlen(guessed_letters);
            guessed_letters[len] = guess;
            guessed_letters[len + 1] = '\0';
            
            // Check if letter is in word
            int found = 0;
            for (int i = 0; i < word_len; i++) {
                if (word[i] == guess) {
                    guessed[i] = guess;
                    found = 1;
                }
            }
            
            if (!found) {
                tries++;
                printf("❌ Wrong! '%c' is not in the word.\n", guess);
                // Sound effect simulation
                printf("💥 *Dun dun dunnn!*\n");
            } else {
                printf("✅ Correct! '%c' is in the word!\n", guess);
                // Sound effect simulation
                printf("🎵 *Applause!*\n");
            }
            
            // Check win condition
            if (strcmp(guessed, word) == 0) {
                print_ascii_art(word, 1);
                score = max_tries * 10 - tries * 5 + word_len * 2;
                if (hints_used > 0) score -= hints_used * 3;
                printf("\n🏆 Score: %d points\n", score);
                total_score += score;
                games_played++;
                games_won++;
                show_leaderboard(total_score);
                break;
            }
        }
        
        // Check loss condition
        if (tries >= max_tries && strcmp(guessed, word) != 0) {
            print_ascii_art(word, 0);
            games_played++;
            show_leaderboard(total_score);
        }
        
        printf("\n📈 Game Statistics:\n");
        printf("  Games Played: %d\n", games_played);
        printf("  Games Won: %d (%.1f%%)\n", games_won, 
               games_played > 0 ? (float)games_won / games_played * 100 : 0);
        printf("  Total Score: %d\n", total_score);
        
    } while (play_again());
    
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║         THANKS FOR PLAYING!               ║\n");
    printf("║         Final Score: %-4d                 ║\n", total_score);
    printf("║         Games Won: %d/%d (%.1f%%)         ║\n", 
           games_won, games_played, 
           games_played > 0 ? (float)games_won / games_played * 100 : 0);
    printf("╚══════════════════════════════════════════════╝\n");
    
    return 0;
}