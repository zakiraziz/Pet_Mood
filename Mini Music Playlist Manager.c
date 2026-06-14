#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char title[100];
    char artist[100];
    struct Song *next;
    struct Song *prev;
} Song;

Song *head = NULL;
Song *current = NULL;

void add_song() {
    Song *new_song = (Song*)malloc(sizeof(Song));
    if (!new_song) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    printf("Enter song title: ");
    getchar();
    fgets(new_song->title, 100, stdin);
    new_song->title[strcspn(new_song->title, "\n")] = 0;
    
    printf("Enter artist name: ");
    fgets(new_song->artist, 100, stdin);
    new_song->artist[strcspn(new_song->artist, "\n")] = 0;
    
    new_song->next = NULL;
    new_song->prev = NULL;
    
    if (!head) {
        head = new_song;
        current = new_song;
    } else {
        Song *temp = head;
        while (temp->next) temp = temp->next;
        temp->next = new_song;
        new_song->prev = temp;
    }
    printf("Song added!\n");
}

void display_playlist() {
    if (!head) {
        printf("Playlist is empty!\n");
        return;
    }
    
    Song *temp = head;
    int count = 1;
    printf("\n=== PLAYLIST ===\n");
    while (temp) {
        printf("%d. %s - %s\n", count++, temp->title, temp->artist);
        temp = temp->next;
    }
}

void play_current() {
    if (!current) {
        printf("No song selected!\n");
        return;
    }
    printf("\nNow Playing: %s by %s\n", current->title, current->artist);
}

void next_song() {
    if (current && current->next) {
        current = current->next;
        play_current();
    } else {
        printf("End of playlist!\n");
    }
}

void prev_song() {
    if (current && current->prev) {
        current = current->prev;
        play_current();
    } else {
        printf("Beginning of playlist!\n");
    }
}

int main() {
    int choice;
    do {
        printf("\n=== MUSIC PLAYLIST MANAGER ===\n");
        printf("1. Add Song\n2. View Playlist\n3. Play Current\n");
        printf("4. Next Song\n5. Previous Song\n6. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_song(); break;
            case 2: display_playlist(); break;
            case 3: play_current(); break;
            case 4: next_song(); break;
            case 5: prev_song(); break;
            case 6: printf("Goodbye!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 6);
    
    // Free memory
    while(head) {
        Song *temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}