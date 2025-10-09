#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"

#define BUF_SIZE 1024

static int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

static int apply_op(int a, int b, char op, int *errorFlag) {
    int result = 0;

    switch (op) {
        case '+': {
            result = a + b;
            break;
        }
        case '-': {
            result = a - b;
            break;
        }
        case '*': {
            result = a * b;
            break;
        }
        case '/': {
            if (b == 0) {
                *errorFlag = 1;  /* Division by zero */
                return -1;
            }
            result = a / b;
            break;
        }
        default: {
            *errorFlag = 2;  /* Invalid operator */
            return -1;
        }
    }

    return result;
}

static int applyTopOperator(IntStack *nums, CharStack *ops, int *errorFlag) {
    char op;
    int right, left, res;

    if (!charStackPop(ops, &op)) { 
        return 0;
    }
    if (!intStackPop(nums, &right)) {
        *errorFlag = 1;
        return 0;
    } 
    if (!intStackPop(nums, &left)) {
        *errorFlag = 1;
        return 0; 
    } 
    res = apply_op(left, right, op, errorFlag);
    if (*errorFlag != 0) {
        return 0;
    }
    if (!intStackPush(nums, res)) { 
        *errorFlag = 1;
        return 0;
    } 
    return 1;
}

int evaluate_expression(const char *expr, int *outResult, int *errorFlag) {
    IntStack nums;
    CharStack ops;
    intStackInit(&nums);
    charStackInit(&ops);
    *errorFlag = 0;

    size_t i = 0, n = strlen(expr);

    while (i < n) {
        char ch = expr[i];

        if (isspace((unsigned char)ch)) {
            i++;
            continue;
        }

        if (isdigit((unsigned char)ch)) {
            long long val = 0;
            int digits = 0;
            while (i < n && isdigit((unsigned char)expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
                digits++;
                if (val > INT_MAX) {
                    *errorFlag = 1;
                    return 0;
                }
            }
            if (!intStackPush(&nums, (int)val)) {
                *errorFlag = 1;
                return 0;
            }
            continue;
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            char topOp;
            while (!charStackIsEmpty(&ops) && charStackPeek(&ops, &topOp) && precedence(topOp) >= precedence(ch)) {
                if (!applyTopOperator(&nums, &ops, errorFlag)) {
                    *errorFlag = 3;  
                    return 0;
                }
                if (*errorFlag != 0) {
                    return 0;
                }
            }
            if (!charStackPush(&ops, ch)) {
                *errorFlag = 1;
                return 0;
            }
            i++;
            continue;
        }

        *errorFlag = 1;
        return 0;
    }

    while (!charStackIsEmpty(&ops)) {
        if (!applyTopOperator(&nums, &ops, errorFlag)) {
            return 0;
        }
        if (*errorFlag) {
            return 0;
        }
    }

    if (nums.top != 0) {
        *errorFlag = 1; 
        return 0;
    }
    if (!intStackPop(&nums, outResult)) { 
        *errorFlag = 1; 
        return 0; 
    }

    return 1;
}

int main(void) {
    char buf[BUF_SIZE];

    printf("Enter a mathematical expression (digits and + - * / only):\n");
    if (!fgets(buf, sizeof(buf), stdin)) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }

    buf[strcspn(buf, "\n")] = '\0';

    int errorFlag = 0;
    int result = 0;
    if (!evaluate_expression(buf, &result, &errorFlag)) {
        switch (errorFlag) {
            case 1:
                fprintf(stderr, "Error: Division by zero occurred in the expression.\n");
                break;
            case 2:
                fprintf(stderr, "Error: Expression contains invalid characters.\n");
                break;
            case 3:
                fprintf(stderr, "Error: Malformed expression (e.g., missing operand or operator).\n");
                break;
            default:
                fprintf(stderr, "Error: Unknown evaluation error.\n");
        }
        return EXIT_FAILURE;
    }

    printf("Result: %d\n", result);
    return EXIT_SUCCESS;
}
