#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

int main() {
    time_t start, current;
    int running = 0, laps = 0;
    char command;
    time_t lapTimes[20];
    time_t lapStart;
    
    printf("⏱️  Stopwatch\n");
    printf("Commands: s=start, p=stop, l=lap, r=reset, q=quit\n");
    
    while(1) {
        printf("\n> ");
        scanf(" %c", &command);
        
        switch(command) {
            case 's':
                if(!running) {
                    time(&start);
                    running = 1;
                    lapStart = start;
                    printf("▶️  Started!\n");
                }
                break;
                
            case 'p':
                if(running) {
                    time(&current);
                    running = 0;
                    printf("⏸️  Stopped! Time: %.2f seconds\n", 
                           difftime(current, start));
                }
                break;
                
            case 'l':
                if(running) {
                    time(&current);
                    if(laps < 20) {
                        lapTimes[laps] = current - lapStart;
                        printf("🏁 Lap %d: %.2f seconds\n", 
                               laps+1, difftime(current, lapStart));
                        lapStart = current;
                        laps++;
                    }
                } else {
                    printf("Start stopwatch first!\n");
                }
                break;
                
            case 'r':
                running = 0;
                laps = 0;
                printf("🔄 Reset!\n");
                break;
                
            case 'q':
                printf("Goodbye! 👋\n");
                return 0;
                
            default:
                printf("Invalid command!\n");
        }
    }
}