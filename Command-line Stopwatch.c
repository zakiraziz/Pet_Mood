#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define CLEAR "cls"
    #define sleep_ms(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #define CLEAR "clear"
    #define sleep_ms(ms) usleep((ms) * 1000)
#endif

#define MAX_LAPS 100

volatile int running = 1;
time_t start_time, lap_start;
time_t lap_times[MAX_LAPS];
int lap_count = 0;
int paused = 0;
time_t pause_start = 0;
time_t total_pause_time = 0;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        running = 0;
        printf("\n\nStopwatch stopped!\n");
    }
}

void display_time(time_t seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    int tenths = (seconds % 1) * 10; // For demonstration
    printf("\r%02d:%02d:%02d", hours, minutes, secs);
    fflush(stdout);
}

void display_lap_times() {
    if (lap_count == 0) {
        printf("\nNo laps recorded yet.\n");
        return;
    }
    
    printf("\n=== LAP TIMES ===\n");
    printf("Lap #\tTime\t\tTotal\n");
    printf("-----\t----\t\t-----\n");
    
    time_t total = 0;
    for (int i = 0; i < lap_count; i++) {
        total += lap_times[i];
        int h = lap_times[i] / 3600;
        int m = (lap_times[i] % 3600) / 60;
        int s = lap_times[i] % 60;
        
        int th = total / 3600;
        int tm = (total % 3600) / 60;
        int ts = total % 60;
        
        printf("%d\t%02d:%02d:%02d\t%02d:%02d:%02d\n", 
               i+1, h, m, s, th, tm, ts);
    }
    printf("\n");
}

void display_stats(time_t current) {
    time_t elapsed = current - start_time - total_pause_time;
    printf("\n=== STATISTICS ===\n");
    printf("Total time: ");
    display_time(elapsed);
    printf("\n");
    
    if (lap_count > 0) {
        // Calculate average lap time
        time_t total_lap_time = 0;
        for (int i = 0; i < lap_count; i++) {
            total_lap_time += lap_times[i];
        }
        time_t avg = total_lap_time / lap_count;
        printf("Average lap: ");
        display_time(avg);
        printf("\n");
        
        // Find best and worst lap
        time_t best = lap_times[0];
        time_t worst = lap_times[0];
        int best_idx = 0, worst_idx = 0;
        
        for (int i = 1; i < lap_count; i++) {
            if (lap_times[i] < best) {
                best = lap_times[i];
                best_idx = i;
            }
            if (lap_times[i] > worst) {
                worst = lap_times[i];
                worst_idx = i;
            }
        }
        
        printf("Best lap (#%d): ", best_idx+1);
        display_time(best);
        printf("\n");
        printf("Worst lap (#%d): ", worst_idx+1);
        display_time(worst);
        printf("\n");
    }
    printf("\n");
}

void show_menu() {
    printf("\n=== COMMANDS ===\n");
    printf("  Enter - Lap/Stop current lap\n");
    printf("  Space - Pause/Resume\n");
    printf("  L     - List all laps\n");
    printf("  S     - Show statistics\n");
    printf("  R     - Reset stopwatch\n");
    printf("  Q/C   - Quit\n");
    printf("  Ctrl+C - Exit\n");
    printf("================\n");
}

int kbhit() {
#ifdef _WIN32
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
#endif
}

char getch() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

int main() {
    signal(SIGINT, handle_signal);
    
    system(CLEAR);
    printf("=== COMMAND LINE STOPWATCH ===\n");
    printf("Press any key to start...\n");
    show_menu();
    getch();
    
    start_time = time(NULL);
    lap_start = start_time;
    time_t last_display = start_time;
    time_t current;
    
    while (running) {
        current = time(NULL);
        
        // Update display every second
        if (current != last_display && !paused) {
            display_time(current - start_time - total_pause_time);
            last_display = current;
        } else if (paused) {
            printf("\r[PAUSED] ");
            display_time(current - start_time - total_pause_time);
        }
        
        // Check for key press (non-blocking)
        if (kbhit()) {
            char c = getch();
            c = tolower(c);
            
            switch(c) {
                case '\n':
                case '\r':
                    if (!paused) {
                        time_t lap_time = current - lap_start;
                        if (lap_count < MAX_LAPS) {
                            lap_times[lap_count++] = lap_time;
                            printf("\nLap %d time: ", lap_count);
                            display_time(lap_time);
                            printf("\n");
                        } else {
                            printf("\nMaximum laps reached (%d)!\n", MAX_LAPS);
                        }
                        lap_start = current;
                    }
                    break;
                    
                case ' ':
                    if (paused) {
                        // Resume
                        total_pause_time += current - pause_start;
                        paused = 0;
                        lap_start = current - (lap_start - pause_start);
                        printf("\nResumed!\n");
                    } else {
                        // Pause
                        paused = 1;
                        pause_start = current;
                        printf("\nPaused!\n");
                    }
                    break;
                    
                case 'l':
                    display_lap_times();
                    break;
                    
                case 's':
                    display_stats(current);
                    break;
                    
                case 'r':
                    printf("\nReset stopwatch? (y/n): ");
                    char confirm = getch();
                    if (confirm == 'y' || confirm == 'Y') {
                        start_time = current;
                        lap_start = current;
                        lap_count = 0;
                        total_pause_time = 0;
                        paused = 0;
                        printf("\nStopwatch reset!\n");
                    }
                    break;
                    
                case 'q':
                case 'c':
                    running = 0;
                    printf("\nExiting stopwatch...\n");
                    break;
                    
                default:
                    break;
            }
        }
        
        sleep_ms(50); // Small delay to reduce CPU usage
    }
    
    time_t end_time = time(NULL);
    time_t final_time = end_time - start_time - total_pause_time;
    
    printf("\n\n=== FINAL RESULTS ===\n");
    printf("Final time: ");
    display_time(final_time);
    printf("\n");
    
    if (lap_count > 0) {
        display_lap_times();
        display_stats(end_time);
    }
    
    printf("\nThank you for using Stopwatch!\n");
    
    return 0;
}
