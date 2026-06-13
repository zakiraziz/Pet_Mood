#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct {
    int hour;
    float temperature;
    float humidity;
} Reading;

// Function prototypes
float generate_temp();
float generate_humidity();
void save_to_csv(Reading readings[], int count);
void calculate_stats(Reading readings[], int count);
void find_extreme_hours(Reading readings[], int count);
void generate_weather_summary(Reading readings[], int count);
void export_to_html(Reading readings[], int count);
void analyze_trends(Reading readings[], int count);
void add_manual_entry(Reading readings[], int *count);

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
    printf("✓ Data saved to weather_data.csv\n");
}

void calculate_stats(Reading readings[], int count) {
    float max_temp = readings[0].temperature, min_temp = readings[0].temperature;
    float max_hum = readings[0].humidity, min_hum = readings[0].humidity;
    float sum_temp = 0, sum_hum = 0;
    float temp_variance = 0, hum_variance = 0;
    
    for (int i = 0; i < count; i++) {
        sum_temp += readings[i].temperature;
        sum_hum += readings[i].humidity;
        if (readings[i].temperature > max_temp) max_temp = readings[i].temperature;
        if (readings[i].temperature < min_temp) min_temp = readings[i].temperature;
        if (readings[i].humidity > max_hum) max_hum = readings[i].humidity;
        if (readings[i].humidity < min_hum) min_hum = readings[i].humidity;
    }
    
    float avg_temp = sum_temp / count;
    float avg_hum = sum_hum / count;
    
    // Calculate standard deviation
    for (int i = 0; i < count; i++) {
        temp_variance += pow(readings[i].temperature - avg_temp, 2);
        hum_variance += pow(readings[i].humidity - avg_hum, 2);
    }
    float temp_std = sqrt(temp_variance / count);
    float hum_std = sqrt(hum_variance / count);
    
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║         STATISTICS               ║\n");
    printf("╚═══════════════════════════════════╝\n");
    printf("Temperature:\n");
    printf("  Average: %.1f°C\n", avg_temp);
    printf("  Max: %.1f°C\n", max_temp);
    printf("  Min: %.1f°C\n", min_temp);
    printf("  Std Dev: %.2f°C\n", temp_std);
    printf("  Range: %.1f°C\n", max_temp - min_temp);
    printf("\nHumidity:\n");
    printf("  Average: %.1f%%\n", avg_hum);
    printf("  Max: %.1f%%\n", max_hum);
    printf("  Min: %.1f%%\n", min_hum);
    printf("  Std Dev: %.2f%%\n", hum_std);
    printf("  Range: %.1f%%\n", max_hum - min_hum);
}

void find_extreme_hours(Reading readings[], int count) {
    int hottest_hour = 0, coldest_hour = 0;
    int most_humid_hour = 0, least_humid_hour = 0;
    
    for (int i = 1; i < count; i++) {
        if (readings[i].temperature > readings[hottest_hour].temperature) 
            hottest_hour = i;
        if (readings[i].temperature < readings[coldest_hour].temperature) 
            coldest_hour = i;
        if (readings[i].humidity > readings[most_humid_hour].humidity) 
            most_humid_hour = i;
        if (readings[i].humidity < readings[least_humid_hour].humidity) 
            least_humid_hour = i;
    }
    
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║         EXTREME HOURS            ║\n");
    printf("╚═══════════════════════════════════╝\n");
    printf("Hottest: Hour %d (%.1f°C)\n", readings[hottest_hour].hour, readings[hottest_hour].temperature);
    printf("Coldest: Hour %d (%.1f°C)\n", readings[coldest_hour].hour, readings[coldest_hour].temperature);
    printf("Most Humid: Hour %d (%.1f%%)\n", readings[most_humid_hour].hour, readings[most_humid_hour].humidity);
    printf("Least Humid: Hour %d (%.1f%%)\n", readings[least_humid_hour].hour, readings[least_humid_hour].humidity);
}

