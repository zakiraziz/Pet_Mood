#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int id;
    char title[100];
    char author[50];
    int isIssued;
    struct Book *next;
} Book;

Book *head = NULL;

void addBook();
void displayBooks();
void issueBook();
void returnBook();
void searchBook();
void freeMemory();

int main() {
    int choice;
    
    while(1) {
        printf("\n=== LIBRARY MANAGEMENT SYSTEM ===\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Search Book\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: searchBook(); break;
            case 6: freeMemory(); printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

void addBook() {
    Book *newBook = (Book*)malloc(sizeof(Book));
    if(newBook == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    printf("\nEnter Book ID: ");
    scanf("%d", &newBook->id);
    printf("Enter Book Title: ");
    scanf(" %[^\n]", newBook->title);
    printf("Enter Author Name: ");
    scanf(" %[^\n]", newBook->author);
    newBook->isIssued = 0;
    newBook->next = NULL;
    
    if(head == NULL) {
        head = newBook;
    } else {
        Book *temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newBook;
    }
    printf("Book added successfully!\n");
}

void displayBooks() {
    if(head == NULL) {
        printf("\nNo books in library!\n");
        return;
    }
    
    Book *temp = head;
    printf("\n=== ALL BOOKS ===\n");
    while(temp != NULL) {
        printf("ID: %d | Title: %s | Author: %s | Status: %s\n", 
               temp->id, temp->title, temp->author, 
               temp->isIssued ? "Issued" : "Available");
        temp = temp->next;
    }
}

void issueBook() {
    int id, found = 0;
    printf("\nEnter Book ID to issue: ");
    scanf("%d", &id);
    
    Book *temp = head;
    while(temp != NULL) {
        if(temp->id == id) {
            found = 1;
            if(temp->isIssued) {
                printf("Book is already issued!\n");
            } else {
                temp->isIssued = 1;
                printf("Book issued successfully!\n");
            }
            break;
        }
        temp = temp->next;
    }
    if(!found) printf("Book not found!\n");
}

void returnBook() {
    int id, found = 0;
    printf("\nEnter Book ID to return: ");
    scanf("%d", &id);
    
    Book *temp = head;
    while(temp != NULL) {
        if(temp->id == id) {
            found = 1;
            if(!temp->isIssued) {
                printf("Book was not issued!\n");
            } else {
                temp->isIssued = 0;
                printf("Book returned successfully!\n");
            }
            break;
        }
        temp = temp->next;
    }
    if(!found) printf("Book not found!\n");
}

void searchBook() {
    int choice, id, found = 0;
    char title[100];
    
    printf("\nSearch by:\n1. ID\n2. Title\nEnter choice: ");
    scanf("%d", &choice);
    
    Book *temp = head;
    if(choice == 1) {
        printf("Enter Book ID: ");
        scanf("%d", &id);
        while(temp != NULL) {
            if(temp->id == id) {
                printf("\nBook Found!\nID: %d | Title: %s | Author: %s | Status: %s\n",
                       temp->id, temp->title, temp->author,
                       temp->isIssued ? "Issued" : "Available");
                found = 1;
                break;
            }
            temp = temp->next;
        }
    } else if(choice == 2) {
        printf("Enter Book Title: ");
        scanf(" %[^\n]", title);
        while(temp != NULL) {
            if(strcmp(temp->title, title) == 0) {
                printf("\nBook Found!\nID: %d | Title: %s | Author: %s | Status: %s\n",
                       temp->id, temp->title, temp->author,
                       temp->isIssued ? "Issued" : "Available");
                found = 1;
            }
            temp = temp->next;
        }
    }
    if(!found) printf("Book not found!\n");
}

void freeMemory() {
    Book *temp;
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}