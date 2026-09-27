#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Student {
    int rollNo;
    char name[50];
    int semester;
    float marks[5];
    float total;
    float percentage;
    char grade;
    char result[10];
};

struct Student students[MAX];
int count = 0;

void calculateResult(struct Student *s) {
    int i;

    s->total = 0;

    for (i = 0; i < 5; i++)
        s->total += s->marks[i];

    s->percentage = s->total / 5;

    if (s->percentage >= 90)
        s->grade = 'A';
    else if (s->percentage >= 80)
        s->grade = 'B';
    else if (s->percentage >= 70)
        s->grade = 'C';
    else if (s->percentage >= 60)
        s->grade = 'D';
    else if (s->percentage >= 50)
        s->grade = 'E';
    else
        s->grade = 'F';

    for (i = 0; i < 5; i++) {
        if (s->marks[i] < 35) {
            strcpy(s->result, "FAIL");
            return;
        }
    }

    strcpy(s->result, "PASS");
}

void loadData() {
    FILE *file;
    char line[300];

    file = fopen("student_academic_performance_analyzer.csv", "r");

    if (file == NULL) {
        printf("CSV file not found.\n");
        return;
    }

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < MAX) {
        sscanf(line, "%d,%49[^,],%d,%f,%f,%f,%f,%f",
               &students[count].rollNo,
               students[count].name,
               &students[count].semester,
               &students[count].marks[0],
               &students[count].marks[1],
               &students[count].marks[2],
               &students[count].marks[3],
               &students[count].marks[4]);

        calculateResult(&students[count]);
        count++;
    }

    fclose(file);
}

void saveData() {
    FILE *file;
    int i;

    file = fopen("student_academic_performance_analyzer.csv", "w");

    if (file == NULL) {
        printf("Unable to save data.\n");
        return;
    }

    fprintf(file, "Roll_No,Name,Semester,Subject_1,Subject_2,Subject_3,Subject_4,Subject_5\n");

    for (i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%d,%.1f,%.1f,%.1f,%.1f,%.1f\n",
                students[i].rollNo,
                students[i].name,
                students[i].semester,
                students[i].marks[0],
                students[i].marks[1],
                students[i].marks[2],
                students[i].marks[3],
                students[i].marks[4]);
    }

    fclose(file);
}

void addStudent() {
    int i, roll;

    if (count >= MAX) {
        printf("\nStudent limit reached.\n");
        return;
    }

    printf("\nEnter Roll No: ");
    scanf("%d", &roll);

    for (i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {
            printf("\nRoll No already exists.\n");
            return;
        }
    }

    students[count].rollNo = roll;

    printf("Enter Name: ");
    scanf(" %[^\n]", students[count].name);

    printf("Enter Semester: ");
    scanf("%d", &students[count].semester);

    printf("Enter marks for 5 subjects:\n");

    for (i = 0; i < 5; i++) {
        printf("Subject %d: ", i + 1);
        scanf("%f", &students[count].marks[i]);
    }

    calculateResult(&students[count]);

    count++;

    saveData();

    printf("\nStudent added successfully.\n");
}

void deleteStudent() {
    int roll, i, j;

    printf("\nEnter Roll No to delete: ");
    scanf("%d", &roll);

    for (i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {

            for (j = i; j < count - 1; j++)
                students[j] = students[j + 1];

            count--;

            saveData();

            printf("\nStudent deleted successfully.\n");
            return;
        }
    }

    printf("\nStudent not found.\n");
}

void displayStudents() {
    int i;

    if (count == 0) {
        printf("\nNo student records available.\n");
        return;
    }

    printf("\n%-8s %-15s %-10s %-10s %-12s %-8s %-8s\n",
           "Roll", "Name", "Semester", "Total",
           "Percentage", "Grade", "Result");

    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-8d %-15s %-10d %-10.1f %-12.1f %-8c %-8s\n",
               students[i].rollNo,
               students[i].name,
               students[i].semester,
               students[i].total,
               students[i].percentage,
               students[i].grade,
               students[i].result);
    }
}

