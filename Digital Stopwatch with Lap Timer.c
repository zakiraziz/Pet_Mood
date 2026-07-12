#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define CLEAR "cls"
    #define sleep_ms(ms) Sleep(ms)
    #define COLOR_RED 12
    #define COLOR_GREEN 10
    #define COLOR_YELLOW 14
    #define COLOR_CYAN 11
    #define COLOR_WHITE 15
    #define set_color(c) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c)
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #define CLEAR "clear"
    #define sleep_ms(ms) usleep((ms) * 1000)
    #define COLOR_RED "\033[31m"
    #define COLOR_GREEN "\033[32m"
    #define COLOR_YELLOW "\033[33m"
    #define COLOR_CYAN "\033[36m"
    #define COLOR_WHITE "\033[37m"
    #define COLOR_RESET "\033[0m"
    #define set_color(c) printf("%s", c)
#endif

#define MAX_LAPS 100
#define MAX_SPLITS 50
#define HISTORY_FILE "stopwatch_history.txt"

volatile int running = 1;
time_t start_time, lap_start;
time_t lap_times[MAX_LAPS];
char lap_notes[MAX_LAPS][50];
int lap_count = 0;
int paused = 0;
time_t pause_start = 0;
time_t total_pause_time = 0;

// Split times
time_t split_times[MAX_SPLITS];
char split_labels[MAX_SPLITS][30];
int split_count = 0;

// Target time for countdown mode
time_t target_time = 0;
int countdown_mode = 0;
int countdown_running = 0;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        running = 0;
        printf("\n\nStopwatch stopped!\n");
    }
}

void display_time_enhanced(time_t seconds, int show_milliseconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    
    if (show_milliseconds) {
        int millis = (int)((seconds - (time_t)seconds) * 1000);
        printf("\r%02d:%02d:%02d.%03d", hours, minutes, secs, millis);
    } else {
        printf("\r%02d:%02d:%02d", hours, minutes, secs);
    }
    fflush(stdout);
}

void display_time(time_t seconds) {
    display_time_enhanced(seconds, 0);
}

void display_progress_bar(time_t current, time_t total) {
    if (total == 0) return;
    
    int percentage = (int)((current * 100) / total);
    int bar_width = 30;
    int filled = (percentage * bar_width) / 100;
    
    printf("\n[");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            if (percentage > 80) {
                set_color(COLOR_RED);
                printf("█");
            } else if (percentage > 50) {
                set_color(COLOR_YELLOW);
                printf("█");
            } else {
                set_color(COLOR_GREEN);
                printf("█");
            }
        } else {
            set_color(COLOR_WHITE);
            printf("░");
        }
    }
    set_color(COLOR_WHITE);
    printf("] %d%%", percentage);
}

void display_lap_times() {
    if (lap_count == 0) {
        printf("\nNo laps recorded yet.\n");
        return;
    }
    
    set_color(COLOR_CYAN);
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║                  LAP TIMES                        ║\n");
    printf("╠════════╦════════════╦════════════╦═══════════════╣\n");
    printf("║ Lap #  ║  Lap Time  ║   Total    ║    Note       ║\n");
    printf("╠════════╬════════════╬════════════╬═══════════════╣\n");
    set_color(COLOR_WHITE);
    
    time_t total = 0;
    for (int i = 0; i < lap_count; i++) {
        total += lap_times[i];
        int h = lap_times[i] / 3600;
        int m = (lap_times[i] % 3600) / 60;
        int s = lap_times[i] % 60;
        
        int th = total / 3600;
        int tm = (total % 3600) / 60;
        int ts = total % 60;
        
        printf("║ %4d   ║ %02d:%02d:%02d ║ %02d:%02d:%02d ║ %-13s ║\n", 
               i+1, h, m, s, th, tm, ts, 
               strlen(lap_notes[i]) > 0 ? lap_notes[i] : "");
    }
    
    set_color(COLOR_CYAN);
    printf("╚════════╩════════════╩════════════╩═══════════════╝\n");
    set_color(COLOR_WHITE);
    printf("\n");
}

