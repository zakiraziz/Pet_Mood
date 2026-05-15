#include "calculator.h"
#include <stdio.h>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        printf("Error: Division by zero!\n");
        return 0;
    }
    return a / b;
}

void print_menu(void) {
    printf("\n=== Simple Calculator ===\n");
    printf("1. Addition (+)\n");
    printf("2. Subtraction (-)\n");
    printf("3. Multiplication (*)\n");
    printf("4. Division (/)\n");
    printf("5. Exit\n");
    printf("========================\n");
}

int get_operator(void) {
    int choice;
    printf("Enter your choice (1-5): ");
    scanf("%d", &choice);
    return choice;
}

double get_number(const char* prompt) {
    double num;
    printf("%s", prompt);
    scanf("%lf", &num);
    return num;
}