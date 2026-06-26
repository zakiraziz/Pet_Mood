#include <stdio.h>
#include <string.h>
#include <time.h>

void log_message(char *msg) {
    FILE *f = fopen("missed_calls.log", "a");
    time_t now = time(NULL);
    fprintf(f, "[%s] %s\n", ctime(&now), msg);
    fclose(f);
}

void respond(char *msg) {
    if (strstr(msg, "urgent") || strstr(msg, "URGENT")) {
        printf("AUTO-REPLY: I'll call you back in 5 minutes!\n");
    } else if (strstr(msg, "meeting") || strstr(msg, "Meeting")) {
        printf("AUTO-REPLY: In a meeting. Will respond later.\n");
    } else {
        printf("AUTO-REPLY: Busy right now. Please text or email.\n");
    }
}

int main() {
    char message[500];
    int choice;
    
    do {
        printf("\n1. Incoming Message\n2. View Log\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();
        
        if (choice == 1) {
            printf("Enter message: ");
            fgets(message, 500, stdin);
            message[strcspn(message, "\n")] = 0;
            respond(message);
            log_message(message);
        } else if (choice == 2) {
            FILE *f = fopen("missed_calls.log", "r");
            if (!f) printf("No logs yet.\n");
            else {
                char line[500];
                while (fgets(line, 500, f)) printf("%s", line);
                fclose(f);
            }
        }
    } while(choice != 3);
    return 0;
}