
#ifndef STR_H
#define STR_H

#include <stdio.h>


struct Student {
    int roll;
    char name[50];
    float marks[3];
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
    return s.marks[0] + s.marks[1] + s.marks[2];
}

float calculateAverage(struct Student s) {
    return (s.marks[0] + s.marks[1] + s.marks[2]) / 3.0;
}

char assignGrade(float avg) {
    if (avg >= 85)
        return 'A';
    else if (avg >= 70)
        return 'B';
    else if (avg >= 50)
        return 'C';
    else if (avg >= 35)
        return 'D';
    else
        return 'F';
}

void printStars(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0; break;
    }

    for (int i = 0; i < stars; i++)
        printf("*");
}

void printRollNumbers(int i, int n) {
    if (i == n)
        return;
    printf("%d ", i + 1);
    printRollNumbers(i + 1, n);
}

#endif
