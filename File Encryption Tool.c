#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

// Simple XOR encryption/decryption
void xor_encrypt_decrypt(const char* input_file, const char* output_file, const char* password) {
    FILE* in = fopen(input_file, "rb");
    if (!in) {
        printf("Error: Cannot open input file '%s'\n", input_file);
        return;
    }
    
    FILE* out = fopen(output_file, "wb");
    if (!out) {
        printf("Error: Cannot create output file '%s'\n", output_file);
        fclose(in);
        return;
    }
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    int pass_len = strlen(password);
    int pass_index = 0;
    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, in)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            buffer[i] ^= password[pass_index % pass_len];
            pass_index++;
        }
        fwrite(buffer, 1, bytes_read, out);
    }
    
    fclose(in);
    fclose(out);
    printf("✓ Operation completed successfully!\n");
}

// Simple Caesar cipher (for text files only)
void caesar_cipher(const char* input_file, const char* output_file, int shift, int encrypt) {
    FILE* in = fopen(input_file, "r");
    if (!in) {
        printf("Error: Cannot open input file '%s'\n", input_file);
        return;
    }
    
    FILE* out = fopen(output_file, "w");
    if (!out) {
        printf("Error: Cannot create output file '%s'\n", output_file);
        fclose(in);
        return;
    }
    
    int ch;
    if (!encrypt) shift = -shift;
    
    while ((ch = fgetc(in)) != EOF) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = ((ch - base + shift) % 26 + 26) % 26 + base;
        }
        fputc(ch, out);
    }
    
    fclose(in);
    fclose(out);
    printf("✓ Caesar cipher completed!\n");
}

void show_menu() {
    printf("\n=== FILE ENCRYPTION TOOL ===\n");
    printf("1. XOR Encrypt/Decrypt (Binary safe)\n");
    printf("2. Caesar Cipher (Text only)\n");
    printf("3. Exit\n");
    printf("Choose option: ");
}

int main() {
    int choice, shift, encrypt;
    char input_file[256], output_file[256], password[256];
    
    while (1) {
        show_menu();
        scanf("%d", &choice);
        getchar(); // Clear newline
        
        switch(choice) {
            case 1:
                printf("\n--- XOR Encryption ---\n");
                printf("Input file: ");
                fgets(input_file, 256, stdin);
                input_file[strcspn(input_file, "\n")] = 0;
                
                printf("Output file: ");
                fgets(output_file, 256, stdin);
                output_file[strcspn(output_file, "\n")] = 0;
                
                printf("Password: ");
                fgets(password, 256, stdin);
                password[strcspn(password, "\n")] = 0;
                
                xor_encrypt_decrypt(input_file, output_file, password);
                break;
                
            case 2:
                printf("\n--- Caesar Cipher ---\n");
                printf("Input file: ");
                fgets(input_file, 256, stdin);
                input_file[strcspn(input_file, "\n")] = 0;
                
                printf("Output file: ");
                fgets(output_file, 256, stdin);
                output_file[strcspn(output_file, "\n")] = 0;
                
                printf("Shift (1-25): ");
                scanf("%d", &shift);
                shift = abs(shift) % 26;
                if (shift == 0) shift = 1;
                
                printf("1. Encrypt\n2. Decrypt\n");
                printf("Choose: ");
                scanf("%d", &encrypt);
                
                caesar_cipher(input_file, output_file, shift, encrypt == 1);
                break;
                
            case 3:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}