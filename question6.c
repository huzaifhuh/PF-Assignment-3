#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char name[100];
    char batch[20];      // CS, SE, Cyber Security, AI
    char membership[10]; // IEEE or ACM
    char regDate[12];    // YYYY-MM-DD
    char dob[12];        // YYYY-MM-DD
    char interest[10];   // IEEE, ACM, Both
} Student;

Student *students = NULL;
int totalStudents = 0;
int capacity = 0;
const char *filename = "Question6_members.dat";


void grow()
{
    capacity = (capacity == 0) ? 10 : capacity * 2;
    students = (Student *)realloc(students, capacity * sizeof(Student));
    if (!students)
    {
        printf("ERROR: Out of memory!\n");
        exit(1);
    }
}

void loadDatabase()
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("No existing database found. Starting fresh.\n");
        return;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, fp))
    {
        if (totalStudents == capacity)
            grow();
        students[totalStudents++] = s;
    }
    fclose(fp);
    printf("Loaded %d records from %s\n", totalStudents, filename);
}

void saveDatabase()
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("ERROR: Cannot save database!\n");
        return;
    }
    fwrite(students, sizeof(Student), totalStudents, fp);
    fclose(fp);
    printf("Database saved successfully.\n");
}

int findStudent(int id)
{
    for (int i = 0; i < totalStudents; i++)
    {
        if (students[i].id == id)
            return i;
    }
    return -1;
}

void addStudent()
{
    if (totalStudents == capacity)
        grow();

    Student s;
    printf("Enter Student ID: ");
    scanf("%d", &s.id);
    if (findStudent(s.id) != -1)
    {
        printf("ERROR: Student ID %d already exists!\n", s.id);
        return;
    }

    printf("Full Name: ");
    scanf(" %99[^\n]", s.name);
    printf("Batch (CS/SE/Cyber Security/AI): ");
    scanf("%s", s.batch);
    printf("Membership (IEEE/ACM): ");
    scanf("%s", s.membership);
    printf("Registration Date (YYYY-MM-DD): ");
    scanf("%s", s.regDate);
    printf("Date of Birth (YYYY-MM-DD): ");
    scanf("%s", s.dob);
    printf("Interest (IEEE/ACM/Both): ");
    scanf("%s", s.interest);

    students[totalStudents++] = s;
    saveDatabase();
    printf("Student registered successfully!\n");
}

void updateStudent()
{
    int id;
    printf("Enter Student ID to update: ");
    scanf("%d", &id);
    int pos = findStudent(id);
    if (pos == -1)
    {
        printf("Student not found!\n");
        return;
    }

    printf("Current: %s | Batch: %s | Membership: %s\n",
           students[pos].name, students[pos].batch, students[pos].membership);

    printf("New Batch: ");
    scanf("%s", students[pos].batch);
    printf("New Membership (IEEE/ACM): ");
    scanf("%s", students[pos].membership);

    saveDatabase();
    printf("Student updated!\n");
}

void deleteStudent()
{
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);
    int pos = findStudent(id);
    if (pos == -1)
    {
        printf("Student not found!\n");
        return;
    }

    for (int i = pos; i < totalStudents - 1; i++)
    {
        students[i] = students[i + 1];
    }
    totalStudents--;


    saveDatabase();
    printf("Student deleted successfully!\n");
}

void viewAll()
{
    if (totalStudents == 0)
    {
        printf("No students registered yet.\n");
        return;
    }
    printf("\n=== ALL REGISTERED STUDENTS ===\n");
    for (int i = 0; i < totalStudents; i++)
    {
        printf("%d | %s | %s | %s | %s\n",
               students[i].id, students[i].name, students[i].batch,
               students[i].membership, students[i].interest);
    }
}

void batchReport()
{
    char batch[20];
    printf("Enter batch (CS/SE/Cyber Security/AI): ");
    scanf("%s", batch);

    printf("\n=== %s BATCH REPORT ===\n", batch);
    int found = 0;
    for (int i = 0; i < totalStudents; i++)
    {
        if (strcmp(students[i].batch, batch) == 0)
        {
            printf("%d | %s | %s | Interest: %s\n",
                   students[i].id, students[i].name, students[i].membership, students[i].interest);
            found = 1;
        }
    }
    if (!found)
        printf("No students found in %s batch.\n", batch);
}

int main()
{
    printf("=== IEEE / ACM MEMBERSHIP SYSTEM ===\n");
    loadDatabase();

    int choice;
    do
    {
        printf("\n1. Register Student\n");
        printf("2. Update Student\n");
        printf("3. Delete Student\n");
        printf("4. View All\n");
        printf("5. Batch Report\n");
        printf("6. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            updateStudent();
            break;
        case 3:
            deleteStudent();
            break;
        case 4:
            viewAll();
            break;
        case 5:
            batchReport();
            break;
        case 6:
            saveDatabase();
            printf("Thank you! Goodbye.\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 6);

    if (students)
        free(students);
    return 0;
}