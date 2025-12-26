#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data.h"
#include "scheduler.h"
#include "queue.h"
#include "hash.h"

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
