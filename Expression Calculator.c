#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 100

// Stack implementation
typedef struct {
    double items[MAX];
    int top;
} Stack;

void init_stack(Stack* s) { s->top = -1; }
int is_empty(Stack* s) { return s->top == -1; }
int is_full(Stack* s) { return s->top == MAX - 1; }

void push(Stack* s, double value) {
    if (!is_full(s)) s->items[++s->top] = value;
}

double pop(Stack* s) {
    if (!is_empty(s)) return s->items[s->top--];
    return 0;
}

double peek(Stack* s) {
    if (!is_empty(s)) return s->items[s->top];
    return 0;
}

// Operator precedence
int precedence(char op) {
    switch(op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// Apply operator
double apply_op(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

// Evaluate expression
double evaluate(const char* expr) {
    Stack values, ops;
    init_stack(&values);
    init_stack(&ops);
    
    int i = 0;
    while (expr[i]) {
        // Skip spaces
        if (expr[i] == ' ') { i++; continue; }
        
        // Handle numbers
        if (isdigit(expr[i]) || expr[i] == '.') {
            char num[50];
            int j = 0;
            while (isdigit(expr[i]) || expr[i] == '.') {
                num[j++] = expr[i++];
            }
            num[j] = '\0';
            push(&values, atof(num));
        }
        // Handle negative numbers
        else if (expr[i] == '-' && (i == 0 || expr[i-1] == '(')) {
            char num[50];
            int j = 0;
            num[j++] = '-';
            i++;
            while (isdigit(expr[i]) || expr[i] == '.') {
                num[j++] = expr[i++];
            }
            num[j] = '\0';
            push(&values, atof(num));
        }
        // Handle operators
        else if (strchr("+-*/^", expr[i])) {
            while (!is_empty(&ops) && precedence(peek(&ops)) >= precedence(expr[i])) {
                double b = pop(&values);
                double a = pop(&values);
                char op = (char)pop(&ops);
                push(&values, apply_op(a, b, op));
            }
            push(&ops, expr[i]);
            i++;
        }
        // Handle parentheses
        else if (expr[i] == '(') {
            push(&ops, expr[i]);
            i++;
        }
        else if (expr[i] == ')') {
            while (!is_empty(&ops) && peek(&ops) != '(') {
                double b = pop(&values);
                double a = pop(&values);
                char op = (char)pop(&ops);
                push(&values, apply_op(a, b, op));
            }
            if (!is_empty(&ops)) pop(&ops); // Remove '('
            i++;
        }
        else {
            printf("Invalid character: %c\n", expr[i]);
            return 0;
        }
    }
    
    // Apply remaining operators
    while (!is_empty(&ops)) {
        double b = pop(&values);
        double a = pop(&values);
        char op = (char)pop(&ops);
        push(&values, apply_op(a, b, op));
    }
    
    return pop(&values);
}

int main() {
    char expr[MAX];
    
    printf("=== EXPRESSION CALCULATOR ===\n");
    printf("Supported: + - * / ^ ( )\n");
    printf("Example: 2 + 3 * 4 = 14\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("> ");
        fgets(expr, MAX, stdin);
        expr[strcspn(expr, "\n")] = 0;
        
        if (strcmp(expr, "quit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        if (strlen(expr) == 0) continue;
        
        double result = evaluate(expr);
        printf("= %.6f\n", result);
    }
    
    return 0;
}