void display_stats(time_t current) {
    time_t elapsed = current - start_time - total_pause_time;
    
    set_color(COLOR_CYAN);
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║           STATISTICS                   ║\n");
    printf("╠════════════════════════════════════════╣\n");
    set_color(COLOR_WHITE);
    
    printf("║ Total Time:  ");
    display_time_enhanced(elapsed, 1);
    printf("                   ║\n");
    
    if (lap_count > 0) {
        // Calculate average lap time
        time_t total_lap_time = 0;
        for (int i = 0; i < lap_count; i++) {
            total_lap_time += lap_times[i];
        }
        time_t avg = total_lap_time / lap_count;
        
        printf("║ Average Lap: ");
        display_time_enhanced(avg, 1);
        printf("                   ║\n");
        
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
        
        printf("║ Best Lap:    ");
        display_time_enhanced(best, 1);
        printf(" (#%d)          ║\n", best_idx+1);
        
        printf("║ Worst Lap:   ");
        display_time_enhanced(worst, 1);
        printf(" (#%d)          ║\n", worst_idx+1);
        
        // Consistency (standard deviation approximation)
        double variance = 0;
        for (int i = 0; i < lap_count; i++) {
            variance += pow((double)(lap_times[i] - avg), 2);
        }
        variance /= lap_count;
        double std_dev = sqrt(variance);
        printf("║ Consistency: +/-");
        display_time_enhanced((time_t)std_dev, 1);
        printf("                ║\n");
    }
    
    if (split_count > 0) {
        printf("║ Splits:      %d                     ║\n", split_count);
    }
    
    set_color(COLOR_CYAN);
    printf("╚════════════════════════════════════════╝\n");
    set_color(COLOR_WHITE);
    printf("\n");
}

void display_split_times() {
    if (split_count == 0) {
        printf("\nNo splits recorded.\n");
        return;
    }
    
    set_color(COLOR_CYAN);
    printf("\n=== SPLIT TIMES ===\n");
    set_color(COLOR_WHITE);
    for (int i = 0; i < split_count; i++) {
        printf("%d. %s: ", i+1, split_labels[i]);
        display_time_enhanced(split_times[i], 1);
        printf("\n");
    }
    printf("\n");
}

void save_history(time_t total_time, time_t current) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (file == NULL) return;
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char date_str[30];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(file, "Session: %s\n", date_str);
    fprintf(file, "Total Time: %ld seconds\n", total_time);
    fprintf(file, "Laps: %d\n", lap_count);
    if (lap_count > 0) {
        fprintf(file, "Best Lap: %ld seconds\n", lap_times[0]);
        for (int i = 0; i < lap_count; i++) {
            fprintf(file, "  Lap %d: %ld seconds", i+1, lap_times[i]);
            if (strlen(lap_notes[i]) > 0) {
                fprintf(file, " (%s)", lap_notes[i]);
            }
            fprintf(file, "\n");
        }
    }
    fprintf(file, "---\n");
    fclose(file);
    printf("\nHistory saved to %s\n", HISTORY_FILE);
}

void load_history() {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        printf("\nNo history file found.\n");
        return;
    }
    
    printf("\n=== HISTORY ===\n");
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
    printf("\n");
}

void show_menu() {
    set_color(COLOR_CYAN);
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║                     COMMANDS                         ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    set_color(COLOR_WHITE);
    printf("║  Enter   - Lap/Stop current lap                     ║\n");
    printf("║  Space   - Pause/Resume                             ║\n");
    printf("║  L       - List all laps                            ║\n");
    printf("║  S       - Show statistics                          ║\n");
    printf("║  R       - Reset stopwatch                          ║\n");
    printf("║  N       - Add note to current lap                  ║\n");
    printf("║  P       - Add split point                          ║\n");
    printf("║  T       - Set target time (countdown mode)         ║\n");
    printf("║  H       - View history                             ║\n");
    printf("║  E       - Export session to file                   ║\n");
    printf("║  Q/C     - Quit                                     ║\n");
    printf("║  Ctrl+C  - Emergency exit                           ║\n");
    set_color(COLOR_CYAN);
    printf("╚═══════════════════════════════════════════════════════╝\n");
    set_color(COLOR_WHITE);
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
