#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROOMS 20
#define MAX_ITEMS 10
#define MAX_DESC 200

typedef struct {
    char name[50];
    char description[MAX_DESC];
    int exits[4]; // 0=north, 1=south, 2=east, 3=west
    int item;
} Room;

typedef struct {
    char name[50];
    char description[MAX_DESC];
    int weight;
} Item;

Room rooms[MAX_ROOMS];
Item items[MAX_ITEMS];
int current_room = 0;
int inventory[MAX_ITEMS];
int inventory_count = 0;
int game_running = 1;

// Game state
int has_sword = 0;
int has_key = 0;
int has_treasure = 0;

void init_game() {
    // Initialize rooms
    strcpy(rooms[0].name, "Entrance");
    strcpy(rooms[0].description, "You are at the entrance of a dark castle. A torch flickers on the wall.");
    rooms[0].exits[0] = 1; // North to Hall
    rooms[0].exits[3] = -1; // West exit (leave)
    rooms[0].item = -1;
    
    strcpy(rooms[1].name, "Great Hall");
    strcpy(rooms[1].description, "A grand hall with a chandelier. Dust covers the floor.");
    rooms[1].exits[1] = 0; // South to Entrance
    rooms[1].exits[2] = 2; // East to Kitchen
    rooms[1].exits[0] = 3; // North to Library
    rooms[1].item = 0; // Sword
    
    strcpy(rooms[2].name, "Kitchen");
    strcpy(rooms[2].description, "An old kitchen. Something is cooking in the pot.");
    rooms[2].exits[3] = 1; // West to Hall
    rooms[2].exits[0] = 4; // North to Dungeon
    rooms[2].item = 1; // Key
    
    strcpy(rooms[3].name, "Library");
    strcpy(rooms[3].description, "A dusty library with many books. A secret passage is visible.");
    rooms[3].exits[1] = 1; // South to Hall
    rooms[3].exits[2] = 4; // East to Dungeon
    rooms[3].item = -1;
    
    strcpy(rooms[4].name, "Dungeon");
    strcpy(rooms[4].description, "A dark dungeon. You hear dripping water and... breathing.");
    rooms[4].exits[1] = 2; // South to Kitchen
    rooms[4].exits[3] = 3; // West to Library
    rooms[4].item = 2; // Treasure
    
    // Initialize items
    strcpy(items[0].name, "Sword");
    strcpy(items[0].description, "A rusty but sharp sword.");
    items[0].weight = 3;
    
    strcpy(items[1].name, "Key");
    strcpy(items[1].description, "An old iron key.");
    items[1].weight = 1;
    
    strcpy(items[2].name, "Treasure");
    strcpy(items[2].description, "A chest full of gold!");
    items[2].weight = 5;
}

void show_room() {
    printf("\n=== %s ===\n", rooms[current_room].name);
    printf("%s\n", rooms[current_room].description);
    
    // Show item in room
    int item = rooms[current_room].item;
    if (item >= 0) {
        printf("\nYou see a %s here.\n", items[item].name);
    }
    
    // Show exits
    printf("\nExits: ");
    if (rooms[current_room].exits[0] >= 0) printf("[N]orth ");
    if (rooms[current_room].exits[1] >= 0) printf("[S]outh ");
    if (rooms[current_room].exits[2] >= 0) printf("[E]ast ");
    if (rooms[current_room].exits[3] >= 0) printf("[W]est ");
    if (current_room == 0) printf("[L]eave ");
    printf("\n");
    
    // Show inventory
    if (inventory_count > 0) {
        printf("\nInventory: ");
        for (int i = 0; i < inventory_count; i++) {
            printf("%s ", items[inventory[i]].name);
        }
        printf("\n");
    }
}

void handle_command(char* cmd) {
    cmd[strcspn(cmd, "\n")] = 0;
    
    // Convert to lowercase for comparison
    for (int i = 0; cmd[i]; i++) {
        cmd[i] = tolower(cmd[i]);
    }
    
    if (strcmp(cmd, "north") == 0 || strcmp(cmd, "n") == 0) {
        if (rooms[current_room].exits[0] >= 0) {
            current_room = rooms[current_room].exits[0];
        } else {
            printf("You can't go that way!\n");
        }
    }
    else if (strcmp(cmd, "south") == 0 || strcmp(cmd, "s") == 0) {
        if (rooms[current_room].exits[1] >= 0) {
            current_room = rooms[current_room].exits[1];
        } else {
            printf("You can't go that way!\n");
        }
    }
    else if (strcmp(cmd, "east") == 0 || strcmp(cmd, "e") == 0) {
        if (rooms[current_room].exits[2] >= 0) {
            current_room = rooms[current_room].exits[2];
        } else {
            printf("You can't go that way!\n");
        }
    }
    else if (strcmp(cmd, "west") == 0 || strcmp(cmd, "w") == 0) {
        if (rooms[current_room].exits[3] >= 0) {
            current_room = rooms[current_room].exits[3];
        } else {
            printf("You can't go that way!\n");
        }
    }
    else if (strcmp(cmd, "take") == 0 || strcmp(cmd, "get") == 0) {
        int item = rooms[current_room].item;
        if (item >= 0) {
            if (inventory_count < MAX_ITEMS) {
                inventory[inventory_count++] = item;
                rooms[current_room].item = -1;
                printf("You took the %s.\n", items[item].name);
                
                // Check for special items
                if (strcmp(items[item].name, "Sword") == 0) has_sword = 1;
                if (strcmp(items[item].name, "Key") == 0) has_key = 1;
                if (strcmp(items[item].name, "Treasure") == 0) {
                    has_treasure = 1;
                    printf("\n*** You found the treasure! ***\n");
                    printf("*** Congratulations! You win! ***\n");
                    game_running = 0;
                }
            } else {
                printf("Your inventory is full!\n");
            }
        } else {
            printf("There's nothing to take here.\n");
        }
    }
    else if (strcmp(cmd, "inventory") == 0 || strcmp(cmd, "i") == 0) {
        if (inventory_count == 0) {
            printf("You're not carrying anything.\n");
        } else {
            printf("You are carrying:\n");
            for (int i = 0; i < inventory_count; i++) {
                printf("  - %s\n", items[inventory[i]].name);
            }
        }
    }
    else if (strcmp(cmd, "leave") == 0 || strcmp(cmd, "l") == 0) {
        if (current_room == 0) {
            printf("\nYou leave the castle. Game over!\n");
            game_running = 0;
        } else {
            printf("You must be at the entrance to leave.\n");
        }
    }
    else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "h") == 0) {
        printf("\nCommands: north/south/east/west, take/get, inventory/i, leave, help, quit\n");
    }
    else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
        printf("Thanks for playing!\n");
        game_running = 0;
    }
    else {
        printf("I don't understand that command. Type 'help' for commands.\n");
    }
}

int main() {
    char command[100];
    
    printf("\n===================================\n");
    printf("  CASTLE ADVENTURE\n");
    printf("  Find the treasure and escape!\n");
    printf("===================================\n");
    printf("Type 'help' for commands.\n");
    
    init_game();
    
    while (game_running) {
        show_room();
        printf("\n> ");
        fgets(command, 100, stdin);
        handle_command(command);
    }
    
    return 0;
}