#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_SPOTS 100
#define MAX_HISTORY 50

typedef struct {
    char level[20];
    int row;
    int spot;
    char notes[100];
    time_t timestamp;
    int floor;          // For multi-level buildings
    char building[50];  // For multi-building campuses
} ParkingSpot;

typedef struct {
    ParkingSpot spot;
    char action[20];    // "saved" or "retrieved"
    time_t timestamp;
} HistoryEntry;

ParkingSpot current_spot;
int saved = 0;
ParkingSpot saved_spots[MAX_SPOTS];
int spot_count = 0;
HistoryEntry history[MAX_HISTORY];
int history_count = 0;
int auto_save_enabled = 1;

void add_history(const char *action, ParkingSpot *spot) {
    if (history_count < MAX_HISTORY) {
        strcpy(history[history_count].action, action);
        history[history_count].spot = *spot;
        history[history_count].timestamp = time(NULL);
        history_count++;
    }
}

void display_time(time_t t) {
    struct tm *tm_info = localtime(&t);
    char buffer[30];
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s", buffer);
}

int is_valid_level(const char *level) {
    // Check if level is A-Z or 1-9
    if (strlen(level) == 1) {
        char c = toupper(level[0]);
        if ((c >= 'A' && c <= 'Z') || (c >= '1' && c <= '9')) {
            return 1;
        }
    }
    return 0;
}

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

void save_spot() {
    ParkingSpot new_spot;
    
    printf("\n--- SAVE PARKING SPOT ---\n");
    
    // Building name
    printf("Building (optional, press Enter to skip): ");
    getchar();
    fgets(new_spot.building, 50, stdin);
    new_spot.building[strcspn(new_spot.building, "\n")] = 0;
    
    // Floor number
    printf("Floor number (0 for ground): ");
    scanf("%d", &new_spot.floor);
    
    // Level
    printf("Enter parking level (A/B/C/D or 1/2/3): ");
    getchar();
    fgets(new_spot.level, 20, stdin);
    new_spot.level[strcspn(new_spot.level, "\n")] = 0;
    to_uppercase(new_spot.level);
    
    // Validate level
    if (!is_valid_level(new_spot.level)) {
        printf("Warning: Unusual level format. Use single letter or number.\n");
    }
    
    // Row
    printf("Enter row number: ");
    scanf("%d", &new_spot.row);
    if (new_spot.row < 0) {
        printf("Invalid row! Setting to 0.\n");
        new_spot.row = 0;
    }
    
    // Spot
    printf("Enter spot number: ");
    scanf("%d", &new_spot.spot);
    if (new_spot.spot < 0) {
        printf("Invalid spot! Setting to 0.\n");
        new_spot.spot = 0;
    }
    
    // Notes
    printf("Notes (optional, press Enter to skip): ");
    getchar();
    fgets(new_spot.notes, 100, stdin);
    new_spot.notes[strcspn(new_spot.notes, "\n")] = 0;
    
    new_spot.timestamp = time(NULL);
    
    // Save as current spot
    current_spot = new_spot;
    saved = 1;
    
    // Save to history
    saved_spots[spot_count++] = new_spot;
    add_history("saved", &new_spot);
    
    printf("\n✓ Parking spot saved successfully!\n");
    printf("📍 Location: ");
    if (strlen(new_spot.building) > 0) printf("%s, ", new_spot.building);
    printf("Floor %d, Level %s, Row %d, Spot %d\n", 
           new_spot.floor, new_spot.level, new_spot.row, new_spot.spot);
    printf("📝 Notes: %s\n", new_spot.notes[0] ? new_spot.notes : "None");
    printf("🕐 Saved at: ");
    display_time(new_spot.timestamp);
    printf("\n");
}

