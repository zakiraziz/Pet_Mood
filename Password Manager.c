#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

void encrypt(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = str[i] + 3; // Simple Caesar cipher
    }
}

void decrypt(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = str[i] - 3;
    }
}

void save_password() {
    char site[MAX], username[MAX], password[MAX];
    printf("Enter site name: ");
    getchar();
    fgets(site, MAX, stdin);
    site[strcspn(site, "\n")] = 0;
    
    printf("Enter username: ");
    fgets(username, MAX, stdin);
    username[strcspn(username, "\n")] = 0;
    
    printf("Enter password: ");
    fgets(password, MAX, stdin);
    password[strcspn(password, "\n")] = 0;
    
    encrypt(password);
    
    FILE *f = fopen("vault.txt", "a");
    fprintf(f, "%s|%s|%s\n", site, username, password);
    fclose(f);
    printf("Password saved securely!\n");
}

void view_passwords() {
    char master[] = "admin123";
    char input[MAX];
    printf("Enter master password: ");
    scanf("%s", input);
    if (strcmp(input, master) != 0) {
        printf("Wrong password!\n");
        return;
    }
    
    FILE *f = fopen("vault.txt", "r");
    if (!f) {
        printf("No saved passwords.\n");
        return;
    }
    char site[MAX], user[MAX], pass[MAX];
    printf("\n=== YOUR PASSWORDS ===\n");
    while (fscanf(f, "%[^|]|%[^|]|%[^\n]\n", site, user, pass) == 3) {
        decrypt(pass);
        printf("Site: %s | User: %s | Pass: %s\n", site, user, pass);
    }
    fclose(f);
}

int main() {
    int choice;
    do {
        printf("\n1. Save Password\n2. View Passwords\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: save_password(); break;
            case 2: view_passwords(); break;
            case 3: printf("Goodbye!\n"); break;
        }
    } while(choice != 3);
    return 0;
}