#ifndef QUEUE_H
#define QUEUE_H

#include "data.h"

void initQueue(Queue *q);
int isQueueEmpty(Queue *q);
void enqueue(Queue *q, PCB *process);
void removeFromQueue(Queue *q, PCB *process);
PCB *dequeue(Queue *q);

#endif
