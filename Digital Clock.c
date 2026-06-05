#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define CLEAR "clear"
#endif

void digitalClock() {
    time_t rawtime;
    struct tm *timeinfo;
    
    while(1) {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        system(CLEAR);
        printf("\n\t==================================\n");
        printf("\t        DIGITAL CLOCK\n");
        printf("\t==================================\n");
        printf("\t        %02d : %02d : %02d\n", 
               timeinfo->tm_hour, 
               timeinfo->tm_min, 
               timeinfo->tm_sec);
        printf("\t==================================\n");
        
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
    }
}

int main() {
    digitalClock();
    return 0;
}