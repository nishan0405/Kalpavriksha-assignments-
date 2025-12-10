#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

int main() {
    char command[20];
    LruCache *cache = NULL;
    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "createCache") == 0) {
            int size;
            scanf("%d", &size);
            cache = createCache(size);
        }
        else if (strcmp(command, "put") == 0) {
            int key;
            char value[Max_Size];
            scanf("%d %s", &key, value);
            put(cache, key, value);
        }
        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char *result = get(cache, key);
            if (result) {
                printf("%s\n", result);
            } else {
                printf("NULL\n");
            }
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}



int hash(int key) {
    return key % HashMap_Size;
}

LruCache *createCache(int capacity) {
    LruCache *cache = (LruCache*)malloc(sizeof(LruCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->hashMap = (Map*)malloc(sizeof(Map) * HashMap_Size);
    for (int i = 0; i < HashMap_Size; i++) {
        cache->hashMap[i].inUse = 0;
        cache->hashMap[i].node = NULL;
        cache->hashMap[i].key = -1;
    }
    return cache;
}

Node *findNode(LruCache *cache, int key) {
    int index = hash(key);
    int start = index;
    while (cache->hashMap[index].inUse) {
        if (cache->hashMap[index].key == key) {
            return cache->hashMap[index].node;
        }
        index = (index + 1) % HashMap_Size;
        if (index == start) {
            break;
        }
    }
    return NULL;
}

void setMap(LruCache *cache, int key, Node *node) {
    int index = hash(key);
    while (cache->hashMap[index].inUse && cache->hashMap[index].key != key) {
        index = (index + 1) % HashMap_Size;
    }
    cache->hashMap[index].inUse = 1;
    cache->hashMap[index].key = key;
    cache->hashMap[index].node = node;
}

void removeMap(LruCache *cache, int key) {
    int index = hash(key);
    int start = index;
    while (cache->hashMap[index].inUse) {
        if (cache->hashMap[index].key == key) {
            cache->hashMap[index].inUse = 0;
            cache->hashMap[index].node = NULL;
            cache->hashMap[index].key = -1;
            return;
        }
        index = (index + 1) % HashMap_Size;
        if (index == start) {
            break;
        }
    }
}

void moveToHead(LruCache *cache, Node *node) {
    if (cache->head == node) {
        return;
    }
    if (node->previous) {
        node->previous->next = node->next;
    }
    if (node->next) {
        node->next->previous = node->previous;
    }
    if (node == cache->tail) {
        cache->tail = node->previous;
    }
    node->previous = NULL;
    node->next = cache->head;
    if (cache->head) {
        cache->head->previous = node;
    }
    cache->head = node;
    if (!cache->tail) {
        cache->tail = node;
    }
}

void removeTail(LruCache *cache) {
    if (!cache->tail) {
        return;
    }
    Node *deleteNode = cache->tail;
    removeMap(cache, deleteNode->key);
    if (deleteNode->previous) {
        cache->tail = deleteNode->previous;
        cache->tail->next = NULL;
    } else {
        cache->head = NULL;
        cache->tail = NULL;
    }
    free(deleteNode);
    cache->size--;
}

char* get(LruCache *cache, int key) {
    Node *node = findNode(cache, key);
    if (!node) {
        return NULL;
    }
    moveToHead(cache, node);
    return node->string;
}

void put(LruCache *cache, int key, char *value) {
    Node *node = findNode(cache, key);
    if (node) {
        strncpy(node->string, value, Max_Size-1);
        node->string[Max_Size-1] = '\0';
        moveToHead(cache, node);
    } else {
        Node *newNode = (Node*)malloc(sizeof(Node));
        newNode->key = key;
        strncpy(newNode->string, value, Max_Size-1);
        newNode->string[Max_Size-1] = '\0';
        newNode->previous = NULL;
        newNode->next = NULL;
        newNode->next = cache->head;
        if (cache->head) {
            cache->head->previous = newNode;
        }
        cache->head = newNode;
        if (!cache->tail) {
            cache->tail = newNode;
        }
        setMap(cache, key, newNode);
        cache->size++;
        if (cache->size > cache->capacity) {
            removeTail(cache);
        }
    }
}