void generate_weather_summary(Reading readings[], int count) {
    int hot_count = 0, cold_count = 0, humid_count = 0, dry_count = 0;
    float avg_temp = 0, avg_hum = 0;
    
    for (int i = 0; i < count; i++) {
        avg_temp += readings[i].temperature;
        avg_hum += readings[i].humidity;
        if (readings[i].temperature > 25) hot_count++;
        if (readings[i].temperature < 18) cold_count++;
        if (readings[i].humidity > 70) humid_count++;
        if (readings[i].humidity < 45) dry_count++;
    }
    avg_temp /= count;
    avg_hum /= count;
    
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║         WEATHER SUMMARY          ║\n");
    printf("╚═══════════════════════════════════╝\n");
    printf("Day Type: ");
    if (avg_temp > 25) printf("Hot Day 🌞\n");
    else if (avg_temp > 20) printf("Warm Day 🌤\n");
    else if (avg_temp > 15) printf("Mild Day ⛅\n");
    else printf("Cool Day 🌥\n");
    
    printf("Overall: ");
    if (avg_hum > 70) printf("Humid 💧\n");
    else if (avg_hum > 50) printf("Moderate 🌊\n");
    else printf("Dry 🏜\n");
    
    printf("\nConditions:\n");
    printf("  Hot hours (>25°C): %d\n", hot_count);
    printf("  Cold hours (<18°C): %d\n", cold_count);
    printf("  Humid hours (>70%%): %d\n", humid_count);
    printf("  Dry hours (<45%%): %d\n", dry_count);
}