void find_spot() {
    if (!saved) {
        printf("\n❌ You haven't saved your parking spot yet!\n");
        printf("Use option 1 to save your current location.\n");
        return;
    }
    
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║        🚗 YOUR CAR IS HERE 🚗        ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    
    if (strlen(current_spot.building) > 0) {
        printf("║ Building: %-30s ║\n", current_spot.building);
    }
    printf("║ Floor:    %-30d ║\n", current_spot.floor);
    printf("║ Level:    %-30s ║\n", current_spot.level);
    printf("║ Row:      %-30d ║\n", current_spot.row);
    printf("║ Spot:     %-30d ║\n", current_spot.spot);
    if (strlen(current_spot.notes) > 0) {
        printf("║ Notes:    %-30s ║\n", current_spot.notes);
    }
    printf("║ Saved:    ");
    display_time(current_spot.timestamp);
    printf(" ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    // Show nearby landmarks if notes exist
    if (strlen(current_spot.notes) > 0) {
        printf("\n📌 Remember: %s\n", current_spot.notes);
    }
}

void show_history() {
    if (history_count == 0) {
        printf("\nNo parking history yet.\n");
        return;
    }
    
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║           PARKING HISTORY                    ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    
    for (int i = history_count - 1; i >= 0 && i >= history_count - 10; i--) {
        printf("║ %s: ", history[i].action);
        if (strlen(history[i].spot.building) > 0) {
            printf("%s, ", history[i].spot.building);
        }
        printf("Floor %d, Level %s, Row %d, Spot %d",
               history[i].spot.floor, history[i].spot.level,
               history[i].spot.row, history[i].spot.spot);
        int len = strlen(history[i].spot.building) + 30;
        for (int j = len; j < 35; j++) printf(" ");
        printf("║\n");
        printf("║   🕐 ");
        display_time(history[i].timestamp);
        printf("                          ║\n");
    }
    printf("╚═══════════════════════════════════════════════╝\n");
}

void clear_history() {
    char confirm;
    printf("Clear all parking history? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        history_count = 0;
        spot_count = 0;
        printf("History cleared!\n");
    } else {
        printf("Operation cancelled.\n");
    }
}

void delete_current_spot() {
    char confirm;
    printf("Delete current parking spot? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        saved = 0;
        memset(&current_spot, 0, sizeof(ParkingSpot));
        printf("Spot deleted!\n");
    } else {
        printf("Operation cancelled.\n");
    }
}

void view_saved_spots() {
    if (spot_count == 0) {
        printf("\nNo saved spots found.\n");
        return;
    }
    
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║           SAVED PARKING SPOTS                ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < spot_count; i++) {
        printf("║ [%d] ", i+1);
        if (strlen(saved_spots[i].building) > 0) {
            printf("%s, ", saved_spots[i].building);
        }
        printf("Floor %d, Level %s, Row %d, Spot %d",
               saved_spots[i].floor, saved_spots[i].level,
               saved_spots[i].row, saved_spots[i].spot);
        int len = strlen(saved_spots[i].building) + 30;
        for (int j = len; j < 35; j++) printf(" ");
        printf("║\n");
    }
    printf("╚═══════════════════════════════════════════════╝\n");
}

void load_saved_spot() {
    if (spot_count == 0) {
        printf("No saved spots to load.\n");
        return;
    }
    
    view_saved_spots();
    int index;
    printf("Enter spot number to load (1-%d): ", spot_count);
    scanf("%d", &index);
    
    if (index < 1 || index > spot_count) {
        printf("Invalid spot number!\n");
        return;
    }
    
    current_spot = saved_spots[index - 1];
    saved = 1;
    add_history("loaded", &current_spot);
    printf("✓ Spot loaded successfully!\n");
}

void toggle_auto_save() {
    auto_save_enabled = !auto_save_enabled;
    printf("Auto-save is now %s\n", auto_save_enabled ? "ENABLED" : "DISABLED");
}

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║     🅿️  SMART PARKING FINDER v3.0   ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    printf("Never lose your car again!\n");
    
    int choice;
    do {
        printf("\n╔═══════════════════════════════════════╗\n");
        printf("║ 1. 🅿️  Save Parking Spot             ║\n");
        printf("║ 2. 🔍 Find My Car                   ║\n");
        printf("║ 3. 📋 View History                 ║\n");
        printf("║ 4. 💾 View Saved Spots             ║\n");
        printf("║ 5. 📂 Load Saved Spot              ║\n");
        printf("║ 6. 🗑️  Clear History               ║\n");
        printf("║ 7. ❌ Delete Current Spot          ║\n");
        printf("║ 8. ⚙️  Toggle Auto-Save            ║\n");
        printf("║ 9. 🚪 Exit                        ║\n");
        printf("╚═══════════════════════════════════════╝\n");
        printf("Choice: ");
        
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                save_spot();
                break;
            case 2:
                find_spot();
                break;
            case 3:
                show_history();
                break;
            case 4:
                view_saved_spots();
                break;
            case 5:
                load_saved_spot();
                break;
            case 6:
                clear_history();
                break;
            case 7:
                delete_current_spot();
                break;
            case 8:
                toggle_auto_save();
                break;
            case 9:
                printf("\n🚗 Safe travels! Don't forget where you parked!\n");
                break;
            default:
                printf("❌ Invalid choice! Please try again.\n");
        }
    } while(choice != 9);
    
    return 0;
}
