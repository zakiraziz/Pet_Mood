#include <stdio.h>

#define NUM_SUBJECTS 5

float calculateAverage(int marks[], int n) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += marks[i];
    }
    return (float)sum / n;
}

char getGrade(float percentage) {
    if(percentage >= 90) return 'A';
    else if(percentage >= 80) return 'B';
    else if(percentage >= 70) return 'C';
    else if(percentage >= 60) return 'D';
    else return 'F';
}

int main() {
    int marks[NUM_SUBJECTS];
    int total = 0;
    float average, percentage;
    
    printf("\n========== STUDENT GRADE CALCULATOR ==========\n\n");
    
    for(int i = 0; i < NUM_SUBJECTS; i++) {
        printf("Enter marks for subject %d (0-100): ", i + 1);
        scanf("%d", &marks[i]);
        
        // Validate input
        while(marks[i] < 0 || marks[i] > 100) {
            printf("Invalid marks! Please enter between 0-100: ");
            scanf("%d", &marks[i]);
        }
        total += marks[i];
    }
    
    average = calculateAverage(marks, NUM_SUBJECTS);
    percentage = (float)total / NUM_SUBJECTS;
    
    printf("\n=============== RESULTS ===============\n");
    printf("Total Marks:     %d / %d\n", total, NUM_SUBJECTS * 100);
    printf("Average Marks:   %.2f\n", average);
    printf("Percentage:      %.2f%%\n", percentage);
    printf("Grade:           %c\n", getGrade(percentage));
    printf("=======================================\n");
    
    return 0;
}