#include <stdio.h>
#include <string.h>

#define MAX 10

typedef struct {
    char location[50];
    int distance_from_current; // in km
} Destination;

Destination itinerary[MAX];
int dest_count = 0;
int current_index = 0;

void add_destination() {
    if (dest_count >= MAX) {
        printf("Itinerary full!\n");
        return;
    }
    printf("Enter destination name: ");
    getchar();
    fgets(itinerary[dest_count].location, 50, stdin);
    itinerary[dest_count].location[strcspn(itinerary[dest_count].location, "\n")] = 0;
    
    printf("Enter distance from previous (km): ");
    scanf("%d", &itinerary[dest_count].distance_from_current);
    dest_count++;
}

void show_next_step() {
    if (current_index >= dest_count) {
        printf("You've reached your final destination!\n");
        return;
    }
    printf("\n=== NEXT DESTINATION ===\n");
    printf("Go to: %s\n", itinerary[current_index].location);
    printf("Distance: %d km\n", itinerary[current_index].distance_from_current);
    printf("========================\n");
    current_index++;
}

int main() {
    int choice;
    do {
        printf("\n1. Add Destination\n2. Show Next Step\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: add_destination(); break;
            case 2: show_next_step(); break;
        }
    } while(choice != 3);
    return 0;
}