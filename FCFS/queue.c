#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "queue.h"

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

int isQueueEmpty(Queue *q) {
    return q->front == NULL;
}

void enqueue(Queue *q, PCB *process) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->process = process;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

void removeFromQueue(Queue *q, PCB *process) {
    QueueNode *prev = NULL;
    QueueNode *cur = q->front;
    while (cur != NULL) {
        if (cur->process == process) {
            if (prev == NULL) {
                q->front = cur->next;
            } else {
                prev->next = cur->next;
            }
            if (cur == q->rear) {
                q->rear = prev;
            }
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

PCB *dequeue(Queue *q) {
    if (q->front == NULL) {
        return NULL;
    }
    QueueNode *node = q->front;
    PCB *p = node->process;
    q->front = node->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(node);
    return p;
}
