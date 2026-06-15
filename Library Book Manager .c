#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int id;
    char title[100];
    char author[100];
    struct Book *left;
    struct Book *right;
} Book;

Book *root = NULL;

Book* create_book(int id, const char *title, const char *author) {
    Book *new_book = (Book*)malloc(sizeof(Book));
    if (!new_book) return NULL;
    
    new_book->id = id;
    strcpy(new_book->title, title);
    strcpy(new_book->author, author);
    new_book->left = new_book->right = NULL;
    return new_book;
}

Book* insert_book(Book *root, Book *new_book) {
    if (root == NULL) return new_book;
    
    if (new_book->id < root->id) {
        root->left = insert_book(root->left, new_book);
    } else if (new_book->id > root->id) {
        root->right = insert_book(root->right, new_book);
    } else {
        printf("Book ID %d already exists!\n", new_book->id);
        free(new_book);
    }
    return root;
}

Book* search_book(Book *root, int id) {
    if (root == NULL || root->id == id) return root;
    
    if (id < root->id) {
        return search_book(root->left, id);
    }
    return search_book(root->right, id);
}

Book* find_min(Book *root) {
    while (root && root->left) root = root->left;
    return root;
}

Book* delete_book(Book *root, int id) {
    if (root == NULL) return NULL;
    
    if (id < root->id) {
        root->left = delete_book(root->left, id);
    } else if (id > root->id) {
        root->right = delete_book(root->right, id);
    } else {
        if (root->left == NULL) {
            Book *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Book *temp = root->left;
            free(root);
            return temp;
        }
        
        Book *temp = find_min(root->right);
        root->id = temp->id;
        strcpy(root->title, temp->title);
        strcpy(root->author, temp->author);
        root->right = delete_book(root->right, temp->id);
    }
    return root;
}

void inorder_display(Book *root) {
    if (root != NULL) {
        inorder_display(root->left);
        printf("ID: %d | Title: %s | Author: %s\n", root->id, root->title, root->author);
        inorder_display(root->right);
    }
}

int main() {
    int choice, id;
    char title[100], author[100];
    
    do {
        printf("\n=== LIBRARY MANAGEMENT ===\n");
        printf("1. Add Book\n2. Search Book\n3. Delete Book\n");
        printf("4. Display All Books\n5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Enter Book ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter Title: ");
                fgets(title, 100, stdin);
                title[strcspn(title, "\n")] = 0;
                printf("Enter Author: ");
                fgets(author, 100, stdin);
                author[strcspn(author, "\n")] = 0;
                Book *new_book = create_book(id, title, author);
                root = insert_book(root, new_book);
                printf("Book added!\n");
                break;
                
            case 2:
                printf("Enter Book ID to search: ");
                scanf("%d", &id);
                Book *found = search_book(root, id);
                if (found) {
                    printf("\nBook Found:\n");
                    printf("ID: %d | Title: %s | Author: %s\n", 
                           found->id, found->title, found->author);
                } else {
                    printf("Book not found!\n");
                }
                break;
                
            case 3:
                printf("Enter Book ID to delete: ");
                scanf("%d", &id);
                root = delete_book(root, id);
                printf("Book deleted (if existed)!\n");
                break;
                
            case 4:
                if (root == NULL) {
                    printf("No books in library!\n");
                } else {
                    printf("\n=== ALL BOOKS ===\n");
                    inorder_display(root);
                }
                break;
                
            case 5:
                printf("Goodbye!\n");
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    } while(choice != 5);
    
    return 0;
}