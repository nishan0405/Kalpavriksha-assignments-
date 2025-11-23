#ifndef CACHE_H
#define CACHE_H

#define Max_Size 50
#define HashMap_Size 1024   

typedef struct Node {
    char string[Max_Size];
    int key;
    struct Node *next;
    struct Node *previous;
} Node;

typedef struct Map {
    int key;
    Node *node;
    int inUse;
} Map;

typedef struct LruCache {
    int capacity;
    int size;
    Node *head;
    Node *tail;
    Map *hashMap;
} LruCache;


int hash(int key);
LruCache *createCache(int capacity);
Node *findNode(LruCache *cache, int key);
void setMap(LruCache *cache, int key, Node *node);
void removeMap(LruCache *cache, int key);
void moveToHead(LruCache *cache, Node *node);
void removeTail(LruCache *cache);
char* get(LruCache *cache, int key);
void put(LruCache *cache, int key, char *value);

#endif