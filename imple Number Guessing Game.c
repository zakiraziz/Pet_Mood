#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int secret = rand() % 100 + 1, guess;
    printf("Guess 1–100: ");
    do {
        scanf("%d", &guess);
        if (guess < secret) printf("Too low: ");
        else if (guess > secret) printf("Too high: ");
    } while (guess != secret);
    printf("Correct!\n");
    return 0;
}