#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define CLEAR "clear"
#endif

volatile int running = 1;
time_t start_time, lap_start;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        running = 0;
        printf("\nStopwatch stopped!\n");
    }
}

void display_time(time_t seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    printf("\r%02d:%02d:%02d", hours, minutes, secs);
    fflush(stdout);
}

int main() {
    signal(SIGINT, handle_signal);
    
    printf("=== COMMAND LINE STOPWATCH ===\n");
    printf("Press Enter to start, then:\n");
    printf("  Enter - Lap/Stop\n");
    printf("  Ctrl+C - Exit\n");
    getchar();
    
    start_time = time(NULL);
    lap_start = start_time;
    time_t last_display = start_time;
    
    while (running) {
        time_t current = time(NULL);
        
        // Update display every second
        if (current != last_display) {
            display_time(current - start_time);
            last_display = current;
        }
        
        // Check for Enter key (non-blocking - simplified)
        char c;
        if (scanf("%c", &c) == 1 && c == '\n') {
            time_t lap_time = current - lap_start;
            printf("\nLap time: ");
            display_time(lap_time);
            printf("\n");
            lap_start = current;
        }
    }
    
    time_t end_time = time(NULL);
    printf("\n\nFinal time: ");
    display_time(end_time - start_time);
    printf("\n");
    
    return 0;
}