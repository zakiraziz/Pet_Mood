#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define PHONE_LEN 15
#define EMAIL_LEN 50
#define ADDRESS_LEN 100
#define GROUP_LEN 30

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char address[ADDRESS_LEN];
    char group[GROUP_LEN];
    char birthday[11];  // YYYY-MM-DD
    char notes[200];
    int favorite;       // 1 for favorite, 0 for not
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

// Helper function to trim whitespace
void trim(char *str) {
    int i = 0, j = strlen(str) - 1;
    while (isspace(str[i])) i++;
    while (j >= i && isspace(str[j])) j--;
    str[j + 1] = '\0';
    if (i > 0) {
        memmove(str, str + i, j - i + 2);
    }
}

// Validate phone number (basic check)
int validate_phone(const char *phone) {
    int digits = 0;
    for (int i = 0; phone[i]; i++) {
        if (isdigit(phone[i])) digits++;
    }
    return digits >= 7 && digits <= 15;
}

// Validate email (basic check)
int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot && (dot - at) > 1 && strlen(at + 1) > 1;
}

void addContact() {
    if(contactCount >= MAX_CONTACTS) {
        printf("\n❌ Contact book is full!\n");
        return;
    }
    
    Contact newContact;
    printf("\n--- Add New Contact ---\n");
    
    printf("Enter name: ");
    scanf(" %[^\n]", newContact.name);
    trim(newContact.name);
    
    printf("Enter phone number: ");
    scanf("%s", newContact.phone);
    trim(newContact.phone);
    while (!validate_phone(newContact.phone)) {
        printf("Invalid phone number! Enter at least 7 digits: ");
        scanf("%s", newContact.phone);
        trim(newContact.phone);
    }
    
    printf("Enter email (optional): ");
    scanf(" %[^\n]", newContact.email);
    trim(newContact.email);
    if (strlen(newContact.email) > 0) {
        while (!validate_email(newContact.email)) {
            printf("Invalid email format! Enter again (or press Enter to skip): ");
            scanf(" %[^\n]", newContact.email);
            trim(newContact.email);
            if (strlen(newContact.email) == 0) break;
        }
    }
    
    printf("Enter address (optional): ");
    scanf(" %[^\n]", newContact.address);
    trim(newContact.address);
    
    printf("Enter group (Family/Friends/Work/Other): ");
    scanf(" %[^\n]", newContact.group);
    trim(newContact.group);
    if (strlen(newContact.group) == 0) {
        strcpy(newContact.group, "General");
    }
    
    printf("Enter birthday (YYYY-MM-DD, optional): ");
    scanf(" %[^\n]", newContact.birthday);
    trim(newContact.birthday);
    
    printf("Enter notes (optional): ");
    scanf(" %[^\n]", newContact.notes);
    trim(newContact.notes);
    
    printf("Is this a favorite contact? (y/n): ");
    char fav;
    scanf(" %c", &fav);
    newContact.favorite = (fav == 'y' || fav == 'Y');
    
    contacts[contactCount++] = newContact;
    printf("\n✅ Contact added successfully!\n");
}

void viewContacts() {
    if(contactCount == 0) {
        printf("\n📭 No contacts found!\n");
        return;
    }
    
    printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                      CONTACT LIST                            ║\n");
    printf("╠════╦════════════════════╦═══════════════╦════════════════════╣\n");
    printf("║ No.║ Name               ║ Phone         ║ Group              ║\n");
    printf("╠════╬════════════════════╬═══════════════╬════════════════════╣\n");
    
    for(int i = 0; i < contactCount; i++) {
        printf("║ %-3d║ %-18s ║ %-13s ║ %-18s ║\n", 
               i+1, 
               contacts[i].name, 
               contacts[i].phone,
               contacts[i].group);
    }
    printf("╚════╩════════════════════╩═══════════════╩════════════════════╝\n");
    printf("Total contacts: %d\n", contactCount);
}

