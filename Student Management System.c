#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DIVISIONS 6  // Max students per division
#define MAX_SUBJECTS 10  // Max number of subjects for attendance
#define MAX_STUDENTS 100  // Max number of students

// Dynamic array for subjects
char subjects[MAX_SUBJECTS][50];
int subjectCount = 0;

// Structure to represent a student
typedef struct Student {
    int id;  // Unique student ID (roll number)
    char name[100];  // Full name of the student
    char course[50];  // Course of the student
    char division;  // Division assigned (A, B, C, ... )
    int attendance[MAX_SUBJECTS]; // Array for attendance in different subjects
    int grades[MAX_SUBJECTS]; // Grades for each subject
    char attendanceDate[50];  // Date when attendance is taken
    struct Student* next;  // Pointer to the next student in the list
} Student;

// Global variables
int nextID = 1;  // Starting ID is 1
int divCount[MAX_DIVISIONS] = {0};  // To keep track of how many students are in each division

// Function to insert a new student
void insertStudent(Student** head, char* name, char* course) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    newStudent->id = nextID++;
    strcpy(newStudent->name, name);
    strcpy(newStudent->course, course);
    newStudent->division = 'A'; // Default division 'A'
    memset(newStudent->attendance, -1, sizeof(newStudent->attendance)); // Set attendance as -1 (not marked)
    newStudent->attendanceDate[0] = '\0'; // Empty date initially
    newStudent->next = *head;
    *head = newStudent;
    divCount[0]++;  // Increment the count for division A (assuming division 'A')
    printf("Student added: %s (ID: %d)\n", newStudent->name, newStudent->id);
}

// Function to search for a student by ID
Student* searchStudent(Student* head, int id) {
    Student* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Function to mark attendance for students
void markAttendance(Student* head) {
    if (head == NULL) {
        printf("No students available to mark attendance.\n");
        return;
    }

    char subjectName[50], date[50];
    printf("Enter the subject name: ");
    scanf(" %[^\n]s", subjectName);

    int subjectIndex = -1;
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjects[i], subjectName) == 0) {
            subjectIndex = i;
            break;
        }
    }

    if (subjectIndex == -1) {
        printf("Invalid subject name. Please try again.\n");
        return;
    }

    // Asking user for the date
    printf("Enter the date for attendance (DD-MM-YYYY): ");
    scanf(" %[^\n]s", date);

    Student* temp = head;
    int attendance;
    while (temp != NULL) {
        printf("Enter attendance for %s (ID: %d): 1 for present, 0 for absent: ", temp->name, temp->id);
        scanf("%d", &attendance);

        if (attendance == 0 || attendance == 1) {
            temp->attendance[subjectIndex] = attendance;
            strcpy(temp->attendanceDate, date);  // Store the date when attendance is taken
        } else {
            printf("Invalid input. Attendance must be 0 or 1. Skipping this student.\n");
        }

        temp = temp->next;
    }

    printf("Attendance for %s on %s has been updated.\n", subjectName, date);
}

// Function to display attendance summary for a subject
void displayAttendanceSummary(Student* head) {
    if (head == NULL) {
        printf("No students available to display attendance.\n");
        return;
    }

    char subjectName[50];
    printf("Enter the subject name: ");
    scanf(" %[^\n]s", subjectName);

    int subjectIndex = -1;
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjects[i], subjectName) == 0) {
            subjectIndex = i;
            break;
        }
    }

    if (subjectIndex == -1) {
        printf("Invalid subject name. Please try again.\n");
        return;
    }

    int presentCount = 0, absentCount = 0;
    Student* temp = head;

    printf("\nAttendance Summary for %s:\n", subjectName);
    while (temp != NULL) {
        if (temp->attendance[subjectIndex] == 1) {
            presentCount++;
        } else {
            absentCount++;
        }

        // Display the date of attendance for each student
        if (strlen(temp->attendanceDate) > 0) {
            printf("Student ID: %d, Name: %s, Attendance: %s, Date: %s\n",
                   temp->id, temp->name, temp->attendance[subjectIndex] ? "Present" : "Absent", temp->attendanceDate);
        }
        temp = temp->next;
    }

    printf("Total Present: %d\n", presentCount);
    printf("Total Absent: %d\n", absentCount);
}

// Main program with the menu system
int main() {
    Student* head = NULL;
    int choice, studentId;

    // Example subjects for attendance
    printf("Enter 3 subjects for attendance:\n");
    for (int i = 0; i < 3; i++) {
        printf("Enter subject %d: ", i + 1);
        scanf(" %49[^\n]", subjects[i]);
    }
    subjectCount = 3;

    while (1) {
        printf("\n1. Add Student\n");
        printf("2. Search Student by ID\n");
        printf("3. Search Student by Name\n");
        printf("4. Edit Student Info\n");
        printf("5. Delete Student\n");
        printf("6. Display Students\n");
        printf("7. Display Divisions\n");
        printf("8. Mark Attendance\n");
        printf("9. Display Attendance Summary\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    char name[100], course[50];
                    printf("Enter name: ");
                    scanf(" %99[^\n]", name);
                    printf("Enter course: ");
                    scanf(" %49[^\n]", course);
                    insertStudent(&head, name, course);
                    break;
                }
            case 2:
                printf("Enter student ID to search: ");
                scanf("%d", &studentId);
                {
                    Student* student = searchStudent(head, studentId);
                    if (student != NULL) {
                        printf("ID: %d, Name: %s, Course: %s, Division: %c\n", student->id, student->name, student->course, student->division);
                    } else {
                        printf("Student not found.\n");
                    }
                }
                break;
            case 8:
                markAttendance(head);  // Use the markAttendance function here
                break;
            case 9:
                displayAttendanceSummary(head);  // Use the displayAttendanceSummary function here
                break;
            case 10:
                exit(0);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
