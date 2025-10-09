#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_STACK
#define MAX_STACK 200
#endif

typedef struct {
    int data[MAX_STACK];
    int top;
} IntStack;

static inline void intStackInit(IntStack *s) { s->top = -1; }
static inline int intStackIsEmpty(const IntStack *s) { return s->top == -1; }
static inline int intStackIsFull(const IntStack *s) { return s->top == MAX_STACK - 1; }
static inline int intStackPush(IntStack *s, int v) {
    if (intStackIsFull(s)) return 0;
    s->data[++s->top] = v;
    return 1;
}
static inline int intStackPop(IntStack *s, int *out) {
    if (intStackIsEmpty(s)) return 0;
    *out = s->data[s->top--];
    return 1;
}
static inline int intStackPeek(const IntStack *s, int *out) {
    if (intStackIsEmpty(s)) return 0;
    *out = s->data[s->top];
    return 1;
}

typedef struct {
    char data[MAX_STACK];
    int top;
} CharStack;

static inline void charStackInit(CharStack *s) { s->top = -1; }
static inline int charStackIsEmpty(const CharStack *s) { return s->top == -1; }
static inline int charStackIsFull(const CharStack *s) { return s->top == MAX_STACK - 1; }
static inline int charStackPush(CharStack *s, char v) {
    if (charStackIsFull(s)) return 0;
    s->data[++s->top] = v;
    return 1;
}
static inline int charStackPop(CharStack *s, char *out) {
    if (charStackIsEmpty(s)) return 0;
    *out = s->data[s->top--];
    return 1;
}
static inline int charStackPeek(const CharStack *s, char *out) {
    if (charStackIsEmpty(s)) return 0;
    *out = s->data[s->top];
    return 1;
}

#endif 
