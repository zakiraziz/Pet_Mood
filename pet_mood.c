#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_pet(int mood) {
    printf("\n");
    if (mood == 1) {
        printf("  /\\_/\\\n");
        printf(" (o.o )\n");
        printf("  > ^ <   Happy Pet!\n");
    } else if (mood == 2) {
        printf("  /\\_/\\\n");
        printf(" (-.- )\n");
        printf("  > ^ <   Sleepy Pet... zzz\n");
    } else if (mood == 3) {
        printf("  /\\_/\\\n");
        printf(" (>.< )\n");
        printf("  > ^ <   Grumpy Pet! Rawr!\n");
    } else {
        printf("  /\\_/\\\n");
        printf(" (o_o )\n");
        printf("  > ^ <   *confused pet noises*\n");
    }
    printf("\n");
}

int main() {
    int choice, mood;
    char name[20];
    
    printf("🐾 Welcome to Digital Pet Mood Meter! 🐾\n");
    printf("What would you like to name your pet? ");
    fgets(name, sizeof(name), stdin);
    
    // Remove newline from fgets
    name[strcspn(name, "\n")] = 0;
    
    printf("\nHello, %s! Let's see how your pet feels today.\n", name);
    
    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Tell me about your day\n");
        printf("2. Pet your buddy\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("\nHow was your day?\n");
                printf("1. Awesome! 🎉\n");
                printf("2. Relaxing 😴\n");
                printf("3. Rough day 😠\n");
                printf("Your choice: ");
                scanf("%d", &mood);
                
                srand(time(NULL));
                if (mood == 1) {
                    printf("\n%s wags tail excitedly! \"Best day ever!\" 🐕\n", name);
                    print_pet(1);
                } else if (mood == 2) {
                    printf("\n%s curls up next to you. \"Cozy vibes...\" 🛌\n", name);
                    print_pet(2);
                } else if (mood == 3) {
                    printf("\n%s growls a little. \"Let's watch something funny!\" 🎬\n", name);
                    print_pet(3);
                } else {
                    printf("\n%s tilts head. \"I don't get it, but I love you!\" 💕\n", name);
                    print_pet(4);
                }
                break;
                
            case 2:
                printf("\nYou gently pet %s... *purrrrr* 🥰\n", name);
                printf("Your pet feels loved!\n");
                print_pet(1);
                break;
                
            case 3:
                printf("\n%s waves paw. \"See you tomorrow, friend!\" 👋\n", name);
                break;
                
            default:
                printf("\nHmm? Try 1, 2, or 3!\n");
        }
    } while(choice != 3);
    
    printf("\nThanks for playing! Stay pawsome! 🐾\n");
    return 0;
}