void searchStudent() {
    int roll, i;

    printf("\nEnter Roll No: ");
    scanf("%d", &roll);

    for (i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {

            printf("\nStudent Details\n");
            printf("-------------------------\n");
            printf("Roll No     : %d\n", students[i].rollNo);
            printf("Name        : %s\n", students[i].name);
            printf("Semester    : %d\n", students[i].semester);
            printf("Subject 1   : %.1f\n", students[i].marks[0]);
            printf("Subject 2   : %.1f\n", students[i].marks[1]);
            printf("Subject 3   : %.1f\n", students[i].marks[2]);
            printf("Subject 4   : %.1f\n", students[i].marks[3]);
            printf("Subject 5   : %.1f\n", students[i].marks[4]);
            printf("Total       : %.1f\n", students[i].total);
            printf("Percentage  : %.1f%%\n", students[i].percentage);
            printf("Grade       : %c\n", students[i].grade);
            printf("Result      : %s\n", students[i].result);

            return;
        }
    }

    printf("\nStudent not found.\n");
}

void performanceAnalysis() {
    int i;
    int pass = 0, fail = 0;
    int gradeA = 0, gradeB = 0, gradeC = 0;
    int gradeD = 0, gradeE = 0, gradeF = 0;
    int highestIndex = 0;
    float average = 0;

    if (count == 0) {
        printf("\nNo data available.\n");
        return;
    }

    for (i = 0; i < count; i++) {

        average += students[i].percentage;

        if (strcmp(students[i].result, "PASS") == 0)
            pass++;
        else
            fail++;

        if (students[i].percentage >
            students[highestIndex].percentage)
            highestIndex = i;

        switch (students[i].grade) {
            case 'A': gradeA++; break;
            case 'B': gradeB++; break;
            case 'C': gradeC++; break;
            case 'D': gradeD++; break;
            case 'E': gradeE++; break;
            case 'F': gradeF++; break;
        }
    }

    average = average / count;

    printf("\n====================================\n");
    printf("       PERFORMANCE ANALYSIS\n");
    printf("====================================\n");

    printf("Total Students   : %d\n", count);
    printf("Average          : %.1f%%\n", average);
    printf("Highest          : %.1f%%\n",
           students[highestIndex].percentage);
    printf("Top Student      : %s\n",
           students[highestIndex].name);
    printf("Passed Students  : %d\n", pass);
    printf("Failed Students  : %d\n", fail);

    printf("\nGrade Distribution\n");
    printf("-------------------------\n");
    printf("A : %d\n", gradeA);
    printf("B : %d\n", gradeB);
    printf("C : %d\n", gradeC);
    printf("D : %d\n", gradeD);
    printf("E : %d\n", gradeE);
    printf("F : %d\n", gradeF);
}

void subjectAnalysis() {
    int i, j;
    float average[5] = {0};

    if (count == 0) {
        printf("\nNo data available.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        for (j = 0; j < 5; j++)
            average[j] += students[i].marks[j];
    }

    printf("\n====================================\n");
    printf("         SUBJECT ANALYSIS\n");
    printf("====================================\n");

    for (i = 0; i < 5; i++)
        printf("Subject %d Average : %.1f\n",
               i + 1, average[i] / count);
}

int main() {
    int choice;

    loadData();

    printf("\n==============================================\n");
    printf(" STUDENT ACADEMIC PERFORMANCE ANALYZER\n");
    printf("==============================================\n");

    printf("%d student records loaded from CSV.\n", count);

    do {
        printf("\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Display All Students\n");
        printf("4. Search Student\n");
        printf("5. Performance Analysis\n");
        printf("6. Subject Analysis\n");
        printf("7. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addStudent();
                break;

            case 2:
                deleteStudent();
                break;

            case 3:
                displayStudents();
                break;

            case 4:
                searchStudent();
                break;

            case 5:
                performanceAnalysis();
                break;

            case 6:
                subjectAnalysis();
                break;

            case 7:
                printf("\nProgram terminated.\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while (choice != 7);

    return 0;
}