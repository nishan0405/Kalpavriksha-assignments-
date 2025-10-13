#ifndef STR_H
#define STR_H

#include <stdio.h>

#define MAX_SUBJECTS 10
#define MAX_GRADE_LEVELS 5

struct Student {
    int roll;
    char name[50];
    float marks[MAX_SUBJECTS];
    int numSubjects;
    float total;
    float avg;
    char grade;
};

float calculateTotal(struct Student s);
float calculateAverage(struct Student s);
char assignGrade(float avg);
void printStars(char grade);
void printRollNumbers(int i, int n);

float calculateTotal(struct Student s) {
    float total = 0;
    for (int i = 0; i < s.numSubjects; i++) {
        total += s.marks[i];
    }
    return total;
}

float calculateAverage(struct Student s) {
    if (s.numSubjects == 0) return 0;
    return s.total / s.numSubjects;
}

char assignGrade(float avg) {
    const float thresholds[MAX_GRADE_LEVELS] = {85, 70, 50, 35, 0};
    const char grades[MAX_GRADE_LEVELS] = {'A', 'B', 'C', 'D', 'F'};

    for (int i = 0; i < MAX_GRADE_LEVELS; i++) {
        if (avg >= thresholds[i])
            return grades[i];
    }
    return 'F';
}

void printStars(char grade) {
    const char gradeLevels[] = {'A', 'B', 'C', 'D', 'F'};
    const int starsMap[] = {5, 4, 3, 2, 0};

    for (int i = 0; i < sizeof(gradeLevels); i++) {
        if (grade == gradeLevels[i]) {
            for (int j = 0; j < starsMap[i]; j++)
                printf("*");
            return;
        }
    }
}

void printRollNumbers(int i, int n) {
    if (i == n)
        return;
    printf("%d ", i + 1);
    printRollNumbers(i + 1, n);
}

#endif
