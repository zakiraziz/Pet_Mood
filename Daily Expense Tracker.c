#include <stdio.h>
#include <string.h>

#define MAX 50

typedef struct {
    char category[50];
    float amount;
} Expense;

Expense expenses[MAX];
int count = 0;
float budget = 1000.0;

void add_expense() {
    printf("Enter category (Food/Transport/Shopping/Other): ");
    getchar();
    fgets(expenses[count].category, 50, stdin);
    expenses[count].category[strcspn(expenses[count].category, "\n")] = 0;
    
    printf("Enter amount: $");
    scanf("%f", &expenses[count].amount);
    count++;
    printf("Expense logged!\n");
}

void show_summary() {
    float total = 0;
    printf("\n=== DAILY SUMMARY ===\n");
    for (int i = 0; i < count; i++) {
        printf("%s: $%.2f\n", expenses[i].category, expenses[i].amount);
        total += expenses[i].amount;
    }
    printf("-------------------\n");
    printf("Total spent: $%.2f\n", total);
    printf("Budget: $%.2f\n", budget);
    if (total > budget) {
        printf("⚠️ OVER BUDGET by $%.2f!\n", total - budget);
    } else {
        printf("✅ Under budget by $%.2f\n", budget - total);
    }
}

int main() {
    int choice;
    do {
        printf("\n1. Add Expense\n2. Show Summary\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: add_expense(); break;
            case 2: show_summary(); break;
        }
    } while(choice != 3);
    return 0;
}