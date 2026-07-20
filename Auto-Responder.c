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
void search_logs() {
    char search_term[MAX_MESSAGE];
    printf("Enter search term: ");
    fgets(search_term, MAX_MESSAGE, stdin);
    search_term[strcspn(search_term, "\n")] = 0;
    
    FILE *f = fopen(LOG_FILE, "r");
    if (!f) {
        printf("No logs yet.\n");
        return;
    }
    
    char line[MAX_MESSAGE];
    int found = 0;
    printf("\n=== SEARCH RESULTS ===\n");
    while (fgets(line, MAX_MESSAGE, f)) {
        if (strstr(line, search_term)) {
            printf("%s", line);
            found++;
        }
    }
    fclose(f);
    
    if (!found) {
        printf("No logs found containing '%s'\n", search_term);
    } else {
        printf("Found %d log entries\n", found);
    }
}

void clear_logs() {
    char confirm;
    printf("Are you sure you want to clear all logs? (y/n): ");
    scanf(" %c", &confirm);
    getchar();
    
    if (confirm == 'y' || confirm == 'Y') {
        FILE *f = fopen(LOG_FILE, "w");
        if (f) {
            fclose(f);
            printf("Logs cleared successfully.\n");
        } else {
            printf("Error clearing logs.\n");
        }
    } else {
        printf("Clear operation cancelled.\n");
    }
}

void view_stats() {
    FILE *f = fopen(LOG_FILE, "r");
    if (!f) {
        printf("No logs yet.\n");
        return;
    }
    
    int total = 0, urgent = 0, meeting = 0, normal = 0;
    char line[MAX_MESSAGE];
    
    while (fgets(line, MAX_MESSAGE, f)) {
        total++;
        if (strstr(line, "Priority: URGENT")) urgent++;
        else if (strstr(line, "Priority: MEETING")) meeting++;
        else normal++;
    }
    fclose(f);
    
    printf("\n=== STATISTICS ===\n");
    printf("Total messages: %d\n", total);
    printf("Urgent: %d (%.1f%%)\n", urgent, total > 0 ? (float)urgent/total*100 : 0);
    printf("Meeting: %d (%.1f%%)\n", meeting, total > 0 ? (float)meeting/total*100 : 0);
    printf("Normal: %d (%.1f%%)\n", normal, total > 0 ? (float)normal/total*100 : 0);
}

int main() {
    char message[MAX_MESSAGE];
    char response[MAX_MESSAGE];
    int choice;
    
    printf("=== MISSED CALL AUTO-RESPONDER ===\n");
    printf("Version 2.0 - Enhanced Features\n");
    
    do {
        printf("\n=== MENU ===\n");
        printf("1. Incoming Message\n");
        printf("2. View All Logs\n");
        printf("3. Search Logs\n");
        printf("4. View Statistics\n");
        printf("5. Clear Logs\n");
        printf("6. Exit\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        
        switch(choice) {
            case 1:
                printf("Enter message: ");
                fgets(message, MAX_MESSAGE, stdin);
                message[strcspn(message, "\n")] = 0;
                
                if (strlen(message) == 0) {
                    printf("Empty message. Please enter a valid message.\n");
                    break;
                }
                
                respond(message, response);
                log_message(message, response);
                printf("Message logged successfully.\n");
                break;
                
            case 2:
                view_logs();
                break;
                
            case 3:
                search_logs();
                break;
                
            case 4:
                view_stats();
                break;
                
            case 5:
                clear_logs();
                break;
                
            case 6:
                printf("Thank you for using the Auto-Responder. Goodbye!\n");
                break;
                
            default:
                printf("Invalid choice. Please select 1-6.\n");
        }
    } while(choice != 6);
    
    return 0;
}
