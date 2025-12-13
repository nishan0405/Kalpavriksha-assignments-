#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data.h"

int main() {
    initHashTable();
    initQueue(&readyQueue);
    initQueue(&waitingQueue);
    initQueue(&terminatedQueue);
    int processCount = readProcesses();
    readKillEvents();
    runSimulation(processCount);
    printFinalResults();
    freeAllMemory();
    return 0;
}

unsigned int hashFunction(int pid) {
    return (pid % HASH_TABLE_SIZE);
}

void initHashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

PCB *findPCB(int pid) {
    unsigned int index = hashFunction(pid);
    HashNode *current = hashTable[index];
    while (current != NULL) {
        if (current->pcb->pid == pid) {
            return current->pcb;
        }
        current = current->next;
    }
    return NULL;
}

void insertPCB(PCB *pcb) {
    unsigned int index = hashFunction(pcb->pid);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->pcb = pcb;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

void removePCB(int pid) {
    unsigned int index = hashFunction(pid);
    HashNode *current = hashTable[index];
    HashNode *prev = NULL;
    while (current != NULL) {
        if (current->pcb->pid == pid) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

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

void addKillEvent(int pid, int tick) {
    if (killEventCount >= MAX_KILL_EVENTS) {
        printf("Maximum kill events reached, ignoring KILL %d %d\n", pid, tick);
        return;
    }
    killEvents[killEventCount].pid = pid;
    killEvents[killEventCount].tick = tick;
    killEventCount++;
}

int getKillsAtTick(int tick, int *killedPids, int *killedCount) {
    int found = 0;
    *killedCount = 0;
    for (int i = 0; i < killEventCount; i++) {
        if (killEvents[i].tick == tick) {
            killedPids[*killedCount] = killEvents[i].pid;
            (*killedCount)++;
            found = 1;
        }
    }
    return found;
}

void applyKillForPid(PCB *runningProcess, int currentTick, int pid) {
    PCB *process = findPCB(pid);
    if (process == NULL || process->state == STATE_TERMINATED) {
        return;
    }
    if (process != runningProcess) {
        removeFromQueue(&readyQueue, process);
        removeFromQueue(&waitingQueue, process);
    }
    process->state = STATE_TERMINATED;
    process->termType = TERM_KILLED;
    process->completionTime = currentTick;
    enqueue(&terminatedQueue, process);
    removePCB(pid);
}

int allProcessesTerminated(int totalProcesses) {
    int terminatedCount = 0;
    QueueNode *node = terminatedQueue.front;
    while (node != NULL) {
        terminatedCount++;
        node = node->next;
    }
    return terminatedCount == totalProcesses;
}

void logTimeline(int tick, PCB *runningProcess) {
    printf("[TICK %d] ", tick);
    if (runningProcess != NULL) {
        printf("RUNNING: %s(%d) ", runningProcess->name, runningProcess->pid);
    } else {
        printf("IDLE ");
    }
    printf("READY: %d ", isQueueEmpty(&readyQueue) ? 0 : 1);
    printf("WAITING: %d\n", isQueueEmpty(&waitingQueue) ? 0 : 1);
}

void updateWaitingIoProcesses() {
    QueueNode *node = waitingQueue.front;
    while (node != NULL) {
        PCB *p = node->process;
        if (p->remainingIo > 0) {
            p->remainingIo--;
        }
        node = node->next;
    }
}

void moveCompletedIoToReady() {
    QueueNode *prev = NULL;
    QueueNode *cur = waitingQueue.front;
    while (cur != NULL) {
        PCB *p = cur->process;
        if (p->remainingIo <= 0) {
            p->state = STATE_READY;
            if (prev == NULL) {
                waitingQueue.front = cur->next;
            } else {
                prev->next = cur->next;
            }
            if (cur == waitingQueue.rear) {
                waitingQueue.rear = prev;
            }
            QueueNode *toFree = cur;
            cur = cur->next;
            free(toFree);
            enqueue(&readyQueue, p);
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

int readProcesses() {
    char input[256];
    int processCount = 0;
    printf("Enter process details (name pid cpuBurst ioStart ioDuration) or END:\n");
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "END") == 0) {
            break;
        }
        PCB *p = (PCB *)malloc(sizeof(PCB));
        if (sscanf(input, "%s %d %d %d %d",
                   p->name, &p->pid, &p->cpuBurst, &p->ioStart, &p->ioDuration) == 5) {
            p->remainingCpu = p->cpuBurst;
            p->remainingIo = p->ioDuration;
            p->state = STATE_READY;
            p->termType = TERM_NORMAL;
            p->firstStartTime = -1;
            p->completionTime = -1;
            p->arrivalTime = 0;
            p->hasIoStarted = 0;
            p->next = NULL;
            insertPCB(p);
            enqueue(&readyQueue, p);
            processCount++;
            printf("Added process %s(%d)\n", p->name, p->pid);
        } else {
            free(p);
        }
    }
    return processCount;
}

void readKillEvents() {
    char input[256];
    printf("Enter kill events (KILL pid tick) or END:\n");
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "END") == 0) {
            break;
        }
        if (strncmp(input, "KILL", 4) == 0) {
            int pid, tick;
            if (sscanf(input + 4, "%d %d", &pid, &tick) == 2) {
                addKillEvent(pid, tick);
            }
        }
    }
}

