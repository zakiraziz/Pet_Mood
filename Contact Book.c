#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define MAX_PHONE 20
#define MAX_EMAIL 50
#define MAX_CONTACTS 100
#define FILENAME "contacts.csv"

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
} Contact;

Contact contacts[MAX_CONTACTS];
int contact_count = 0;

// Function prototypes
void load_contacts();
void save_contacts();
void add_contact();
void list_contacts();
void search_contact();
void delete_contact();
void sort_contacts();
int compare_contacts(const void* a, const void* b);
void clear_screen();
void press_any_key();

int main() {
    load_contacts();
    int choice;
    
    while (1) {
        clear_screen();
        printf("\n========== CONTACT BOOK ==========\n");
        printf(" 1. Add Contact\n");
        printf(" 2. List Contacts (%d)\n", contact_count);
        printf(" 3. Search Contact\n");
        printf(" 4. Delete Contact\n");
        printf(" 5. Sort Contacts\n");
        printf(" 6. Save & Exit\n");
        printf("==================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: add_contact(); break;
            case 2: list_contacts(); break;
            case 3: search_contact(); break;
            case 4: delete_contact(); break;
            case 5: sort_contacts(); break;
            case 6: save_contacts(); 
                    printf("\n✓ Contacts saved. Goodbye!\n");
                    return 0;
            default: printf("Invalid choice!\n");
        }
        press_any_key();
    }
    return 0;
}

void load_contacts() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) return;
    
    contact_count = 0;
    while (contact_count < MAX_CONTACTS && 
           fscanf(file, "%[^,],%[^,],%[^\n]\n", 
                  contacts[contact_count].name,
                  contacts[contact_count].phone,
                  contacts[contact_count].email) == 3) {
        contact_count++;
    }
    fclose(file);
}

void save_contacts() {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error: Cannot save contacts!\n");
        return;
    }
    
    for (int i = 0; i < contact_count; i++) {
        fprintf(file, "%s,%s,%s\n", 
                contacts[i].name,
                contacts[i].phone,
                contacts[i].email);
    }
    fclose(file);
}

void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Contact book is full!\n");
        return;
    }
    
    Contact* c = &contacts[contact_count];
    
    printf("\n--- Add New Contact ---\n");
    printf("Name: ");
    fgets(c->name, MAX_NAME, stdin);
    c->name[strcspn(c->name, "\n")] = 0;
    
    printf("Phone: ");
    fgets(c->phone, MAX_PHONE, stdin);
    c->phone[strcspn(c->phone, "\n")] = 0;
    
    printf("Email: ");
    fgets(c->email, MAX_EMAIL, stdin);
    c->email[strcspn(c->email, "\n")] = 0;
    
    contact_count++;
    printf("\n✓ Contact added successfully!\n");
}

void list_contacts() {
    if (contact_count == 0) {
        printf("\nNo contacts found!\n");
        return;
    }
    
    printf("\n%-3s %-20s %-15s %s\n", "#", "Name", "Phone", "Email");
    printf("--- %s %s %s\n", "--------------------", "---------------", "------------------");
    
    for (int i = 0; i < contact_count; i++) {
        printf("%-3d %-20s %-15s %s\n", 
               i+1,
               contacts[i].name,
               contacts[i].phone,
               contacts[i].email);
    }
}

void search_contact() {
    if (contact_count == 0) {
        printf("\nNo contacts to search!\n");
        return;
    }
    
    char query[MAX_NAME];
    printf("\nEnter name to search: ");
    fgets(query, MAX_NAME, stdin);
    query[strcspn(query, "\n")] = 0;
    
    int found = 0;
    printf("\n%-3s %-20s %-15s %s\n", "#", "Name", "Phone", "Email");
    printf("--- %s %s %s\n", "--------------------", "---------------", "------------------");
    
    for (int i = 0; i < contact_count; i++) {
        if (strcasestr(contacts[i].name, query)) {
            printf("%-3d %-20s %-15s %s\n", 
                   i+1,
                   contacts[i].name,
                   contacts[i].phone,
                   contacts[i].email);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No contacts found matching '%s'\n", query);
    }
}

void delete_contact() {
    if (contact_count == 0) {
        printf("\nNo contacts to delete!\n");
        return;
    }
    
    list_contacts();
    int index;
    printf("\nEnter contact number to delete (0 to cancel): ");
    scanf("%d", &index);
    getchar();
    
    if (index == 0) {
        printf("Operation cancelled.\n");
        return;
    }
    
    if (index < 1 || index > contact_count) {
        printf("Invalid contact number!\n");
        return;
    }
    
    index--; // Convert to 0-based
    printf("Delete '%s'? (y/n): ", contacts[index].name);
    char confirm = getchar();
    if (tolower(confirm) != 'y') {
        printf("Operation cancelled.\n");
        return;
    }
    
    // Shift contacts
    for (int i = index; i < contact_count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contact_count--;
    printf("\n✓ Contact deleted successfully!\n");
}

int compare_contacts(const void* a, const void* b) {
    Contact* c1 = (Contact*)a;
    Contact* c2 = (Contact*)b;
    return strcasecmp(c1->name, c2->name);
}

void sort_contacts() {
    if (contact_count == 0) {
        printf("\nNo contacts to sort!\n");
        return;
    }
    
    qsort(contacts, contact_count, sizeof(Contact), compare_contacts);
    printf("\n✓ Contacts sorted alphabetically!\n");
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void press_any_key() {
    printf("\nPress any key to continue...");
    getchar();
}