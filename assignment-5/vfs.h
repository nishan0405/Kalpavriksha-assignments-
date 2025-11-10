#ifndef VFS_H
#define VFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_BLOCKS_PER_FILE 50

typedef struct FreeBlock {
    int index;
    struct FreeBlock *prev, *next;
} FreeBlock;

typedef struct FileNode {
    int isDirectory;
    char name[MAX_NAME];
    struct FileNode *next, *prev;
    struct FileNode *parent;
    struct FileNode *children;
    int blockCount;
    int blocks[MAX_BLOCKS_PER_FILE];
    int fileSize;
} FileNode;

extern unsigned char disk[NUM_BLOCKS][BLOCK_SIZE];
extern FreeBlock *freeHead, *freeTail;
extern FileNode *root, *cwd;

void init_disk();
void handle_command(char *cmd);
void mkdir_cmd(char *name);
void create_cmd(char *name);
void write_cmd(char *filename, char *data);
void read_cmd(char *filename);
void delete_cmd(char *filename);
void rmdir_cmd(char *name);
void ls_cmd();
void cd_cmd(char *name);
void pwd_cmd();
void df_cmd();
void free_memory();
FileNode* find_in_cwd(char *name);
void print_prompt();

#endif
