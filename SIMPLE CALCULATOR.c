#include <stdio.h>

float add(float a, float b) { return a + b; }
float subtract(float a, float b) { return a - b; }
float multiply(float a, float b) { return a * b; }
float divide(float a, float b) { 
    if (b == 0) {
        printf("❌ Error: Division by zero!\n");
        return 0;
    }
    return a / b; 
}

int main() {
    int choice;
    float num1, num2, result;
    
    printf("\n==================================\n");
    printf("   🧮 SIMPLE CALCULATOR\n");
    printf("==================================\n");
    
    while (1) {
        printf("\n[1] Add\n");
        printf("[2] Subtract\n");
        printf("[3] Multiply\n");
        printf("[4] Divide\n");
        printf("[5] Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        if (choice == 5) {
            printf("\n👋 Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > 5) {
            printf("❌ Invalid choice!\n");
            continue;
        }
        
        printf("Enter first number: ");
        scanf("%f", &num1);
        printf("Enter second number: ");
        scanf("%f", &num2);
        
        switch(choice) {
            case 1: 
                result = add(num1, num2);
                printf("✅ %.2f + %.2f = %.2f\n", num1, num2, result);
                break;
            case 2: 
                result = subtract(num1, num2);
                printf("✅ %.2f - %.2f = %.2f\n", num1, num2, result);
                break;
            case 3: 
                result = multiply(num1, num2);
                printf("✅ %.2f × %.2f = %.2f\n", num1, num2, result);
                break;
            case 4: 
                result = divide(num1, num2);
                if (num2 != 0)
                    printf("✅ %.2f ÷ %.2f = %.2f\n", num1, num2, result);
                break;
        }
    }
    return 0;
}