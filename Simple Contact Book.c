
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define PHONE_LEN 15

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

void addContact() {
    if(contactCount >= MAX_CONTACTS) {
        printf("\nContact book is full!\n");
        return;
    }
    
    printf("\n--- Add New Contact ---\n");
    printf("Enter name: ");
    scanf(" %[^\n]", contacts[contactCount].name);
    printf("Enter phone number: ");
    scanf("%s", contacts[contactCount].phone);
    
    contactCount++;
    printf("\nContact added successfully!\n");
}

void viewContacts() {
    if(contactCount == 0) {
        printf("\nNo contacts found!\n");
        return;
    }
    
    printf("\n========== CONTACT LIST ==========\n");
    printf("%-3s %-20s %-15s\n", "No.", "Name", "Phone");
    printf("-----------------------------------\n");
    
    for(int i = 0; i < contactCount; i++) {
        printf("%-3d %-20s %-15s\n", i+1, contacts[i].name, contacts[i].phone);
    }
    printf("===================================\n");
}

void searchContact() {
    char searchName[NAME_LEN];
    int found = 0;
    
    printf("\nEnter name to search: ");
    scanf(" %[^\n]", searchName);
    
    printf("\n--- Search Results ---\n");
    for(int i = 0; i < contactCount; i++) {
        if(strstr(contacts[i].name, searchName) != NULL) {
            printf("Name: %s, Phone: %s\n", contacts[i].name, contacts[i].phone);
            found = 1;
        }
    }
    
    if(!found) {
        printf("No contact found with name '%s'\n", searchName);
    }
}

void deleteContact() {
    char deleteName[NAME_LEN];
    int found = 0;
    
    printf("\nEnter name to delete: ");
    scanf(" %[^\n]", deleteName);
    
    for(int i = 0; i < contactCount; i++) {
        if(strcmp(contacts[i].name, deleteName) == 0) {
            // Shift all contacts after i one position left
            for(int j = i; j < contactCount - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            contactCount--;
            found = 1;
            printf("\nContact deleted successfully!\n");
            break;
        }
    }
    
    if(!found) {
        printf("\nContact not found!\n");
    }
}

int main() {
    int choice;
    
    do {
        printf("\n========== CONTACT BOOK ==========\n");
        printf("1. Add Contact\n");
        printf("2. View All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("===================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: printf("\nGoodbye!\n"); break;
            default: printf("\nInvalid choice! Try again.\n");
        }
    } while(choice != 5);
    
    return 0;
}