#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
    char sample[] = "The quick brown fox jumps over the lazy dog";
    char input[200];
    time_t start, end;
    double timeTaken;
    int words, characters = 0;
    
    printf("⌨️  Typing Speed Test\n");
    printf("Type the following text:\n\n");
    printf("📝 %s\n\n", sample);
    printf("Press Enter when ready...");
    getchar();
    
    time(&start);
    printf("▶️  Start typing: ");
    fgets(input, sizeof(input), stdin);
    time(&end);
    
    // Remove newline
    input[strcspn(input, "\n")] = 0;
    
    // Count characters typed correctly
    for(int i = 0; i < strlen(sample) && i < strlen(input); i++) {
        if(sample[i] == input[i])
            characters++;
    }
    
    timeTaken = difftime(end, start);
    int totalWords = strlen(sample) / 5; // Standard WPM calculation
    int wpm = (characters / 5) / (timeTaken / 60);
    
    printf("\n📊 Results:\n");
    printf("Time: %.1f seconds\n", timeTaken);
    printf("Characters correct: %d/%zu\n", characters, strlen(sample));
    printf("Accuracy: %.1f%%\n", (characters * 100.0) / strlen(sample));
    printf("Speed: %d WPM\n", wpm);
    
    // Rating system
    if(wpm >= 60) printf("🏆 Excellent typist!\n");
    else if(wpm >= 40) printf("👍 Good job!\n");
    else if(wpm >= 20) printf("📖 Keep practicing!\n");
    else printf("💪 Practice more!\n");
    
    return 0;
}