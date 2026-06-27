#include <stdio.h>
#include <unistd.h>  // For sleep() on Linux/Mac
// For Windows, use #include <windows.h> and Sleep()

void countdown(int minutes) {
    int total_seconds = minutes * 60;
    while (total_seconds > 0) {
        int mins = total_seconds / 60;
        int secs = total_seconds % 60;
        printf("\rTime remaining: %02d:%02d", mins, secs);
        fflush(stdout);
        sleep(1);  // Windows: Sleep(1000);
        total_seconds--;
    }
    printf("\n⏰ TIME'S UP! ⏰\n");
}

int main() {
    int choice;
    do {
        printf("\n=== POMODORO TIMER ===\n");
        printf("1. Work (25 min)\n");
        printf("2. Short Break (5 min)\n");
        printf("3. Long Break (15 min)\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: printf("Starting work session...\n"); countdown(25); break;
            case 2: printf("Taking short break...\n"); countdown(5); break;
            case 3: printf("Taking long break...\n"); countdown(15); break;
            case 4: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(choice != 4);
    return 0;
}