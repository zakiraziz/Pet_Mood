#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

void print_usage(const char *prog_name) {
    printf("Usage: %s num op num\n", prog_name);
    printf("Supported operators: + - * / %% ^ (power)\n");
    printf("Special commands:\n");
    printf("  %s sqrt num     - Calculate square root\n", prog_name);
    printf("  %s abs num      - Calculate absolute value\n", prog_name);
    printf("  %s help         - Show this help\n", prog_name);
}

int is_number(const char *str) {
    if (!str || *str == '\0') return 0;
    if (*str == '-') str++;
    int has_digit = 0;
    int has_decimal = 0;
    while (*str) {
        if (isdigit(*str)) {
            has_digit = 1;
        } else if (*str == '.' && !has_decimal) {
            has_decimal = 1;
        } else {
            return 0;
        }
        str++;
    }
    return has_digit;
}

int main(int argc, char *argv[]) {
    // Help command
    if (argc == 2 && (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage(argv[0]);
        return 0;
    }
    
    // Handle unary operations
    if (argc == 3) {
        if (strcmp(argv[1], "sqrt") == 0 && is_number(argv[2])) {
            double a = atof(argv[2]);
            if (a < 0) {
                printf("Error: Cannot calculate square root of negative number\n");
                return 1;
            }
            printf("sqrt(%g) = %g\n", a, sqrt(a));
            return 0;
        }
        else if (strcmp(argv[1], "abs") == 0 && is_number(argv[2])) {
            double a = atof(argv[2]);
            printf("abs(%g) = %g\n", a, fabs(a));
            return 0;
        }
        else {
            printf("Error: Unknown command or invalid number\n");
            print_usage(argv[0]);
            return 1;
        }
    }
    
    // Binary operations
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Validate inputs
    if (!is_number(argv[1]) || !is_number(argv[3])) {
        printf("Error: Invalid number format\n");
        return 1;
    }
    
    double a = atof(argv[1]), b = atof(argv[3]);
    char op = argv[2][0];
    
    // Check for multi-character operator
    if (argv[2][1] != '\0') {
        if (strcmp(argv[2], "^") == 0 || strcmp(argv[2], "**") == 0) {
            printf("%g\n", pow(a, b));
            return 0;
        }
        printf("Error: Bad operator '%s'\n", argv[2]);
        return 1;
    }
    
    // Perform calculation
    switch (op) {
        case '+':
            printf("%g\n", a + b);
            break;
        case '-':
            printf("%g\n", a - b);
            break;
        case '*':
            printf("%g\n", a * b);
            break;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero\n");
                return 1;
            }
            printf("%g\n", a / b);
            break;
        case '%':
            if (b == 0) {
                printf("Error: Modulo by zero\n");
                return 1;
            }
            printf("%g\n", fmod(a, b));
            break;
        case '^':
            printf("%g\n", pow(a, b));
            break;
        default:
            printf("Error: Unsupported operator '%c'\n", op);
            printf("Supported operators: + - * / %% ^\n");
            return 1;
    }
    
    return 0;
}