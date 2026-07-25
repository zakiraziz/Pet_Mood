#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DESC 200
#define MAX_LINE 256
#define FILENAME "tasks.txt"

// Task structure
typedef struct Task {
    int id;
    char description[MAX_DESC];
    int priority;     // 1-5 (1 = highest)
    int completed;    // 0 = incomplete, 1 = complete
    struct Task* next;
} Task;

// Function prototypes
Task* add_task(Task* head, const char* desc, int priority);
void list_tasks(Task* head);
void list_completed(Task* head);
void mark_completed(Task* head, int id);
void delete_task(Task** head, int id);
void save_tasks(Task* head);
Task* load_tasks(void);
void free_tasks(Task* head);
void clear_screen(void);
void press_any_key(void);
int get_valid_int(const char* prompt, int min, int max);

// Main menu
int main() {
    Task* tasks = load_tasks();
    int choice, id, priority;
    char description[MAX_DESC];
    
    while (1) {
        clear_screen();
        printf("\n========== TO-DO LIST MANAGER ==========\n");
        printf(" 1. Add Task\n");
        printf(" 2. List All Tasks\n");
        printf(" 3. List Completed Tasks\n");
        printf(" 4. Mark Task as Complete\n");
        printf(" 5. Delete Task\n");
        printf(" 6. Save & Exit\n");
        printf("=======================================\n");
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        getchar(); // Clear newline
        
        switch (choice) {
            case 1:
                printf("\nEnter task description: ");
                fgets(description, MAX_DESC, stdin);
                description[strcspn(description, "\n")] = 0; // Remove newline
                
                if (strlen(description) == 0) {
                    printf("Description cannot be empty!\n");
                    press_any_key();
                    break;
                }
                
                priority = get_valid_int("Enter priority (1-5, 1=highest): ", 1, 5);
                tasks = add_task(tasks, description, priority);
                printf("\n✓ Task added successfully!\n");
                press_any_key();
                break;
                
            case 2:
                printf("\n");
                list_tasks(tasks);
                press_any_key();
                break;
                
            case 3:
                printf("\n");
                list_completed(tasks);
                press_any_key();
                break;
                
            case 4:
                printf("\nEnter task ID to mark complete: ");
                scanf("%d", &id);
                mark_completed(tasks, id);
                printf("\n✓ Task marked complete!\n");
                press_any_key();
                break;
                
            case 5:
                printf("\nEnter task ID to delete: ");
                scanf("%d", &id);
                delete_task(&tasks, id);
                printf("\n✓ Task deleted!\n");
                press_any_key();
                break;
                
            case 6:
                save_tasks(tasks);
                free_tasks(tasks);
                printf("\n✓ Tasks saved. Goodbye!\n");
                return 0;
                
            default:
                printf("\nInvalid choice! Try again.\n");
                press_any_key();
        }
    }
    
    return 0;
}

// Add a new task (inserts in priority order)
Task* add_task(Task* head, const char* desc, int priority) {
    Task* new_task = (Task*)malloc(sizeof(Task));
    if (!new_task) {
        printf("Memory allocation failed!\n");
        return head;
    }
    
    // Generate new ID
    static int next_id = 1;
    Task* temp = head;
    while (temp) {
        if (temp->id >= next_id) next_id = temp->id + 1;
        temp = temp->next;
    }
    
    new_task->id = next_id++;
    strcpy(new_task->description, desc);
    new_task->priority = priority;
    new_task->completed = 0;
    new_task->next = NULL;
    
    // Insert in priority order (higher priority = lower number)
    if (!head || priority < head->priority) {
        new_task->next = head;
        return new_task;
    }
    
    Task* current = head;
    while (current->next && current->next->priority <= priority) {
        current = current->next;
    }
    
    new_task->next = current->next;
    current->next = new_task;
    
    return head;
}