void runSimulation(int processCount) {
    int currentTick = 0;
    PCB *runningProcess = NULL;
    printf("\n=== SIMULATION START ===\n");
    while (!allProcessesTerminated(processCount)) {
        logTimeline(currentTick, runningProcess);

        int killedPids[MAX_KILL_EVENTS];
        int killedCount = 0;
        if (getKillsAtTick(currentTick, killedPids, &killedCount)) {
            for (int k = 0; k < killedCount; k++) {
                int pid = killedPids[k];
                if (runningProcess && runningProcess->pid == pid) {
                    runningProcess->state = STATE_TERMINATED;
                    runningProcess->termType = TERM_KILLED;
                    runningProcess->completionTime = currentTick;
                    enqueue(&terminatedQueue, runningProcess);
                    runningProcess = NULL;
                } else {
                    applyKillForPid(runningProcess, currentTick, pid);
                }
            }
        }
        if (runningProcess == NULL) {
            runningProcess = dequeue(&readyQueue);
            if (runningProcess != NULL) {
                runningProcess->state = STATE_RUNNING;
                if (runningProcess->firstStartTime == -1) {
                    runningProcess->firstStartTime = currentTick;
                }
            }
        }
        if (runningProcess != NULL) {
            runningProcess->remainingCpu--;
            int cpuUsedSoFar = runningProcess->cpuBurst - runningProcess->remainingCpu;

            if (!runningProcess->hasIoStarted && runningProcess->ioDuration > 0 && cpuUsedSoFar >= runningProcess->ioStart) {
                runningProcess->hasIoStarted = 1;
                runningProcess->state = STATE_WAITING;
                enqueue(&waitingQueue, runningProcess);
                runningProcess = NULL;
            } else if (runningProcess->remainingCpu <= 0) {
                runningProcess->state = STATE_TERMINATED;
                runningProcess->termType = TERM_NORMAL;
                runningProcess->completionTime = currentTick + 1;
                enqueue(&terminatedQueue, runningProcess);
                runningProcess = NULL;
            }
        }
        updateWaitingIoProcesses();
        moveCompletedIoToReady();
        sleep(1);
        currentTick++;
    }
}

void printFinalResults() {
    printf("\nPID\tName\tCPU\tIO\t");
    printf("Status\t\tTurnaround\tWaiting\n");

    QueueNode *node = terminatedQueue.front;
    while (node != NULL) {
        PCB *p = node->process;
        int turnaround = -1;
        int waitingTime = -1;

        if (p->termType == TERM_NORMAL) {
            turnaround = p->completionTime - p->arrivalTime;
            waitingTime = turnaround - p->cpuBurst - p->ioDuration;
            if (waitingTime < 0) {
                waitingTime = 0;
            }
        }
        if (p->termType == TERM_KILLED) {
            printf("%d\t%s\t%d\t%d\tKILLED at %d\t-\t\t-\n",
                   p->pid, p->name, p->cpuBurst, p->ioDuration, p->completionTime);
        } else {
            printf("%d\t%s\t%d\t%d\tOK\t\t%d\t\t%d\n",
                   p->pid, p->name, p->cpuBurst, p->ioDuration, turnaround, waitingTime);
        }
        node = node->next;
    }
}

void freeAllMemory() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *current = hashTable[i];
        while (current != NULL) {
            free(current->pcb);
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    QueueNode *node = terminatedQueue.front;
    while (node != NULL) {
        QueueNode *tmp = node;
        node = node->next;
        free(tmp);
    }
}

