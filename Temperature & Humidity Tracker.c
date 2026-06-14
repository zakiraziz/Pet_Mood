#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int hour;
    float temperature;
    float humidity;
} Reading;

float generate_temp() {
    return 15.0 + (rand() % 200) / 10.0; // 15-35 degrees
}

float generate_humidity() {
    return 30.0 + (rand() % 700) / 10.0; // 30-100%
}

void save_to_csv(Reading readings[], int count) {
    FILE *fp = fopen("weather_data.csv", "w");
    if (!fp) {
        printf("Error creating CSV file!\n");
        return;
    }
    
    fprintf(fp, "Hour,Temperature (C),Humidity (%%)\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%.1f,%.1f\n", readings[i].hour, readings[i].temperature, readings[i].humidity);
    }
    fclose(fp);
    printf("Data saved to weather_data.csv\n");
}

void calculate_stats(Reading readings[], int count) {
    float max_temp = readings[0].temperature, min_temp = readings[0].temperature;
    float max_hum = readings[0].humidity, min_hum = readings[0].humidity;
    float sum_temp = 0, sum_hum = 0;
    
    for (int i = 0; i < count; i++) {
        sum_temp += readings[i].temperature;
        sum_hum += readings[i].humidity;
        if (readings[i].temperature > max_temp) max_temp = readings[i].temperature;
        if (readings[i].temperature < min_temp) min_temp = readings[i].temperature;
        if (readings[i].humidity > max_hum) max_hum = readings[i].humidity;
        if (readings[i].humidity < min_hum) min_hum = readings[i].humidity;
    }
    
    printf("\n=== STATISTICS ===\n");
    printf("Temperature:\n");
    printf("  Average: %.1fC\n", sum_temp / count);
    printf("  Max: %.1fC\n", max_temp);
    printf("  Min: %.1fC\n", min_temp);
    printf("Humidity:\n");
    printf("  Average: %.1f%%\n", sum_hum / count);
    printf("  Max: %.1f%%\n", max_hum);
    printf("  Min: %.1f%%\n", min_hum);
}

int main() {
    srand(time(NULL));
    Reading readings[24];
    
    printf("=== WEATHER TRACKER ===\n");
    printf("Simulating 24 hours of data...\n\n");
    
    for (int hour = 0; hour < 24; hour++) {
        readings[hour].hour = hour;
        readings[hour].temperature = generate_temp();
        readings[hour].humidity = generate_humidity();
        
        printf("Hour %2d: %.1fC, %.1f%% humidity\n", 
               hour, readings[hour].temperature, readings[hour].humidity);
    }
    
    calculate_stats(readings, 24);
    save_to_csv(readings, 24);
    
    return 0;
}