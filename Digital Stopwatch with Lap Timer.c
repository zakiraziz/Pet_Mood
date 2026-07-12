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

int get_terminal_width() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

void animate_transition() {
    char spinner[] = "|/-\\";
    for (int i = 0; i < 10; i++) {
        printf("\rLoading... %c", spinner[i % 4]);
        fflush(stdout);
        sleep_ms(50);
    }
    printf("\r          \r");
}

int main() {
    signal(SIGINT, handle_signal);
    
    system(CLEAR);
    set_color(COLOR_CYAN);
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              ADVANCED COMMAND LINE STOPWATCH             ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    set_color(COLOR_WHITE);
    printf("Press any key to start...\n");
    show_menu();
    getch();
    
    start_time = time(NULL);
    lap_start = start_time;
    time_t last_display = start_time;
    time_t current;
    int show_milliseconds = 0;
    
    while (running) {
        current = time(NULL);
        time_t elapsed = current - start_time - total_pause_time;
        
        // Update display every second
        if (current != last_display && !paused) {
            if (countdown_mode && countdown_running) {
                time_t remaining = target_time - elapsed;
                if (remaining <= 0) {
                    printf("\r!!! TIME'S UP !!!   ");
                    set_color(COLOR_RED);
                    for (int i = 0; i < 5; i++) {
                        printf("\a"); // Beep
                        sleep_ms(200);
                        printf("\r!!! TIME'S UP !!!   ");
                        sleep_ms(200);
                    }
                    set_color(COLOR_WHITE);
                    countdown_running = 0;
                    printf("\n");
                } else {
                    display_time_enhanced(remaining, 0);
                }
            } else {
                display_time_enhanced(elapsed, show_milliseconds);
            }
            last_display = current;
        } else if (paused) {
            set_color(COLOR_YELLOW);
            printf("\r[PAUSED] ");
            display_time_enhanced(elapsed, show_milliseconds);
            set_color(COLOR_WHITE);
        }
        
        // Check for key press (non-blocking)
        if (kbhit()) {
            char c = getch();
            c = tolower(c);
            
            switch(c) {
                case '\n':
                case '\r':
                    if (!paused && !countdown_running) {
                        time_t lap_time = current - lap_start;
                        if (lap_count < MAX_LAPS) {
                            lap_times[lap_count] = lap_time;
                            strcpy(lap_notes[lap_count], "");
                            lap_count++;
                            set_color(COLOR_GREEN);
                            printf("\n✓ Lap %d time: ", lap_count);
                            display_time_enhanced(lap_time, 1);
                            set_color(COLOR_WHITE);
                            
                            // Show split times
                            if (split_count > 0) {
                                printf(" (");
                                for (int i = 0; i < split_count; i++) {
                                    printf("%s: ", split_labels[i]);
                                    display_time_enhanced(split_times[i], 1);
                                    if (i < split_count - 1) printf(", ");
                                }
                                printf(")");
                            }
                            printf("\n");
                        } else {
                            set_color(COLOR_RED);
                            printf("\nMaximum laps reached (%d)!\n", MAX_LAPS);
                            set_color(COLOR_WHITE);
                        }
                        lap_start = current;
                        
                        // Reset splits for new lap
                        split_count = 0;
                    }
                    break;
                    
                case ' ':
                    if (paused) {
                        // Resume
                        total_pause_time += current - pause_start;
                        paused = 0;
                        lap_start = current - (lap_start - pause_start);
                        set_color(COLOR_GREEN);
                        printf("\n▶ Resumed!\n");
                        set_color(COLOR_WHITE);
                    } else {
                        // Pause
                        paused = 1;
                        pause_start = current;
                        set_color(COLOR_YELLOW);
                        printf("\n⏸ Paused!\n");
                        set_color(COLOR_WHITE);
                    }
                    break;
                    
                case 'l':
                    display_lap_times();
                    break;
                    
                case 's':
                    display_stats(current);
                    break;
                    
                case 'p':
                    if (!paused && lap_count > 0) {
                        time_t split_time = current - lap_start;
                        if (split_count < MAX_SPLITS) {
                            split_times[split_count] = split_time;
                            printf("\nEnter split label (max 29 chars): ");
                            fflush(stdout);
                            char label[30];
                            if (fgets(label, sizeof(label), stdin)) {
                                label[strcspn(label, "\n")] = 0;
                                if (strlen(label) > 0) {
                                    strcpy(split_labels[split_count], label);
                                } else {
                                    sprintf(split_labels[split_count], "Split %d", split_count+1);
                                }
                            }
                            split_count++;
                            set_color(COLOR_CYAN);
                            printf("✓ Split recorded: %s at ", split_labels[split_count-1]);
                            display_time_enhanced(split_time, 1);
                            printf("\n");
                            set_color(COLOR_WHITE);
                        } else {
                            set_color(COLOR_RED);
                            printf("\nMaximum splits reached (%d)!\n", MAX_SPLITS);
                            set_color(COLOR_WHITE);
                        }
                    }
                    break;
                    
                case 'n':
                    if (lap_count > 0) {
                        printf("\nEnter note for lap %d: ", lap_count);
                        fflush(stdout);
                        char note[50];
                        if (fgets(note, sizeof(note), stdin)) {
                            note[strcspn(note, "\n")] = 0;
                            strcpy(lap_notes[lap_count-1], note);
                            set_color(COLOR_GREEN);
                            printf("✓ Note added: %s\n", note);
                            set_color(COLOR_WHITE);
                        }
                    }
                    break;
                    
                case 't':
                    printf("\nEnter target time in seconds (for countdown): ");
                    int target_secs;
                    if (scanf("%d", &target_secs) == 1 && target_secs > 0) {
                        target_time = target_secs;
                        countdown_mode = 1;
                        countdown_running = 1;
                        start_time = current;
                        lap_start = current;
                        total_pause_time = 0;
                        set_color(COLOR_YELLOW);
                        printf("✓ Countdown mode set for ");
                        display_time_enhanced(target_time, 0);
                        printf("\n");
                        set_color(COLOR_WHITE);
                    }
                    getchar(); // Clear newline
                    break;
                    
                case 'h':
                    load_history();
                    break;
                    
                case 'e':
                    save_history(elapsed, current);
                    break;
                    
                case 'r':
                    printf("\nReset stopwatch? (y/n): ");
                    char confirm = getch();
                    if (confirm == 'y' || confirm == 'Y') {
                        animate_transition();
                        start_time = current;
                        lap_start = current;
                        lap_count = 0;
                        total_pause_time = 0;
                        paused = 0;
                        split_count = 0;
                        countdown_mode = 0;
                        countdown_running = 0;
                        set_color(COLOR_GREEN);
                        printf("\n✓ Stopwatch reset!\n");
                        set_color(COLOR_WHITE);
                    }
                    break;
                    
                case 'm':
                    show_milliseconds = !show_milliseconds;
                    set_color(COLOR_CYAN);
                    printf("\n✓ Milliseconds display %s\n", 
                           show_milliseconds ? "ON" : "OFF");
                    set_color(COLOR_WHITE);
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
    
    system(CLEAR);
    set_color(COLOR_CYAN);
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║             FINAL RESULTS                   ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    set_color(COLOR_WHITE);
    printf("║ Final Time: ");
    display_time_enhanced(final_time, 1);
    printf("                ║\n");
    
    if (lap_count > 0) {
        printf("║ Total Laps: %d                              ║\n", lap_count);
    }
    
    set_color(COLOR_CYAN);
    printf("╚════════════════════════════════════════════╝\n");
    set_color(COLOR_WHITE);
    
    if (lap_count > 0) {
        display_lap_times();
        display_stats(end_time);
    }
    
    // Ask to save
    printf("\nSave session to history? (y/n): ");
    char save = getch();
    if (save == 'y' || save == 'Y') {
        save_history(final_time, end_time);
    }
    
    printf("\nThank you for using Advanced Stopwatch!\n");
    printf("Session ended at: %s", ctime(&end_time));
    
    return 0;
}
    return 0;
