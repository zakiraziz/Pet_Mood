#include <stdio.h>
#include <string.h>
#include <unistd.h>  // For sleep()

void speed_read(char *filename, int delay) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("File not found!\n");
        return;
    }
    
    char word[100];
    printf("\n=== SPEED READING ===\n");
    while (fscanf(f, "%s", word) == 1) {
        printf("%s ", word);
        fflush(stdout);
        sleep(delay);
    }
    printf("\n=== FINISHED ===\n");
    fclose(f);
}

int main() {
    char filename[100];
    int delay;
    
    printf("Enter text file name: ");
    scanf("%s", filename);
    printf("Enter delay between words (seconds, e.g., 1): ");
    scanf("%d", &delay);
    
    speed_read(filename, delay);
    return 0;
}