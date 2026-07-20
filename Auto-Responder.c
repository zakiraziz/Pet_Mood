#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_MESSAGE 500
#define LOG_FILE "missed_calls.log"

// Structure to store call information
typedef struct {
    char message[MAX_MESSAGE];
    char timestamp[100];
    char response[100];
    int priority; // 0=normal, 1=urgent, 2=meeting
} CallRecord;

// Function prototypes
void log_message(char *msg, char *response);
void respond(char *msg, char *response);
void view_logs();
void search_logs();
void clear_logs();
void view_stats();
void to_lowercase(char *str);
int get_priority(char *msg);
void add_timestamp(char *buffer, size_t size);

void log_message(char *msg, char *response) {
    FILE *f = fopen(LOG_FILE, "a");
    if (!f) {
        printf("Error: Unable to open log file.\n");
        return;
    }
    
    time_t now = time(NULL);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    // Get priority level
    int priority = get_priority(msg);
    char *priority_str = priority == 1 ? "URGENT" : (priority == 2 ? "MEETING" : "NORMAL");
    
    fprintf(f, "[%s] [%s] Priority: %s | Message: %s | Response: %s\n", 
            timestamp, priority_str, priority_str, msg, response);
    fclose(f);
}

void respond(char *msg, char *response) {
    char lower_msg[MAX_MESSAGE];
    strcpy(lower_msg, msg);
    to_lowercase(lower_msg);
    
    if (strstr(lower_msg, "urgent") || strstr(lower_msg, "emergency")) {
        strcpy(response, "AUTO-REPLY: I'll call you back in 5 minutes!");
        printf("%s\n", response);
    } else if (strstr(lower_msg, "meeting") || strstr(lower_msg, "conference")) {
        strcpy(response, "AUTO-REPLY: In a meeting. Will respond later.");
        printf("%s\n", response);
    } else if (strstr(lower_msg, "call") || strstr(lower_msg, "phone")) {
        strcpy(response, "AUTO-REPLY: Will call you as soon as I'm free.");
        printf("%s\n", response);
    } else if (strstr(lower_msg, "email") || strstr(lower_msg, "mail")) {
        strcpy(response, "AUTO-REPLY: Please email me at support@example.com");
        printf("%s\n", response);
    } else {
        strcpy(response, "AUTO-REPLY: Busy right now. Please text or email.");
        printf("%s\n", response);
    }
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int get_priority(char *msg) {
    char lower_msg[MAX_MESSAGE];
    strcpy(lower_msg, msg);
    to_lowercase(lower_msg);
    
    if (strstr(lower_msg, "urgent") || strstr(lower_msg, "emergency")) {
        return 1; // Urgent
    } else if (strstr(lower_msg, "meeting") || strstr(lower_msg, "conference")) {
        return 2; // Meeting
    }
    return 0; // Normal
}

void view_logs() {
    FILE *f = fopen(LOG_FILE, "r");
    if (!f) {
        printf("No logs yet.\n");
        return;
    }
    
    char line[MAX_MESSAGE];
    int count = 0;
    printf("\n=== ALL CALL LOGS ===\n");
    while (fgets(line, MAX_MESSAGE, f)) {
        printf("%d. %s", ++count, line);
    }
    fclose(f);
    printf("Total: %d logs\n", count);
}

