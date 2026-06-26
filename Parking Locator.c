#include <stdio.h>
#include <string.h>

typedef struct {
    char level[20];
    int row;
    int spot;
} ParkingSpot;

ParkingSpot spot;
int saved = 0;

void save_spot() {
    printf("Enter parking level (A/B/C/D): ");
    getchar();
    fgets(spot.level, 20, stdin);
    spot.level[strcspn(spot.level, "\n")] = 0;
    
    printf("Enter row number: ");
    scanf("%d", &spot.row);
    printf("Enter spot number: ");
    scanf("%d", &spot.spot);
    saved = 1;
    printf("Parking spot saved!\n");
}

void find_spot() {
    if (!saved) {
        printf("You haven't saved your parking spot yet!\n");
        return;
    }
    printf("\n=== YOUR CAR IS HERE ===\n");
    printf("Level: %s\n", spot.level);
    printf("Row: %d\n", spot.row);
    printf("Spot: %d\n", spot.spot);
    printf("========================\n");
}

int main() {
    int choice;
    do {
        printf("\n1. Save Parking Spot\n2. Find My Car\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: save_spot(); break;
            case 2: find_spot(); break;
        }
    } while(choice != 3);
    return 0;
}