#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

typedef struct {
    int id;
    char name[50];
    float grades[5];
    float average;
} Student;

void addStudent();
void displayStudents();
void searchStudent();
float calculateAverage(float grades[]);
void saveToFile(Student s);
void loadFromFile();

int main() {
    int choice;
    
    while (1) {
        printf("\n=== STUDENT GRADE MANAGEMENT SYSTEM ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

float calculateAverage(float grades[]) {
    float sum = 0;
    for(int i = 0; i < 5; i++) {
        sum += grades[i];
    }
    return sum / 5;
}

void addStudent() {
    Student s;
    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);
    printf("Enter Student Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter 5 grades:\n");
    for(int i = 0; i < 5; i++) {
        printf("Grade %d: ", i+1);
        scanf("%f", &s.grades[i]);
    }
    s.average = calculateAverage(s.grades);
    saveToFile(s);
    printf("Student added successfully! Average: %.2f\n", s.average);
}

void saveToFile(Student s) {
    FILE *file = fopen(FILENAME, "ab");
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&s, sizeof(Student), 1, file);
    fclose(file);
}

void displayStudents() {
    Student s;
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No records found!\n");
        return;
    }
    printf("\n=== ALL STUDENTS ===\n");
    while(fread(&s, sizeof(Student), 1, file)) {
        printf("ID: %d | Name: %s | Average: %.2f\n", s.id, s.name, s.average);
    }
    fclose(file);
}

void searchStudent() {
    int id, found = 0;
    Student s;
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No records found!\n");
        return;
    }
    printf("Enter Student ID to search: ");
    scanf("%d", &id);
    
    while(fread(&s, sizeof(Student), 1, file)) {
        if(s.id == id) {
            printf("\nStudent Found!\n");
            printf("ID: %d\nName: %s\nAverage: %.2f\nGrades: ", s.id, s.name, s.average);
            for(int i = 0; i < 5; i++) {
                printf("%.2f ", s.grades[i]);
            }
            printf("\n");
            found = 1;
            break;
        }
    }
    if(!found) printf("Student not found!\n");
    fclose(file);
}