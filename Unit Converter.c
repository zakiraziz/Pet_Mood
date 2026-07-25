#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Conversion functions
double celsius_to_fahrenheit(double c) { return (c * 9/5) + 32; }
double fahrenheit_to_celsius(double f) { return (f - 32) * 5/9; }
double celsius_to_kelvin(double c) { return c + 273.15; }
double kelvin_to_celsius(double k) { return k - 273.15; }

double meters_to_feet(double m) { return m * 3.28084; }
double feet_to_meters(double ft) { return ft / 3.28084; }
double meters_to_inches(double m) { return m * 39.3701; }
double inches_to_meters(double in) { return in / 39.3701; }

double kg_to_lbs(double kg) { return kg * 2.20462; }
double lbs_to_kg(double lbs) { return lbs / 2.20462; }
double kg_to_oz(double kg) { return kg * 35.274; }
double oz_to_kg(double oz) { return oz / 35.274; }

// Currency rates (example - update these)
double usd_to_eur = 0.92;
double usd_to_gbp = 0.79;
double usd_to_jpy = 149.50;
double usd_to_inr = 83.12;

void temperature_menu() {
    int choice;
    double value, result;
    
    printf("\n=== TEMPERATURE CONVERTER ===\n");
    printf("1. Celsius to Fahrenheit\n");
    printf("2. Fahrenheit to Celsius\n");
    printf("3. Celsius to Kelvin\n");
    printf("4. Kelvin to Celsius\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    printf("Enter value: ");
    scanf("%lf", &value);
    
    switch(choice) {
        case 1: result = celsius_to_fahrenheit(value); 
                printf("%.2f°C = %.2f°F\n", value, result); break;
        case 2: result = fahrenheit_to_celsius(value);
                printf("%.2f°F = %.2f°C\n", value, result); break;
        case 3: result = celsius_to_kelvin(value);
                printf("%.2f°C = %.2fK\n", value, result); break;
        case 4: result = kelvin_to_celsius(value);
                printf("%.2fK = %.2f°C\n", value, result); break;
        default: printf("Invalid choice!\n");
    }
}

void length_menu() {
    int choice;
    double value, result;
    
    printf("\n=== LENGTH CONVERTER ===\n");
    printf("1. Meters to Feet\n");
    printf("2. Feet to Meters\n");
    printf("3. Meters to Inches\n");
    printf("4. Inches to Meters\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    printf("Enter value: ");
    scanf("%lf", &value);
    
    switch(choice) {
        case 1: result = meters_to_feet(value);
                printf("%.2f m = %.2f ft\n", value, result); break;
        case 2: result = feet_to_meters(value);
                printf("%.2f ft = %.2f m\n", value, result); break;
        case 3: result = meters_to_inches(value);
                printf("%.2f m = %.2f in\n", value, result); break;
        case 4: result = inches_to_meters(value);
                printf("%.2f in = %.2f m\n", value, result); break;
        default: printf("Invalid choice!\n");
    }
}

void weight_menu() {
    int choice;
    double value, result;
    
    printf("\n=== WEIGHT CONVERTER ===\n");
    printf("1. Kilograms to Pounds\n");
    printf("2. Pounds to Kilograms\n");
    printf("3. Kilograms to Ounces\n");
    printf("4. Ounces to Kilograms\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    printf("Enter value: ");
    scanf("%lf", &value);
    
    switch(choice) {
        case 1: result = kg_to_lbs(value);
                printf("%.2f kg = %.2f lbs\n", value, result); break;
        case 2: result = lbs_to_kg(value);
                printf("%.2f lbs = %.2f kg\n", value, result); break;
        case 3: result = kg_to_oz(value);
                printf("%.2f kg = %.2f oz\n", value, result); break;
        case 4: result = oz_to_kg(value);
                printf("%.2f oz = %.2f kg\n", value, result); break;
        default: printf("Invalid choice!\n");
    }
}

void currency_menu() {
    int choice;
    double value, result;
    
    printf("\n=== CURRENCY CONVERTER (USD Base) ===\n");
    printf("1. USD to EUR (%.2f)\n", usd_to_eur);
    printf("2. USD to GBP (%.2f)\n", usd_to_gbp);
    printf("3. USD to JPY (%.2f)\n", usd_to_jpy);
    printf("4. USD to INR (%.2f)\n", usd_to_inr);
    printf("5. EUR to USD\n");
    printf("6. GBP to USD\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    printf("Enter amount: ");
    scanf("%lf", &value);
    
    switch(choice) {
        case 1: result = value * usd_to_eur;
                printf("$%.2f = €%.2f\n", value, result); break;
        case 2: result = value * usd_to_gbp;
                printf("$%.2f = £%.2f\n", value, result); break;
        case 3: result = value * usd_to_jpy;
                printf("$%.2f = ¥%.2f\n", value, result); break;
        case 4: result = value * usd_to_inr;
                printf("$%.2f = ₹%.2f\n", value, result); break;
        case 5: result = value / usd_to_eur;
                printf("€%.2f = $%.2f\n", value, result); break;
        case 6: result = value / usd_to_gbp;
                printf("£%.2f = $%.2f\n", value, result); break;
        default: printf("Invalid choice!\n");
    }
}

int main() {
    int choice;
    
    while(1) {
        printf("\n===== UNIT CONVERTER =====\n");
        printf("1. Temperature\n");
        printf("2. Length\n");
        printf("3. Weight\n");
        printf("4. Currency\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: temperature_menu(); break;
            case 2: length_menu(); break;
            case 3: weight_menu(); break;
            case 4: currency_menu(); break;
            case 5: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}