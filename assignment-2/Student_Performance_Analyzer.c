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
        printf("\nEnter Roll No, Name, and Marks in 3 subjects: ");
        scanf("%d %s %f %f %f", &students[i].roll, students[i].name,
              &students[i].marks[0], &students[i].marks[1], &students[i].marks[2]);

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

    printf("\nList of Roll Numbers : ");
    printRollNumbers(0, n);

    return 0;
}
