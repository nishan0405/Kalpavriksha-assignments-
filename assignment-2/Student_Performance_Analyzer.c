#include <stdio.h>
#include "str.h"

int main() {
    struct Student students[100];
    int n;

    printf("Enter number of students: ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid number of students.\n");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        printf("\n--- Enter details for Student %d ---\n", i + 1);

        do {
            printf("Enter Roll No (positive integer): ");
            scanf("%d", &students[i].roll);
            if (students[i].roll <= 0)
                printf("Invalid roll number! Please enter a positive integer.\n");
        } while (students[i].roll <= 0);

        printf("Enter Name: ");
        scanf("%s", students[i].name);

        students[i].numSubjects = 3;
        for (int j = 0; j < students[i].numSubjects; j++) {
            do {
                printf("Enter marks for Subject %d (0–100): ", j + 1);
                scanf("%f", &students[i].marks[j]);
                if (students[i].marks[j] < 0 || students[i].marks[j] > 100)
                    printf("Invalid marks! Please enter between 0 and 100.\n");
            } while (students[i].marks[j] < 0 || students[i].marks[j] > 100);
        }

        students[i].total = calculateTotal(students[i]);
        students[i].avg = calculateAverage(students[i]);
        students[i].grade = assignGrade(students[i].avg);
    }

    printf("\n----- Student Performance -----\n");

    for (int i = 0; i < n; i++) {
        printf("\nRoll: %d", students[i].roll);
        printf("\nName: %s", students[i].name);
        printf("\nTotal: %.2f", students[i].total);
        printf("\nAverage: %.2f", students[i].avg);
        printf("\nGrade: %c", students[i].grade);

        if (students[i].avg < 35)
            continue;

        printf("\nPerformance: ");
        printStars(students[i].grade);
        printf("\n");
    }

    printf("\nList of Roll Numbers: ");
    printRollNumbers(0, n);

    return 0;
}
