#include <stdio.h>

void convert_to_roman(int num) {
    if (num < 1 || num > 3999) {
        printf("Number out of range! Please enter 1-3999\n");
        return;
    }
    
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    
    printf("Roman numeral: ");
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            printf("%s", symbols[i]);
            num -= values[i];
        }
    }
    printf("\n");
}

int main() {
    int number;
    printf("=== ROMAN NUMERAL CONVERTER ===\n");
    printf("Enter a number (1-3999): ");
    scanf("%d", &number);
    convert_to_roman(number);
    
    // Bonus: Roman to decimal (simple version)
    printf("\nWant to convert Roman to Decimal? (y/n): ");
    char choice;
    getchar();
    scanf("%c", &choice);
    
    if (choice == 'y' || choice == 'Y') {
        char roman[20];
        printf("Enter Roman numeral: ");
        scanf("%s", roman);
        
        int result = 0;
        int values_r[] = {'I':1, 'V':5, 'X':10, 'L':50, 'C':100, 'D':500, 'M':1000};
        
        for (int i = 0; roman[i] != '\0'; i++) {
            int current = values_r[roman[i]];
            int next = values_r[roman[i+1]];
            if (current < next)
                result -= current;
            else
                result += current;
        }
        printf("Decimal value: %d\n", result);
    }
    return 0;
}