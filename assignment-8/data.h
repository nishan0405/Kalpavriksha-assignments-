#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 32
#define MAX_PROCESSES 100
#define MAX_PID 10000
#define MAX_KILL_EVENTS 100
#define HASH_TABLE_SIZE 1009

typedef enum {
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED
} ProcessState;

typedef enum {
    TERM_NORMAL,
    TERM_KILLED
} TerminationType;

typedef struct ProcessControlBlock {
    int pid;
    char name[MAX_NAME_LEN];
    int cpuBurst;
    int ioStart;
    int ioDuration;
    int remainingCpu;
    int remainingIo;
    ProcessState state;
    TerminationType termType;
    int firstStartTime;
    int completionTime;
    int arrivalTime;
    int hasIoStarted;
    struct ProcessControlBlock *next;
} PCB;

typedef struct QueueNode {
    PCB *process;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

typedef struct {
    int pid;
    int tick;
} KillEvent;

typedef struct HashNode {
    PCB *pcb;
    struct HashNode *next;
} HashNode;

HashNode *hashTable[HASH_TABLE_SIZE];
Queue readyQueue;
Queue waitingQueue;
Queue terminatedQueue;
KillEvent killEvents[MAX_KILL_EVENTS];
int killEventCount = 0;

unsigned int hashFunction(int pid);
void initHashTable(void);
PCB *findPCB(int pid);
void insertPCB(PCB *pcb);
void removePCB(int pid);
void initQueue(Queue *q);
int  isQueueEmpty(Queue *q);
void enqueue(Queue *q, PCB *process);
void removeFromQueue(Queue *q, PCB *process);
PCB *dequeue(Queue *q);
void addKillEvent(int pid, int tick);
int  getKillsAtTick(int tick, int *killedPids, int *killedCount);
void applyKillForPid(PCB *runningProcess, int currentTick, int pid);
int  allProcessesTerminated(int totalProcesses);
void logTimeline(int tick, PCB *runningProcess);
void updateWaitingIoProcesses(void);
void moveCompletedIoToReady(void);
int  readProcesses(void);
void readKillEvents(void);
void runSimulation(int processCount);
void printFinalResults(void);
void freeAllMemory(void);

#endif