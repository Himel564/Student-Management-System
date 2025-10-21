#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== Structure Definition =====
struct Student {
    int roll;
    char name[50];
    int age;
    float marks;
};

// ===== Function Prototypes =====
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// ===== Main Function =====
int main() {
    int choice;

    while (1) {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: 
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// ===== Function to Add a Student =====
void addStudent() {
    struct Student s;
    FILE *fp = fopen("students.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    getchar(); // to clear newline
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // remove newline
    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Student added successfully!\n");
}

// ===== Function to Display All Students =====
void displayStudents() {
    struct Student s;
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    int found = 0;
    printf("\n----- Student Records -----\n");
    while (fread(&s, sizeof(s), 1, fp)) {
        printf("\nRoll: %d\nName: %s\nAge: %d\nMarks: %.2f\n", s.roll, s.name, s.age, s.marks);
        found = 1;
    }

    if (!found)
        printf("No records found!\n");

    fclose(fp);
}

// ===== Function to Search a Student =====
void searchStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to search: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            printf("\nRecord Found!\n");
            printf("Roll: %d\nName: %s\nAge: %d\nMarks: %.2f\n", s.roll, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No student found with Roll Number %d\n", roll);

    fclose(fp);
}

// ===== Function to Update a Student =====
void updateStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen("students.dat", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to update: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            printf("Enter new Name: ");
            getchar();
            fgets(s.name, 50, stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &s.age);
            printf("Enter new Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);

            printf("Record updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student with Roll Number %d not found.\n", roll);

    fclose(fp);
}

// ===== Function to Delete a Student =====
void deleteStudent() {
    struct Student s;
    int roll, found = 0;

    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Error opening temporary file!\n");
        fclose(fp);
        return;
    }

    printf("Enter Roll Number to delete: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll != roll) {
            fwrite(&s, sizeof(s), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("Student with Roll Number %d not found.\n", roll);
}
