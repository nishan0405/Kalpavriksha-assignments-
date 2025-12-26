#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "hash.h"

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