void export_to_html(Reading readings[], int count) {
    FILE *fp = fopen("weather_report.html", "w");
    if (!fp) {
        printf("Error creating HTML file!\n");
        return;
    }
    
    fprintf(fp, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(fp, "<title>Weather Report</title>\n");
    fprintf(fp, "<style>\n");
    fprintf(fp, "  body { font-family: Arial; margin: 40px; }\n");
    fprintf(fp, "  table { border-collapse: collapse; width: 100%%; }\n");
    fprintf(fp, "  th, td { border: 1px solid #ddd; padding: 8px; text-align: center; }\n");
    fprintf(fp, "  th { background-color: #4CAF50; color: white; }\n");
    fprintf(fp, "  tr:nth-child(even) { background-color: #f2f2f2; }\n");
    fprintf(fp, "  .hot { color: red; font-weight: bold; }\n");
    fprintf(fp, "  .cold { color: blue; font-weight: bold; }\n");
    fprintf(fp, "  .humid { color: #0066cc; font-weight: bold; }\n");
    fprintf(fp, "</style>\n</head>\n<body>\n");
    fprintf(fp, "<h1>📊 24-Hour Weather Report</h1>\n");
    fprintf(fp, "<table>\n<tr><th>Hour</th><th>Temperature (°C)</th><th>Humidity (%%%)</th></tr>\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(fp, "<tr><td>%d:00</td>", readings[i].hour);
        if (readings[i].temperature > 25)
            fprintf(fp, "<td class='hot'>%.1f°C</td>", readings[i].temperature);
        else if (readings[i].temperature < 18)
            fprintf(fp, "<td class='cold'>%.1f°C</td>", readings[i].temperature);
        else
            fprintf(fp, "<td>%.1f°C</td>", readings[i].temperature);
            
        if (readings[i].humidity > 70)
            fprintf(fp, "<td class='humid'>%.1f%%</td></tr>\n", readings[i].humidity);
        else
            fprintf(fp, "<td>%.1f%%</td></tr>\n", readings[i].humidity);
    }
    
    fprintf(fp, "</table>\n</body>\n</html>");
    fclose(fp);
    printf("✓ HTML report saved to weather_report.html\n");
}

void analyze_trends(Reading readings[], int count) {
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║         TREND ANALYSIS           ║\n");
    printf("╚═══════════════════════════════════╝\n");
    
    // Temperature trends
    int temp_up = 0, temp_down = 0;
    for (int i = 1; i < count; i++) {
        if (readings[i].temperature > readings[i-1].temperature) temp_up++;
        else if (readings[i].temperature < readings[i-1].temperature) temp_down++;
    }
    
    printf("Temperature Trend: ");
    if (temp_up > temp_down * 1.5) printf("Rising 🔥\n");
    else if (temp_down > temp_up * 1.5) printf("Falling ❄️\n");
    else printf("Stable ⚖️\n");
    
    // Humidity trends
    int hum_up = 0, hum_down = 0;
    for (int i = 1; i < count; i++) {
        if (readings[i].humidity > readings[i-1].humidity) hum_up++;
        else if (readings[i].humidity < readings[i-1].humidity) hum_down++;
    }
    
    printf("Humidity Trend: ");
    if (hum_up > hum_down * 1.5) printf("Increasing 💦\n");
    else if (hum_down > hum_up * 1.5) printf("Decreasing 🏜\n");
    else printf("Stable ⚖️\n");
    
    // Morning vs Evening comparison
    float morning_temp = 0, evening_temp = 0;
    float morning_hum = 0, evening_hum = 0;
    for (int i = 0; i < 12; i++) {
        morning_temp += readings[i].temperature;
        morning_hum += readings[i].humidity;
    }
    for (int i = 12; i < 24; i++) {
        evening_temp += readings[i].temperature;
        evening_hum += readings[i].humidity;
    }
    morning_temp /= 12;
    morning_hum /= 12;
    evening_temp /= 12;
    evening_hum /= 12;
    
    printf("\nComparison (AM vs PM):\n");
    printf("  Morning Avg Temp: %.1f°C\n", morning_temp);
    printf("  Evening Avg Temp: %.1f°C\n", evening_temp);
    printf("  Morning Avg Hum: %.1f%%\n", morning_hum);
    printf("  Evening Avg Hum: %.1f%%\n", evening_hum);
}

void add_manual_entry(Reading readings[], int *count) {
    if (*count >= 24) {
        printf("Cannot add more entries. Maximum 24 hours.\n");
        return;
    }
    
    Reading new_reading;
    printf("\n📝 Manual Entry\n");
    printf("Enter hour (0-23): ");
    scanf("%d", &new_reading.hour);
    
    // Check if hour already exists
    for (int i = 0; i < *count; i++) {
        if (readings[i].hour == new_reading.hour) {
            printf("Hour %d already exists. Updating...\n", new_reading.hour);
            printf("Enter temperature (°C): ");
            scanf("%f", &readings[i].temperature);
            printf("Enter humidity (%%): ");
            scanf("%f", &readings[i].humidity);
            return;
        }
    }
    
    printf("Enter temperature (°C): ");
    scanf("%f", &new_reading.temperature);
    printf("Enter humidity (%%): ");
    scanf("%f", &new_reading.humidity);
    
    readings[*count] = new_reading;
    (*count)++;
    printf("✓ Entry added successfully!\n");
}

int main() {
    srand(time(NULL));
    Reading readings[24];
    int count = 24;
    int choice;
    
    printf("╔═══════════════════════════════════╗\n");
    printf("║      WEATHER TRACKER v2.0        ║\n");
    printf("╚═══════════════════════════════════╝\n\n");
    
    // Generate initial data
    printf("Generating 24 hours of weather data...\n");
    for (int hour = 0; hour < 24; hour++) {
        readings[hour].hour = hour;
        readings[hour].temperature = generate_temp();
        readings[hour].humidity = generate_humidity();
    }
    printf("✓ Data generated!\n\n");
    
    do {
        printf("\n╔═══════════════════════════════════╗\n");
        printf("║        MAIN MENU                ║\n");
        printf("╚═══════════════════════════════════╝\n");
        printf("1. 📊 Display All Data\n");
        printf("2. 📈 Show Statistics\n");
        printf("3. 🔥 Find Extreme Hours\n");
        printf("4. 🌤 Weather Summary\n");
        printf("5. 📉 Trend Analysis\n");
        printf("6. 💾 Export to CSV\n");
        printf("7. 🌐 Export to HTML\n");
        printf("8. ✏️  Add/Edit Manual Entry\n");
        printf("9. 🔄 Regenerate Data\n");
        printf("0. 🚪 Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("\n=== WEATHER DATA ===\n");
                for (int i = 0; i < count; i++) {
                    printf("Hour %2d: %.1f°C, %.1f%% humidity\n", 
                           readings[i].hour, readings[i].temperature, readings[i].humidity);
                }
                break;
            case 2:
                calculate_stats(readings, count);
                break;
            case 3:
                find_extreme_hours(readings, count);
                break;
            case 4:
                generate_weather_summary(readings, count);
                break;
            case 5:
                analyze_trends(readings, count);
                break;
            case 6:
                save_to_csv(readings, count);
                break;
            case 7:
                export_to_html(readings, count);
                break;
            case 8:
                add_manual_entry(readings, &count);
                break;
            case 9:
                for (int i = 0; i < count; i++) {
                    readings[i].temperature = generate_temp();
                    readings[i].humidity = generate_humidity();
                }
                printf("✓ Data regenerated!\n");
                break;
            case 0:
                printf("\nThank you for using Weather Tracker! 🌤\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);
    
    return 0;
}