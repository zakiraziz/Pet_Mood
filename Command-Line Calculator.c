#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Global variable for error handling
bool has_error = false;

void print_usage(const char *prog_name) {
    printf("Usage: %s num op num\n", prog_name);
    printf("Supported operators: + - * / %% ^ (power)\n");
    printf("Special commands:\n");
    printf("  %s sqrt num     - Calculate square root\n", prog_name);
    printf("  %s abs num      - Calculate absolute value\n", prog_name);
    printf("  %s log num      - Calculate natural logarithm\n", prog_name);
    printf("  %s log10 num    - Calculate base-10 logarithm\n", prog_name);
    printf("  %s sin num      - Calculate sine (in radians)\n", prog_name);
    printf("  %s cos num      - Calculate cosine (in radians)\n", prog_name);
    printf("  %s tan num      - Calculate tangent (in radians)\n", prog_name);
    printf("  %s fact num     - Calculate factorial (integer only)\n", prog_name);
    printf("  %s help         - Show this help\n", prog_name);
    printf("  %s --version    - Show version information\n", prog_name);
}

void print_version() {
    printf("Advanced Calculator v2.0\n");
    printf("Built with: C99 Standard\n");
    printf("Features: Basic arithmetic, trigonometry, logarithms, factorial\n");
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

int is_integer(const char *str) {
    if (!str || *str == '\0') return 0;
    if (*str == '-') str++;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

unsigned long long factorial(int n) {
    if (n < 0) return 0;
    if (n == 0 || n == 1) return 1;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double calculate_binary(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                printf("Error: Division by zero\n");
                has_error = true;
                return 0;
            }
            return a / b;
        case '%':
            if (b == 0) {
                printf("Error: Modulo by zero\n");
                has_error = true;
                return 0;
            }
            return fmod(a, b);
        case '^':
            return pow(a, b);
        default:
            printf("Error: Unsupported operator '%c'\n", op);
            has_error = true;
            return 0;
    }
}

int main(int argc, char *argv[]) {
    // Version command
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        print_version();
        return 0;
    }
    
    // Help command
    if (argc == 2 && (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage(argv[0]);
        return 0;
    }
    
    // Handle unary operations
    if (argc == 3) {
        if (!is_number(argv[2])) {
            printf("Error: Invalid number format\n");
            return 1;
        }
        double a = atof(argv[2]);
        int int_a = (int)a;
        
        if (strcmp(argv[1], "sqrt") == 0) {
            if (a < 0) {
                printf("Error: Cannot calculate square root of negative number\n");
                return 1;
            }
            printf("sqrt(%g) = %g\n", a, sqrt(a));
            return 0;
        }
        else if (strcmp(argv[1], "abs") == 0) {
            printf("abs(%g) = %g\n", a, fabs(a));
            return 0;
        }
        else if (strcmp(argv[1], "log") == 0) {
            if (a <= 0) {
                printf("Error: Logarithm undefined for non-positive numbers\n");
                return 1;
            }
            printf("ln(%g) = %g\n", a, log(a));
            return 0;
        }
        else if (strcmp(argv[1], "log10") == 0) {
            if (a <= 0) {
                printf("Error: Logarithm undefined for non-positive numbers\n");
                return 1;
            }
            printf("log10(%g) = %g\n", a, log10(a));
            return 0;
        }
        else if (strcmp(argv[1], "sin") == 0) {
            printf("sin(%g) = %g\n", a, sin(a));
            return 0;
        }
        else if (strcmp(argv[1], "cos") == 0) {
            printf("cos(%g) = %g\n", a, cos(a));
            return 0;
        }
        else if (strcmp(argv[1], "tan") == 0) {
            printf("tan(%g) = %g\n", a, tan(a));
            return 0;
        }
        else if (strcmp(argv[1], "fact") == 0) {
            if (!is_integer(argv[2])) {
                printf("Error: Factorial requires an integer\n");
                return 1;
            }
            if (a < 0) {
                printf("Error: Factorial undefined for negative numbers\n");
                return 1;
            }
            if (a > 20) {
                printf("Error: Factorial overflow (max 20! = 2.43e18)\n");
                return 1;
            }
            printf("%d! = %llu\n", int_a, factorial(int_a));
            return 0;
        }
        else {
            printf("Error: Unknown command '%s'\n", argv[1]);
            printf("Use '%s help' for available commands\n", argv[0]);
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
    has_error = false;
    double result = calculate_binary(a, b, op);
    
    if (!has_error) {
        // Format output nicely
        if (result == (int)result) {
            printf("%g\n", result);
        } else {
            printf("%.10g\n", result);
        }
    } else {
        return 1;
    }
    
    return 0;
}
