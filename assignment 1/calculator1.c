#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 200

void removeSpaces(char *src) {
    int i = 0, j = 0;
    while (src[i] != '\0') {
        if (src[i] != ' ')
            src[j++] = src[i];
        i++;
    }
    src[j] = '\0';
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int tokenize(char *expr, int nums[], char ops[], int *numCount, int *opCount) {
    int i = 0;
    int n = strlen(expr);
    *numCount = 0;
    *opCount = 0;

    while (i < n) {
        if (isdigit(expr[i])) {
            int val = 0;
            while (i < n && isdigit(expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
            nums[(*numCount)++] = val;
        } 
        else if (isOperator(expr[i])) {
            ops[(*opCount)++] = expr[i];
            i++;
        } 
        else {
            return 0;
        }
    }
    return 1;
}

int applyOp(int a, int b, char op, int *errorFlag) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                *errorFlag = 1;
                return 0;
            }
            return a / b;
    }
    return 0;
}

int evaluate(int nums[], char ops[], int numCount, int opCount, int *errorFlag) {
    for (int i = 0; i < opCount; i++) {
        if (ops[i] == '*' || ops[i] == '/') {
            int result = applyOp(nums[i], nums[i + 1], ops[i], errorFlag);
            if (*errorFlag) return 0;
            nums[i] = result;
            for (int j = i + 1; j < numCount - 1; j++)
                nums[j] = nums[j + 1];
            for (int j = i; j < opCount - 1; j++)
                ops[j] = ops[j + 1];
            numCount--;
            opCount--;
            i--;
        }
    }

    int result = nums[0];
    for (int i = 0; i < opCount; i++) {
        result = applyOp(result, nums[i + 1], ops[i], errorFlag);
        if (*errorFlag) return 0;
    }
    return result;
}

int main() {
    char expr[MAX];
    int numbers[MAX], numCount;
    char operators[MAX];
    int opCount;
    int errorFlag = 0;

    printf("Enter a mathematical expression: ");
    if (!fgets(expr, sizeof(expr), stdin)) {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    expr[strcspn(expr, "\n")] = '\0';
    removeSpaces(expr);

    if (!tokenize(expr, numbers, operators, &numCount, &opCount)) {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    if (numCount == 0 || numCount != opCount + 1) {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    int result = evaluate(numbers, operators, numCount, opCount, &errorFlag);
    if (errorFlag) {
        printf("Error: Division by zero.\n");
    } else {
        printf("Result: %d\n", result);
    }

    return 0;
}
