#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char letter;
    char morse[10];
} MorseCode;

MorseCode morseTable[] = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},
    {'E', "."}, {'F', "..-."}, {'G', "--."}, {'H', "...."},
    {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
    {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."},
    {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
    {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
    {'Y', "-.--"}, {'Z', "--.."},
    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
    {'8', "---.."}, {'9', "----."}
};

void textToMorse(char *text) {
    for(int i = 0; text[i]; i++) {
        char ch = toupper(text[i]);
        if(ch == ' ') {
            printf("  ");
            continue;
        }
        for(int j = 0; j < 36; j++) {
            if(morseTable[j].letter == ch) {
                printf("%s ", morseTable[j].morse);
                break;
            }
        }
    }
    printf("\n");
}

void morseToText(char *morse) {
    char *token = strtok(morse, " ");
    while(token != NULL) {
        if(strcmp(token, "/") == 0) {
            printf(" ");
        } else {
            for(int i = 0; i < 36; i++) {
                if(strcmp(morseTable[i].morse, token) == 0) {
                    printf("%c", morseTable[i].letter);
                    break;
                }
            }
        }
        token = strtok(NULL, " ");
    }
    printf("\n");
}

int main() {
    int choice;
    char input[200];
    
    printf("📡 Morse Code Translator\n");
    printf("1. Text to Morse\n");
    printf("2. Morse to Text\n");
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();
    
    if(choice == 1) {
        printf("Enter text: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        printf("Morse: ");
        textToMorse(input);
    } else if(choice == 2) {
        printf("Enter Morse (separate letters with space, words with /): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        printf("Text: ");
        morseToText(input);
    }
    
    return 0;
}