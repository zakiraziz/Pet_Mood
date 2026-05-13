#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

typedef struct {
    int roll;
    char name[50];
    float marks[3];
    float average;
    char grade;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

// Function declarations
void addStudent();
void viewAll();
void searchStudent();
void deleteStudent();
void updateStudent();
void saveToFile();
void loadFromFile();
void displayTopPerformer();
void sortByMarks();
char calculateGrade(float avg);
void clearInputBuffer();
void printHeader();
void printStudent(Student s, int index);

int main() {
    loadFromFile();
    int choice;
    
    do {
        printf("\n╔════════════════════════════════════╗\n");
        printf("║   STUDENT RECORD MANAGEMENT SYSTEM  ║\n");
        printf("╠════════════════════════════════════╣\n");
        printf("║  1. Add Student                    ║\n");
        printf("║  2. View All Students              ║\n");
        printf("║  3. Search Student                 ║\n");
        printf("║  4. Delete Student                 ║\n");
        printf("║  5. Update Student                 ║\n");
        printf("║  6. Top Performer                  ║\n");
        printf("║  7. Sort by Marks                  ║\n");
        printf("║  8. Save & Exit                    ║\n");
        printf("╚════════════════════════════════════╝\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: viewAll(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: updateStudent(); break;
            case 6: displayTopPerformer(); break;
            case 7: sortByMarks(); break;
            case 8: saveToFile(); 
                    printf("✓ Data saved. Exiting...\n");
                    break;
            default: printf("✗ Invalid choice! Please try again.\n");
        }
    } while(choice != 8);
    
    return 0;
}

void addStudent() {
    if(studentCount >= MAX_STUDENTS) {
        printf("✗ Maximum students limit reached!\n");
        return;
    }
    
    Student newStudent;
    int rollExists;
    
    do {
        rollExists = 0;
        printf("\n--- Add New Student ---\n");
        printf("Enter Roll Number: ");
        scanf("%d", &newStudent.roll);
        
        // Check if roll number already exists
        for(int i = 0; i < studentCount; i++) {
            if(students[i].roll == newStudent.roll) {
                printf("✗ Roll number already exists! Please use a unique roll number.\n");
                rollExists = 1;
                break;
            }
        }
    } while(rollExists);
    
    clearInputBuffer();
    printf("Enter Name: ");
    fgets(newStudent.name, 50, stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = 0; // Remove newline
    
    printf("Enter marks for 3 subjects:\n");
    for(int i = 0; i < 3; i++) {
        do {
            printf("Subject %d: ", i+1);
            scanf("%f", &newStudent.marks[i]);
            if(newStudent.marks[i] < 0 || newStudent.marks[i] > 100) {
                printf("✗ Invalid marks! Please enter between 0-100.\n");
            }
        } while(newStudent.marks[i] < 0 || newStudent.marks[i] > 100);
    }
    
    // Calculate average and grade
    newStudent.average = (newStudent.marks[0] + newStudent.marks[1] + newStudent.marks[2]) / 3;
    newStudent.grade = calculateGrade(newStudent.average);
    
    students[studentCount] = newStudent;
    studentCount++;
    
    printf("✓ Student added successfully!\n");
    printf("  Name: %s | Roll: %d | Average: %.2f | Grade: %c\n", 
           newStudent.name, newStudent.roll, newStudent.average, newStudent.grade);
}

void viewAll() {
    if(studentCount == 0) {
        printf("\n✗ No students found! Please add students first.\n");
        return;
    }
    
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                         STUDENT RECORDS                          ║\n");
    printf("╠═════╦════════════════════╦══════════════════════════════════════╣\n");
    printf("║ No ║ Roll Number        ║ Name               │ Marks     │ Avg │ Grade ║\n");
    printf("╠═════╬════════════════════╬══════════════════════════════════════╣\n");
    
    for(int i = 0; i < studentCount; i++) {
        printf("║ %-3d ║ %-18d ║ %-18s ", i+1, students[i].roll, students[i].name);
        printf("│ ");
        for(int j = 0; j < 3; j++) {
            printf("%.0f ", students[i].marks[j]);
        }
        printf("│ %.2f │ %c    ║\n", students[i].average, students[i].grade);
    }
    printf("╚═════╩════════════════════╩══════════════════════════════════════╝\n");
    printf("Total Students: %d\n", studentCount);
}

void searchStudent() {
    if(studentCount == 0) {
        printf("\n✗ No students found!\n");
        return;
    }
    
    int roll, found = 0;
    printf("\n--- Search Student ---\n");
    printf("Enter Roll Number: ");
    scanf("%d", &roll);
    
    for(int i = 0; i < studentCount; i++) {
        if(students[i].roll == roll) {
            printf("\n✓ Student Found!\n");
            printf("╔════════════════════════════════╗\n");
            printf("║ Roll Number: %-20d ║\n", students[i].roll);
            printf("║ Name: %-24s ║\n", students[i].name);
            printf("║ Marks:                           ║\n");
            printf("║   Subject 1: %-18.2f ║\n", students[i].marks[0]);
            printf("║   Subject 2: %-18.2f ║\n", students[i].marks[1]);
            printf("║   Subject 3: %-18.2f ║\n", students[i].marks[2]);
            printf("║ Average: %-21.2f ║\n", students[i].average);
            printf("║ Grade: %-23c ║\n", students[i].grade);
            printf("╚════════════════════════════════╝\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("✗ Student with Roll Number %d not found!\n", roll);
    }
}

void deleteStudent() {
    if(studentCount == 0) {
        printf("\n✗ No students to delete!\n");
        return;
    }
    
    int roll, found = 0;
    char confirm;
    
    printf("\n--- Delete Student ---\n");
    printf("Enter Roll Number to delete: ");
    scanf("%d", &roll);
    
    for(int i = 0; i < studentCount; i++) {
        if(students[i].roll == roll) {
            printf("\nStudent to delete:\n");
            printf("Name: %s | Roll: %d\n", students[i].name, students[i].roll);
            printf("Are you sure? (y/n): ");
            clearInputBuffer();
            scanf("%c", &confirm);
            
            if(confirm == 'y' || confirm == 'Y') {
                // Shift all students after i one position left
                for(int j = i; j < studentCount - 1; j++) {
                    students[j] = students[j+1];
                }
                studentCount--;
                printf("✓ Student deleted successfully!\n");
            } else {
                printf("✗ Deletion cancelled.\n");
            }
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("✗ Student with Roll Number %d not found!\n", roll);
    }
}

void updateStudent() {
    if(studentCount == 0) {
        printf("\n✗ No students to update!\n");
        return;
    }
    
    int roll, found = 0;
    int choice;
    
    printf("\n--- Update Student ---\n");
    printf("Enter Roll Number to update: ");
    scanf("%d", &roll);
    
    for(int i = 0; i < studentCount; i++) {
        if(students[i].roll == roll) {
            printf("\n✓ Student Found!\n");
            printf("Current Details:\n");
            printf("Name: %s\n", students[i].name);
            printf("Marks: %.2f, %.2f, %.2f\n", students[i].marks[0], students[i].marks[1], students[i].marks[2]);
            
            printf("\nWhat do you want to update?\n");
            printf("1. Name\n");
            printf("2. Marks\n");
            printf("3. Both\n");
            printf("Choice: ");
            scanf("%d", &choice);
            
            clearInputBuffer();
            
            if(choice == 1 || choice == 3) {
                printf("Enter new name: ");
                fgets(students[i].name, 50, stdin);
                students[i].name[strcspn(students[i].name, "\n")] = 0;
            }
            
            if(choice == 2 || choice == 3) {
                printf("Enter new marks for 3 subjects:\n");
                for(int j = 0; j < 3; j++) {
                    do {
                        printf("Subject %d: ", j+1);
                        scanf("%f", &students[i].marks[j]);
                        if(students[i].marks[j] < 0 || students[i].marks[j] > 100) {
                            printf("✗ Invalid marks! Please enter between 0-100.\n");
                        }
                    } while(students[i].marks[j] < 0 || students[i].marks[j] > 100);
                }
                // Recalculate average and grade
                students[i].average = (students[i].marks[0] + students[i].marks[1] + students[i].marks[2]) / 3;
                students[i].grade = calculateGrade(students[i].average);
            }
            
            printf("✓ Student record updated successfully!\n");
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("✗ Student with Roll Number %d not found!\n", roll);
    }
}

void displayTopPerformer() {
    if(studentCount == 0) {
        printf("\n✗ No students found!\n");
        return;
    }
    
    int topIndex = 0;
    for(int i = 1; i < studentCount; i++) {
        if(students[i].average > students[topIndex].average) {
            topIndex = i;
        }
    }
    
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║         ★ TOP PERFORMER ★           ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("Name: %s\n", students[topIndex].name);
    printf("Roll Number: %d\n", students[topIndex].roll);
    printf("Average Marks: %.2f\n", students[topIndex].average);
    printf("Grade: %c\n", students[topIndex].grade);
    
    if(students[topIndex].average >= 90) {
        printf("🏆 Excellent performance!\n");
    } else if(students[topIndex].average >= 75) {
        printf("👍 Very good performance!\n");
    }
}

void sortByMarks() {
    if(studentCount == 0) {
        printf("\n✗ No students to sort!\n");
        return;
    }
    
    // Bubble sort based on average marks
    Student temp;
    for(int i = 0; i < studentCount - 1; i++) {
        for(int j = 0; j < studentCount - i - 1; j++) {
            if(students[j].average < students[j+1].average) {
                temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }
    
    printf("\n✓ Students sorted by marks (highest to lowest)!\n");
    viewAll(); // Display sorted list
}

void saveToFile() {
    FILE *file = fopen(FILENAME, "wb");
    if(file == NULL) {
        printf("✗ Error opening file for writing!\n");
        return;
    }
    
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    
    fclose(file);
    printf("✓ Data saved successfully to %s\n", FILENAME);
}

void loadFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No existing data file found. Starting fresh...\n");
        studentCount = 0;
        return;
    }
    
    fread(&studentCount, sizeof(int), 1, file);
    fread(students, sizeof(Student), studentCount, file);
    
    fclose(file);
    printf("✓ Loaded %d student records from file.\n", studentCount);
}

char calculateGrade(float avg) {
    if(avg >= 90) return 'A';
    else if(avg >= 80) return 'B';
    else if(avg >= 70) return 'C';
    else if(avg >= 60) return 'D';
    else if(avg >= 50) return 'E';
    else return 'F';
}

void clearInputBuffer() {
    while(getchar() != '\n');
}

void printHeader() {
    printf("\n=== Student Record Management System ===\n");
}

void printStudent(Student s, int index) {
    printf("%d. Roll: %d | Name: %s | Avg: %.2f | Grade: %c\n", 
           index, s.roll, s.name, s.average, s.grade);
}