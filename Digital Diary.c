#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENTRY 1000
#define FILENAME "diary.txt"

void add_entry() {
    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp)-1] = '\0'; // Remove newline
    
    char entry[MAX_ENTRY];
    printf("Write your entry (max 1000 chars):\n");
    getchar(); // Clear newline
    fgets(entry, MAX_ENTRY, stdin);
    
    fprintf(fp, "[%s]\n%s\n---\n", timestamp, entry);
    fclose(fp);
    printf("Entry saved!\n");
}

void view_entries() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No diary entries found.\n");
        return;
    }
    
    char line[MAX_ENTRY];
    printf("\n========== YOUR DIARY ==========\n");
    while (fgets(line, MAX_ENTRY, fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void search_entries() {
    char keyword[50];
    printf("Enter keyword to search: ");
    getchar();
    fgets(keyword, 50, stdin);
    keyword[strcspn(keyword, "\n")] = 0;
    
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No entries found.\n");
        return;
    }
    
    char line[MAX_ENTRY];
    int found = 0;
    printf("\n--- Search Results ---\n");
    while (fgets(line, MAX_ENTRY, fp)) {
        if (strstr(line, keyword)) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) printf("No entries found with '%s'\n", keyword);
    fclose(fp);
}

int main() {
    int choice;
    do {
        printf("\n=== DIGITAL DIARY ===\n");
        printf("1. Add Entry\n2. View All Entries\n3. Search Entries\n4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_entry(); break;
            case 2: view_entries(); break;
            case 3: search_entries(); break;
            case 4: printf("Goodbye!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 4);
    return 0;
}