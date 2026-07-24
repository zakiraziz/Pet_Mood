#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TASKS 100
#define MAX_LENGTH 200

char tasks[MAX_TASKS][MAX_LENGTH];
int task_count = 0;

void add_task() {
    if (task_count >= MAX_TASKS) {
        printf("\n❌ Task list is full!\n");
        return;
    }
    
    printf("\n📝 Enter task: ");
    getchar(); // Clear buffer
    fgets(tasks[task_count], MAX_LENGTH, stdin);
    tasks[task_count][strcspn(tasks[task_count], "\n")] = 0; // Remove newline
    task_count++;
    printf("✅ Task added successfully!\n");
}

void view_tasks() {
    if (task_count == 0) {
        printf("\n📭 No tasks yet!\n");
        return;
    }
    
    printf("\n📋 YOUR TASKS:\n");
    printf("-----------------\n");
    for (int i = 0; i < task_count; i++) {
        printf("%d. %s\n", i+1, tasks[i]);
    }
    printf("-----------------\n");
}

void delete_task() {
    if (task_count == 0) {
        printf("\n📭 No tasks to delete!\n");
        return;
    }
    
    view_tasks();
    int num;
    printf("\n🗑️ Enter task number to delete: ");
    scanf("%d", &num);
    
    if (num < 1 || num > task_count) {
        printf("❌ Invalid task number!\n");
        return;
    }
    
    // Shift all tasks left
    for (int i = num-1; i < task_count-1; i++) {
        strcpy(tasks[i], tasks[i+1]);
    }
    task_count--;
    printf("✅ Task deleted successfully!\n");
}

int main() {
    int choice;
    
    printf("\n==================================\n");
    printf("   📋 TO-DO LIST MANAGER\n");
    printf("==================================\n");
    
    while (1) {
        printf("\n[1] Add Task\n");
        printf("[2] View Tasks\n");
        printf("[3] Delete Task\n");
        printf("[4] Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_task(); break;
            case 2: view_tasks(); break;
            case 3: delete_task(); break;
            case 4: 
                printf("\n👋 Goodbye!\n");
                return 0;
            default: 
                printf("❌ Invalid choice! Try again.\n");
        }
    }
    return 0;
}