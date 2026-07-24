#include <stdio.h>

float celsius_to_fahrenheit(float c) {
    return (c * 9/5) + 32;
}

float fahrenheit_to_celsius(float f) {
    return (f - 32) * 5/9;
}

float celsius_to_kelvin(float c) {
    return c + 273.15;
}

float kelvin_to_celsius(float k) {
    return k - 273.15;
}

int main() {
    int choice;
    float temp, result;
    
    printf("\n==================================\n");
    printf("   🌡️ TEMPERATURE CONVERTER\n");
    printf("==================================\n");
    
    while (1) {
        printf("\n[1] Celsius → Fahrenheit\n");
        printf("[2] Fahrenheit → Celsius\n");
        printf("[3] Celsius → Kelvin\n");
        printf("[4] Kelvin → Celsius\n");
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
        
        printf("Enter temperature: ");
        scanf("%f", &temp);
        
        switch(choice) {
            case 1:
                result = celsius_to_fahrenheit(temp);
                printf("✅ %.2f°C = %.2f°F\n", temp, result);
                break;
            case 2:
                result = fahrenheit_to_celsius(temp);
                printf("✅ %.2f°F = %.2f°C\n", temp, result);
                break;
            case 3:
                result = celsius_to_kelvin(temp);
                printf("✅ %.2f°C = %.2f K\n", temp, result);
                break;
            case 4:
                result = kelvin_to_celsius(temp);
                printf("✅ %.2f K = %.2f°C\n", temp, result);
                break;
        }
        
        // Show common temperature references
        printf("\n📊 Reference: Water freezes at 0°C (32°F, 273.15K)\n");
        printf("📊 Reference: Water boils at 100°C (212°F, 373.15K)\n");
    }
    return 0;
}