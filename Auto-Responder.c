#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_MESSAGES 100
#define MAX_LENGTH 500

typedef struct {
    char message[MAX_LENGTH];
    time_t timestamp;
    int is_urgent;
    int is_read;
} CallRecord;

CallRecord call_history[MAX_MESSAGES];
int message_count = 0;

void log_message(char *msg) {
    FILE *f = fopen("missed_calls.log", "a");
    if (!f) {
        printf("Error opening log file!\n");
        return;
    }
    time_t now = time(NULL);
    char time_str[100];
    strcpy(time_str, ctime(&now));
    time_str[strlen(time_str) - 1] = '\0';  // Remove newline
    fprintf(f, "[%s] %s\n", time_str, msg);
    fclose(f);
    
    // Store in memory
    if (message_count < MAX_MESSAGES) {
        strcpy(call_history[message_count].message, msg);
        call_history[message_count].timestamp = now;
        call_history[message_count].is_urgent = (strstr(msg, "urgent") || strstr(msg, "URGENT"));
        call_history[message_count].is_read = 0;
        message_count++;
    }
}

void respond(char *msg) {
    if (strstr(msg, "urgent") || strstr(msg, "URGENT")) {
        printf("🔴 AUTO-REPLY: I'll call you back in 5 minutes!\n");
    } else if (strstr(msg, "meeting") || strstr(msg, "Meeting")) {
        printf("📅 AUTO-REPLY: In a meeting. Will respond later.\n");
    } else if (strstr(msg, "call") || strstr(msg, "Call") || strstr(msg, "CALL")) {
        printf("📞 AUTO-REPLY: I'll call you when I'm free!\n");
    } else if (strstr(msg, "help") || strstr(msg, "Help") || strstr(msg, "HELP")) {
        printf("❓ AUTO-REPLY: How can I help you? I'll respond as soon as possible.\n");
    } else {
        printf("📱 AUTO-REPLY: Busy right now. Please text or email.\n");
    }
}

void view_logs() {
    FILE *f = fopen("missed_calls.log", "r");
    if (!f) {
        printf("📭 No logs yet.\n");
        return;
    }
    
    printf("\n========== CALL LOGS ==========\n");
    char line[MAX_LENGTH];
    int line_num = 1;
    while (fgets(line, MAX_LENGTH, f)) {
        printf("%d. %s", line_num++, line);
    }
    fclose(f);
    printf("===============================\n");
}

void view_recent_messages() {
    printf("\n========== RECENT MESSAGES ==========\n");
    if (message_count == 0) {
        printf("No messages in memory.\n");
        return;
    }
    
    int start = (message_count > 5) ? message_count - 5 : 0;
    for (int i = start; i < message_count; i++) {
        char time_str[100];
        strcpy(time_str, ctime(&call_history[i].timestamp));
        time_str[strlen(time_str) - 1] = '\0';
        printf("%d. [%s] %s %s\n", 
               i + 1, 
               time_str,
               call_history[i].is_urgent ? "⚠️ URGENT:" : "",
               call_history[i].message);
    }
    printf("=====================================\n");
}

void search_logs(char *keyword) {
    FILE *f = fopen("missed_calls.log", "r");
    if (!f) {
        printf("No logs to search.\n");
        return;
    }
    
    char line[MAX_LENGTH];
    int found = 0;
    printf("\n========== SEARCH RESULTS ==========\n");
    while (fgets(line, MAX_LENGTH, f)) {
        if (strstr(line, keyword)) {
            printf("%s", line);
            found = 1;
        }
    }
    fclose(f);
    
    if (!found) {
        printf("No messages containing '%s' found.\n", keyword);
    }
    printf("=====================================\n");
}

void clear_logs() {
    char confirm;
    printf("⚠️ Are you sure you want to clear all logs? (y/n): ");
    scanf(" %c", &confirm);
    getchar();
    
    if (confirm == 'y' || confirm == 'Y') {
        FILE *f = fopen("missed_calls.log", "w");
        if (f) {
            fclose(f);
            message_count = 0;
            printf("✅ Logs cleared successfully!\n");
        } else {
            printf("❌ Error clearing logs!\n");
        }
    } else {
        printf("Operation cancelled.\n");
    }
}

void display_statistics() {
    printf("\n========== STATISTICS ==========\n");
    printf("Total messages: %d\n", message_count);
    
    int urgent_count = 0;
    int unread_count = 0;
    for (int i = 0; i < message_count; i++) {
        if (call_history[i].is_urgent) urgent_count++;
        if (!call_history[i].is_read) unread_count++;
    }
    
    printf("Urgent messages: %d\n", urgent_count);
    printf("Unread messages: %d\n", unread_count);
    printf("=================================\n");
}

void mark_messages_read() {
    for (int i = 0; i < message_count; i++) {
        call_history[i].is_read = 1;
    }
    printf("✅ All messages marked as read!\n");
}

int main() {
    char message[MAX_LENGTH];
    char keyword[MAX_LENGTH];
    int choice;
    
    printf("📱 VOICEMAIL SYSTEM v2.0\n");
    printf("========================\n");
    
    do {
        printf("\n📋 MENU:\n");
        printf("1. 📨 Incoming Message\n");
        printf("2. 📖 View Full Log\n");
        printf("3. 📝 View Recent Messages (Last 5)\n");
        printf("4. 🔍 Search Logs\n");
        printf("5. 📊 View Statistics\n");
        printf("6. ✅ Mark All as Read\n");
        printf("7. 🗑️ Clear Logs\n");
        printf("8. 🚪 Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1:
                printf("Enter message: ");
                fgets(message, MAX_LENGTH, stdin);
                message[strcspn(message, "\n")] = 0;
                respond(message);
                log_message(message);
                printf("✅ Message logged successfully!\n");
                break;
                
            case 2:
                view_logs();
                break;
                
            case 3:
                view_recent_messages();
                break;
                
            case 4:
                printf("Enter keyword to search: ");
                fgets(keyword, MAX_LENGTH, stdin);
                keyword[strcspn(keyword, "\n")] = 0;
                search_logs(keyword);
                break;
                
            case 5:
                display_statistics();
                break;
                
            case 6:
                mark_messages_read();
                break;
                
            case 7:
                clear_logs();
                break;
                
            case 8:
                printf("👋 Goodbye!\n");
                break;
                
            default:
                printf("❌ Invalid choice! Please try again.\n");
        }
    } while(choice != 8);
    
    return 0;
}
