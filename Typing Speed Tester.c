#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

const char *sentences[] = {
    "The quick brown fox jumps over the lazy dog",
    "Programming in C is fun and challenging",
    "Practice makes perfect when learning to code",
    "Speed and accuracy both matter in typing tests",
    "Welcome to the typing speed tester program"
};
#define NUM_SENTENCES 5

void calculate_wpm(double seconds, int char_count) {
    int words = char_count / 5; // Average word length
    double wpm = (words / seconds) * 60;
    printf("\n--- RESULTS ---\n");
    printf("Time: %.2f seconds\n", seconds);
    printf("Characters: %d\n", char_count);
    printf("Words typed: %d\n", words);
    printf("Speed: %.0f WPM\n", wpm);
}

int main() {
    srand(time(NULL));
    int index = rand() % NUM_SENTENCES;
    const char *sentence = sentences[index];
    
    printf("=== TYPING SPEED TESTER ===\n");
    printf("Type the following sentence exactly:\n\n");
    printf("\"%s\"\n\n", sentence);
    printf("Press Enter to start...");
    getchar();
    
    time_t start = time(NULL);
    
    char input[200];
    printf("\nStart typing: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    
    time_t end = time(NULL);
    double elapsed = difftime(end, start);
    
    if (strcmp(input, sentence) == 0) {
        calculate_wpm(elapsed, strlen(input));
    } else {
        printf("\nWrong! You made mistakes.\n");
        calculate_wpm(elapsed, strlen(input));
        printf("\nCorrect sentence was:\n%s\n", sentence);
    }
    return 0;
}