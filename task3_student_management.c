#include <stdio.h>
#include <string.h>

#define FILENAME "students.dat"
#define TEMPFILE "temp.dat"

typedef struct {
    int roll;
    char name[50];
    int age;
    float marks;
} Student;

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int rollExists(int roll) {
    FILE *fp = fopen(FILENAME, "rb");
    Student s;
    if (!fp) return 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1)
        if (s.roll == roll) { fclose(fp); return 1; }
    fclose(fp);
    return 0;
}

void addStudent(void) {
    Student s;
    FILE *fp;
    printf("Enter roll number: ");
    if (scanf("%d", &s.roll) != 1) { clearInput(); printf("Invalid input.\n"); return; }
    if (rollExists(s.roll)) { printf("Roll number already exists.\n"); return; }
    clearInput();
    printf("Enter name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';
    printf("Enter age: ");
    scanf("%d", &s.age);
    printf("Enter marks: ");
    scanf("%f", &s.marks);

    fp = fopen(FILENAME, "ab");
    if (!fp) { printf("Error opening file.\n"); return; }
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayStudents(void) {
    FILE *fp = fopen(FILENAME, "rb");
    Student s;
    int count = 0;
    if (!fp) { printf("No records found.\n"); return; }
    printf("\n%-8s %-20s %-5s %-8s\n", "Roll", "Name", "Age", "Marks");
    printf("------------------------------------------------\n");
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-8d %-20s %-5d %-8.2f\n", s.roll, s.name, s.age, s.marks);
        count++;
    }
    fclose(fp);
    if (count == 0) printf("No records found.\n");
}

void searchStudent(void) {
    FILE *fp = fopen(FILENAME, "rb");
    Student s;
    int roll, found = 0;
    if (!fp) { printf("No records found.\n"); return; }
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\nRoll: %d\nName: %s\nAge: %d\nMarks: %.2f\n",
                   s.roll, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Student not found.\n");
}

void updateStudent(void) {
    FILE *fp = fopen(FILENAME, "rb+");
    Student s;
    int roll, found = 0;
    if (!fp) { printf("No records found.\n"); return; }
    printf("Enter roll number to update: ");
    scanf("%d", &roll);
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            clearInput();
            printf("Enter new name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter new age: ");
            scanf("%d", &s.age);
            printf("Enter new marks: ");
            scanf("%f", &s.marks);
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    printf(found ? "Record updated successfully.\n" : "Student not found.\n");
}

void deleteStudent(void) {
    FILE *fp = fopen(FILENAME, "rb");
    FILE *tmp;
    Student s;
    int roll, found = 0;
    if (!fp) { printf("No records found.\n"); return; }
    tmp = fopen(TEMPFILE, "wb");
    if (!tmp) { fclose(fp); printf("Error opening temp file.\n"); return; }
    printf("Enter roll number to delete: ");
    scanf("%d", &roll);
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) found = 1;
        else fwrite(&s, sizeof(Student), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);
    remove(FILENAME);
    rename(TEMPFILE, FILENAME);
    printf(found ? "Record deleted successfully.\n" : "Student not found.\n");
}

int main(void) {
    int choice;
    do {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n2. Delete Student\n3. Update Student\n");
        printf("4. Search Student\n5. Display All\n6. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { clearInput(); choice = 0; }
        switch (choice) {
        case 1: addStudent(); break;
        case 2: deleteStudent(); break;
        case 3: updateStudent(); break;
        case 4: searchStudent(); break;
        case 5: displayStudents(); break;
        case 6: printf("Goodbye!\n"); break;
        default: printf("Invalid choice.\n");
        }
    } while (choice != 6);
    return 0;
}
