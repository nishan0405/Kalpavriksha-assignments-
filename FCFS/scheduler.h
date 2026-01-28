#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "data.h"

void initHashTable(void);
PCB* findPCB(int pid);
void insertPCB(PCB *pcb);
void removePCB(int pid);
int isQueueEmpty(Queue *q);
void enqueue(Queue *q, PCB *process);
void removeFromQueue(Queue *q, PCB *process);
PCB* dequeue(Queue *q);
void addKillEvent(int pid, int tick);
int getKillsAtTick(int tick, int *killedPids, int *killedCount);
void applyKillForPid(PCB *runningProcess, int currentTick, int pid);
int allProcessesTerminated(int totalProcesses);
void logTimeline(int tick, PCB *runningProcess);
void updateWaitingIoProcesses(void);
void moveCompletedIoToReady(void);
int readProcesses(void);
void readKillEvents(void);
void runSimulation(int processCount);
void printFinalResults(void);
void freeAllMemory(void);

#endif