void viewContactDetails() {
    if(contactCount == 0) {
        printf("\n📭 No contacts found!\n");
        return;
    }
    
    char searchName[NAME_LEN];
    printf("\nEnter contact name to view details: ");
    scanf(" %[^\n]", searchName);
    trim(searchName);
    
    int found = 0;
    for(int i = 0; i < contactCount; i++) {
        if(strcasecmp(contacts[i].name, searchName) == 0 || 
           strstr(contacts[i].name, searchName) != NULL) {
            
            printf("\n╔════════════════════════════════════════════════════════╗\n");
            printf("║                    CONTACT DETAILS                    ║\n");
            printf("╠════════════════════════════════════════════════════════╣\n");
            printf("║ Name:     %-40s ║\n", contacts[i].name);
            printf("║ Phone:    %-40s ║\n", contacts[i].phone);
            if (strlen(contacts[i].email) > 0)
                printf("║ Email:    %-40s ║\n", contacts[i].email);
            if (strlen(contacts[i].address) > 0)
                printf("║ Address:  %-40s ║\n", contacts[i].address);
            printf("║ Group:    %-40s ║\n", contacts[i].group);
            if (strlen(contacts[i].birthday) > 0)
                printf("║ Birthday: %-40s ║\n", contacts[i].birthday);
            printf("║ Favorite: %-40s ║\n", contacts[i].favorite ? "⭐ Yes" : "No");
            if (strlen(contacts[i].notes) > 0)
                printf("║ Notes:    %-40s ║\n", contacts[i].notes);
            printf("╚════════════════════════════════════════════════════════╝\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("❌ Contact not found!\n");
    }
}

void searchContact() {
    char searchName[NAME_LEN];
    int found = 0;
    
    printf("\n🔍 Enter name to search: ");
    scanf(" %[^\n]", searchName);
    trim(searchName);
    
    printf("\n--- Search Results ---\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║ Name                 ║ Phone         ║ Group              ║\n");
    printf("╠══════════════════════╬═══════════════╬════════════════════╣\n");
    
    for(int i = 0; i < contactCount; i++) {
        if(strstr(contacts[i].name, searchName) != NULL || 
           strcasecmp(contacts[i].name, searchName) == 0) {
            printf("║ %-20s ║ %-13s ║ %-18s ║\n", 
                   contacts[i].name, 
                   contacts[i].phone,
                   contacts[i].group);
            found = 1;
        }
    }
    printf("╚══════════════════════╩═══════════════╩════════════════════╝\n");
    
    if(!found) {
        printf("❌ No contact found with name containing '%s'\n", searchName);
    }
}

void deleteContact() {
    char deleteName[NAME_LEN];
    int found = 0;
    
    printf("\n🗑️  Enter name to delete: ");
    scanf(" %[^\n]", deleteName);
    trim(deleteName);
    
    for(int i = 0; i < contactCount; i++) {
        if(strcasecmp(contacts[i].name, deleteName) == 0) {
            // Confirm deletion
            char confirm;
            printf("Are you sure you want to delete '%s'? (y/n): ", contacts[i].name);
            scanf(" %c", &confirm);
            if (confirm != 'y' && confirm != 'Y') {
                printf("Deletion cancelled.\n");
                return;
            }
            
            // Shift all contacts after i one position left
            for(int j = i; j < contactCount - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            contactCount--;
            found = 1;
            printf("\n✅ Contact deleted successfully!\n");
            break;
        }
    }
    
    if(!found) {
        printf("\n❌ Contact not found!\n");
    }
}

void editContact() {
    if(contactCount == 0) {
        printf("\n📭 No contacts to edit!\n");
        return;
    }
    
    char editName[NAME_LEN];
    printf("\n✏️  Enter name to edit: ");
    scanf(" %[^\n]", editName);
    trim(editName);
    
    int found = 0;
    for(int i = 0; i < contactCount; i++) {
        if(strcasecmp(contacts[i].name, editName) == 0) {
            printf("\nEditing contact: %s\n", contacts[i].name);
            printf("Press Enter to keep current value\n");
            
            char temp[200];
            
            printf("Name (%s): ", contacts[i].name);
            scanf(" %[^\n]", temp);
            trim(temp);
            if (strlen(temp) > 0) strcpy(contacts[i].name, temp);
            
            printf("Phone (%s): ", contacts[i].phone);
            scanf("%s", temp);
            trim(temp);
            if (strlen(temp) > 0) {
                if (validate_phone(temp)) {
                    strcpy(contacts[i].phone, temp);
                } else {
                    printf("Invalid phone number! Keeping current.\n");
                }
            }
            
            printf("Email (%s): ", contacts[i].email);
            scanf(" %[^\n]", temp);
            trim(temp);
            if (strlen(temp) > 0) strcpy(contacts[i].email, temp);
            
            printf("Address (%s): ", contacts[i].address);
            scanf(" %[^\n]", temp);
            trim(temp);
            if (strlen(temp) > 0) strcpy(contacts[i].address, temp);
            
            printf("Group (%s): ", contacts[i].group);
            scanf(" %[^\n]", temp);
            trim(temp);
            if (strlen(temp) > 0) strcpy(contacts[i].group, temp);
            
            printf("Birthday (%s): ", contacts[i].birthday);
            scanf(" %[^\n]", temp);
            trim(temp);
            if (strlen(temp) > 0) strcpy(contacts[i].birthday, temp);
            
            printf("Notes (%s): ", contacts[i].notes);
            scanf(" %[^\n]", temp);
            trim(temp);
            if (strlen(temp) > 0) strcpy(contacts[i].notes, temp);
            
            printf("\n✅ Contact updated successfully!\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\n❌ Contact not found!\n");
    }
}

void viewFavorites() {
    int favCount = 0;
    printf("\n⭐ FAVORITE CONTACTS ⭐\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║ Name                 ║ Phone         ║ Group              ║\n");
    printf("╠══════════════════════╬═══════════════╬════════════════════╣\n");
    
    for(int i = 0; i < contactCount; i++) {
        if(contacts[i].favorite) {
            printf("║ %-20s ║ %-13s ║ %-18s ║\n", 
                   contacts[i].name, 
                   contacts[i].phone,
                   contacts[i].group);
            favCount++;
        }
    }
    printf("╚══════════════════════╩═══════════════╩════════════════════╝\n");
    
    if(favCount == 0) {
        printf("No favorite contacts yet.\n");
    } else {
        printf("Total favorites: %d\n", favCount);
    }
}

void viewByGroup() {
    char groupName[GROUP_LEN];
    printf("\nEnter group name to view: ");
    scanf(" %[^\n]", groupName);
    trim(groupName);
    
    int count = 0;
    printf("\n--- Contacts in '%s' group ---\n", groupName);
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║ Name                 ║ Phone         ║ Favorite           ║\n");
    printf("╠══════════════════════╬═══════════════╬════════════════════╣\n");
    
    for(int i = 0; i < contactCount; i++) {
        if(strcasecmp(contacts[i].group, groupName) == 0) {
            printf("║ %-20s ║ %-13s ║ %-18s ║\n", 
                   contacts[i].name, 
                   contacts[i].phone,
                   contacts[i].favorite ? "⭐ Yes" : "No");
            count++;
        }
    }
    printf("╚══════════════════════╩═══════════════╩════════════════════╝\n");
    
    if(count == 0) {
        printf("No contacts found in group '%s'\n", groupName);
    } else {
        printf("Total contacts in group: %d\n", count);
    }
}

void sortContacts() {
    if(contactCount < 2) return;
    
    int sortChoice;
    printf("\nSort by:\n");
    printf("1. Name (A-Z)\n");
    printf("2. Name (Z-A)\n");
    printf("3. Group\n");
    printf("Choice: ");
    scanf("%d", &sortChoice);
    
    for(int i = 0; i < contactCount - 1; i++) {
        for(int j = i + 1; j < contactCount; j++) {
            int swap = 0;
            switch(sortChoice) {
                case 1:
                    if(strcasecmp(contacts[i].name, contacts[j].name) > 0) swap = 1;
                    break;
                case 2:
                    if(strcasecmp(contacts[i].name, contacts[j].name) < 0) swap = 1;
                    break;
                case 3:
                    if(strcasecmp(contacts[i].group, contacts[j].group) > 0) swap = 1;
                    break;
                default:
                    printf("Invalid choice!\n");
                    return;
            }
            if(swap) {
                Contact temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }
    printf("\n✅ Contacts sorted successfully!\n");
}

void exportContacts() {
    if(contactCount == 0) {
        printf("No contacts to export!\n");
        return;
    }
    
    char filename[100];
    printf("Enter filename to export (e.g., contacts.txt): ");
    scanf("%s", filename);
    
    FILE *file = fopen(filename, "w");
    if(!file) {
        printf("Error creating file!\n");
        return;
    }
    
    fprintf(file, "=== CONTACT BOOK EXPORT ===\n");
    fprintf(file, "Generated on: %s\n", __DATE__);
    fprintf(file, "Total contacts: %d\n\n", contactCount);
    
    for(int i = 0; i < contactCount; i++) {
        fprintf(file, "Contact #%d\n", i+1);
        fprintf(file, "Name: %s\n", contacts[i].name);
        fprintf(file, "Phone: %s\n", contacts[i].phone);
        if(strlen(contacts[i].email) > 0)
            fprintf(file, "Email: %s\n", contacts[i].email);
        if(strlen(contacts[i].address) > 0)
            fprintf(file, "Address: %s\n", contacts[i].address);
        fprintf(file, "Group: %s\n", contacts[i].group);
        if(strlen(contacts[i].birthday) > 0)
            fprintf(file, "Birthday: %s\n", contacts[i].birthday);
        fprintf(file, "Favorite: %s\n", contacts[i].favorite ? "Yes" : "No");
        if(strlen(contacts[i].notes) > 0)
            fprintf(file, "Notes: %s\n", contacts[i].notes);
        fprintf(file, "---\n");
    }
    
    fclose(file);
    printf("✅ Contacts exported to '%s' successfully!\n", filename);
}

int main() {
    int choice;
    
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║         📇 ADVANCED CONTACT BOOK v3.0        ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    do {
        printf("\n╔═══════════════════════════════════════════════╗\n");
        printf("║ 1. ➕ Add Contact                           ║\n");
        printf("║ 2. 📋 View All Contacts                    ║\n");
        printf("║ 3. 🔍 Search Contact                       ║\n");
        printf("║ 4. 📖 View Contact Details                 ║\n");
        printf("║ 5. ✏️  Edit Contact                         ║\n");
        printf("║ 6. 🗑️  Delete Contact                       ║\n");
        printf("║ 7. ⭐ View Favorites                       ║\n");
        printf("║ 8. 👥 View by Group                       ║\n");
        printf("║ 9. 🔄 Sort Contacts                       ║\n");
        printf("║ 10. 💾 Export Contacts                    ║\n");
        printf("║ 11. 🚪 Exit                              ║\n");
        printf("╚═══════════════════════════════════════════════╝\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: viewContactDetails(); break;
            case 5: editContact(); break;
            case 6: deleteContact(); break;
            case 7: viewFavorites(); break;
            case 8: viewByGroup(); break;
            case 9: sortContacts(); break;
            case 10: exportContacts(); break;
            case 11: printf("\n👋 Goodbye! Have a great day!\n"); break;
            default: printf("\n❌ Invalid choice! Try again.\n");
        }
    } while(choice != 11);
    
    return 0;
}
