#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char name[30];
    int hunger;
    int happiness;
    int health;
    int age;
} Pet;

void displayPet(Pet *p) {
    printf("\n🐾 %s's Status\n", p->name);
    printf("━━━━━━━━━━━━━━━━━━━━\n");
    printf("❤️  Health: ");
    for(int i = 0; i < p->health/10; i++) printf("█");
    printf(" %d%%\n", p->health);
    
    printf("🍖 Hunger: ");
    for(int i = 0; i < p->hunger/10; i++) printf("█");
    printf(" %d%%\n", p->hunger);
    
    printf("😊 Happiness: ");
    for(int i = 0; i < p->happiness/10; i++) printf("█");
    printf(" %d%%\n", p->happiness);
    
    printf("📅 Age: %d days\n", p->age);
}

void feed(Pet *p) {
    if(p->hunger < 100) {
        p->hunger += 20;
        if(p->hunger > 100) p->hunger = 100;
        printf("🍽️  %s is eating... Yum!\n", p->name);
        p->happiness += 5;
    } else {
        printf("❌ %s is not hungry!\n", p->name);
    }
}

void play(Pet *p) {
    if(p->happiness < 100) {
        p->happiness += 15;
        if(p->happiness > 100) p->happiness = 100;
        printf("🎮 Playing with %s... Fun!\n", p->name);
        p->hunger -= 10;
        p->health += 5;
    } else {
        printf("😴 %s is already very happy!\n", p->name);
    }
}

void sleepPet(Pet *p) {
    printf("💤 %s is sleeping... Zzz...\n", p->name);
    p->health += 10;
    p->hunger -= 5;
    if(p->health > 100) p->health = 100;
}

void timePasses(Pet *p) {
    p->hunger -= 5 + rand() % 10;
    p->happiness -= 5 + rand() % 10;
    p->health -= 2 + rand() % 5;
    p->age++;
    
    // Clamp values
    if(p->hunger < 0) p->hunger = 0;
    if(p->happiness < 0) p->happiness = 0;
    if(p->health < 0) p->health = 0;
}

int main() {
    Pet pet;
    int choice;
    
    srand(time(0));
    
    printf("🎮 Virtual Pet Simulator\n");
    printf("Name your pet: ");
    scanf("%s", pet.name);
    
    pet.hunger = 80;
    pet.happiness = 80;
    pet.health = 100;
    pet.age = 0;
    
    printf("\n✨ Welcome %s! Take care of your new friend!\n", pet.name);
    
    while(1) {
        displayPet(&pet);
        
        if(pet.health <= 0) {
            printf("\n💀 Oh no! %s has passed away. Game Over!\n", pet.name);
            break;
        }
        
        if(pet.hunger <= 0 || pet.happiness <= 0) {
            printf("\n⚠️  Warning: %s needs attention!\n", pet.name);
        }
        
        printf("\nOptions:\n");
        printf("1. 🍖 Feed\n");
        printf("2. 🎮 Play\n");
        printf("3. 💤 Sleep\n");
        printf("4. ⏰ Skip time\n");
        printf("5. ❌ Quit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: feed(&pet); break;
            case 2: play(&pet); break;
            case 3: sleepPet(&pet); break;
            case 4: timePasses(&pet); break;
            case 5: 
                printf("👋 Goodbye! %s will miss you!\n", pet.name);
                return 0;
            default: printf("Invalid choice!\n");
        }
        
        timePasses(&pet); // Time passes after each action
    }
    
    return 0;
}