// List all tasks (incomplete only)
void list_tasks(Task* head) {
    if (!head) {
        printf("No tasks found!\n");
        return;
    }
    
    int found = 0;
    printf("ID  Priority  Status    Description\n");
    printf("--- -------- ---------- %s\n", "------------------------------");
    
    Task* current = head;
    while (current) {
        if (!current->completed) {
            printf("%-3d %-8d %-9s %s\n", 
                   current->id, 
                   current->priority,
                   "[ ]",
                   current->description);
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No incomplete tasks! Great job!\n");
    }
}

// List completed tasks
void list_completed(Task* head) {
    if (!head) {
        printf("No tasks found!\n");
        return;
    }
    
    int found = 0;
    printf("ID  Priority  Status    Description\n");
    printf("--- -------- ---------- %s\n", "------------------------------");
    
    Task* current = head;
    while (current) {
        if (current->completed) {
            printf("%-3d %-8d %-9s %s\n", 
                   current->id, 
                   current->priority,
                   "[✓]",
                   current->description);
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No completed tasks yet.\n");
    }
}

// Mark a task as completed
void mark_completed(Task* head, int id) {
    Task* current = head;
    while (current) {
        if (current->id == id) {
            current->completed = 1;
            printf("\nTask #%d completed: %s\n", id, current->description);
            return;
        }
        current = current->next;
    }
    printf("\nTask #%d not found!\n", id);
}

// Delete a task
void delete_task(Task** head, int id) {
    if (!*head) {
        printf("No tasks to delete!\n");
        return;
    }
    
    Task* current = *head;
    Task* prev = NULL;
    
    while (current && current->id != id) {
        prev = current;
        current = current->next;
    }
    
    if (!current) {
        printf("Task #%d not found!\n", id);
        return;
    }
    
    if (prev) {
        prev->next = current->next;
    } else {
        *head = current->next;
    }
    
    free(current);
    printf("\nTask #%d deleted successfully.", id);
}

// Save tasks to file
void save_tasks(Task* head) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error: Could not save tasks!\n");
        return;
    }
    
    Task* current = head;
    int count = 0;
    
    while (current) {
        fprintf(file, "%d|%s|%d|%d\n", 
                current->id, 
                current->description,
                current->priority, 
                current->completed);
        count++;
        current = current->next;
    }
    
    fclose(file);
    printf("\n✓ %d tasks saved to %s\n", count, FILENAME);
}

// Load tasks from file
Task* load_tasks(void) {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        return NULL; // No existing file
    }
    
    Task* head = NULL;
    Task* tail = NULL;
    char line[MAX_LINE];
    int id, priority, completed;
    char description[MAX_DESC];
    
    while (fgets(line, MAX_LINE, file)) {
        // Parse line: id|description|priority|completed
        char* token = strtok(line, "|");
        if (!token) continue;
        id = atoi(token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(description, token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        priority = atoi(token);
        
        token = strtok(NULL, "|\n");
        if (!token) continue;
        completed = atoi(token);
        
        // Create task
        Task* new_task = (Task*)malloc(sizeof(Task));
        if (!new_task) {
            printf("Memory allocation failed while loading!\n");
            break;
        }
        
        new_task->id = id;
        strcpy(new_task->description, description);
        new_task->priority = priority;
        new_task->completed = completed;
        new_task->next = NULL;
        
        // Add to list
        if (!head) {
            head = new_task;
            tail = new_task;
        } else {
            tail->next = new_task;
            tail = new_task;
        }
    }
    
    fclose(file);
    return head;
}

// Free all task memory
void free_tasks(Task* head) {
    Task* current = head;
    while (current) {
        Task* next = current->next;
        free(current);
        current = next;
    }
}

// Clear screen (cross-platform)
void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Wait for user to press any key
void press_any_key(void) {
    printf("\nPress any key to continue...");
    getchar();
}

// Get validated integer input
int get_valid_int(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            getchar(); // Clear newline
            return value;
        }
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
        getchar(); // Clear invalid input
    }
}