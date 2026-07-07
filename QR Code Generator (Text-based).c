#include <stdio.h>
#include <string.h>

void generatePattern(char *text) {
    int size = strlen(text) + 4;
    if(size < 10) size = 10;
    
    printf("\n📱 QR Pattern for: %s\n\n", text);
    
    // Top border
    for(int i = 0; i < size; i++) printf("█");
    printf("\n");
    
    // Middle with text
    printf("█");
    for(int i = 0; i < size-2; i++) printf(" ");
    printf("█\n");
    
    printf("█");
    int padding = (size - 4 - strlen(text)) / 2;
    for(int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);
    for(int i = 0; i < padding; i++) printf(" ");
    if(strlen(text) % 2 != (size - 4) % 2) printf(" ");
    printf("█\n");
    
    printf("█");
    for(int i = 0; i < size-2; i++) printf(" ");
    printf("█\n");
    
    // Bottom border
    for(int i = 0; i < size; i++) printf("█");
    printf("\n");
    
    // Binary pattern
    printf("\n🔢 Digital Pattern:\n");
    for(int i = 0; text[i]; i++) {
        int ascii = text[i];
        for(int bit = 7; bit >= 0; bit--) {
            printf("%d", (ascii >> bit) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

int main() {
    char text[100];
    printf("Enter text to encode: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;
    
    generatePattern(text);
    return 0;
}