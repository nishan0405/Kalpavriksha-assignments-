#include "data.h"

HashNode *hashTable[HASH_TABLE_SIZE];
Queue readyQueue;
Queue waitingQueue;
Queue terminatedQueue;
KillEvent killEvents[MAX_KILL_EVENTS];
int killEventCount = 0;
