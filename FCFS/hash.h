#ifndef HASH_H
#define HASH_H

#include "data.h"

unsigned int hashFunction(int pid);
void initHashTable(void);
PCB* findPCB(int pid);
void insertPCB(PCB *pcb);
void removePCB(int pid);

#endif
