#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_LEN 256

char tasks[MAX_TASKS][MAX_LEN];
int count = 0;

void save() {
    FILE *f = fopen("tasks.txt", "w");
    for (int i = 0; i < count; i++)
        fprintf(f, "%s\n", tasks[i]);
    fclose(f);
}

void load() {
    FILE *f = fopen("tasks.txt", "r");
    if (!f) return;
    while (count < MAX_TASKS && fgets(tasks[count], MAX_LEN, f))
        tasks[count++][strcspn(tasks[count-1], "\n")] = 0;
    fclose(f);
}

void add(char *task) {
    if (count < MAX_TASKS) {
        strcpy(tasks[count++], task);
        save();
        printf("✓ Added: %s\n", task);
    } else printf("✗ Task list full!\n");
}

void list() {
    if (!count) printf("No tasks.\n");
    else for (int i = 0; i < count; i++)
        printf("%d. %s\n", i+1, tasks[i]);
}

void delete(int n) {
    if (n < 1 || n > count) printf("✗ Invalid number\n");
    else {
        printf("✓ Deleted: %s\n", tasks[n-1]);
        for (int i = n-1; i < count-1; i++)
            strcpy(tasks[i], tasks[i+1]);
        count--;
        save();
    }
}

int main() {
    load();
    char cmd[10], arg[MAX_LEN];
    
    printf("Todo Manager - Commands: add <task>, list, del <num>, quit\n");
    
    while (1) {
        printf("> ");
        scanf("%s", cmd);
        
        if (!strcmp(cmd, "add")) {
            scanf(" %[^\n]", arg);
            add(arg);
        }
        else if (!strcmp(cmd, "list")) list();
        else if (!strcmp(cmd, "del")) {
            scanf("%d", &arg[0]);
            delete(arg[0]);
        }
        else if (!strcmp(cmd, "quit")) break;
        else printf("Commands: add, list, del, quit\n");
    }
    return 0;
}