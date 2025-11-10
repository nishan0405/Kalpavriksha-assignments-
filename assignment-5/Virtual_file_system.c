#include "vfs.h"

unsigned char disk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeHead = NULL, *freeTail = NULL;
FileNode *root = NULL, *cwd = NULL;

int main() {
    init_disk();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char line[256];
    while (1) {
        print_prompt();
        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = 0;
        if (!strcmp(line, "exit"))
            break;

        handle_command(line);
    }

    free_memory();
    printf("Memory released. Exiting program...\n");
    return 0;
}

void print_prompt() {
    if (cwd == root) printf("/ > ");
    else printf("%s > ", cwd->name);
}

void init_disk() {
    int i;
    freeHead = freeTail = NULL;

    for (i = 0; i < NUM_BLOCKS; i++) {
        FreeBlock *block = (FreeBlock*)malloc(sizeof(FreeBlock));
        block->index = i;
        block->next = NULL;
        block->prev = freeTail;

        if (freeTail) freeTail->next = block;
        freeTail = block;
        if (!freeHead) freeHead = block;
    }

    root = (FileNode*)malloc(sizeof(FileNode));
    root->isDirectory = 1;
    strcpy(root->name, "/");
    root->next = root->prev = root;
    root->parent = NULL;
    root->children = NULL;
    root->blockCount = 0;
    root->fileSize = 0;
    cwd = root;
}

void handle_command(char *cmd) {
    if (!strncmp(cmd, "mkdir ", 6))
        mkdir_cmd(cmd+6);
    else if (!strncmp(cmd, "create ", 7))
        create_cmd(cmd+7);
    else if (!strncmp(cmd, "write ", 6)) {
        char *p = strchr(cmd+6, ' ');
        if (!p) {
            printf("Invalid write command.\n");
            return;
        }
        *p = 0;
        write_cmd(cmd+6, p+1);
    }
    else if (!strncmp(cmd, "read ", 5))
        read_cmd(cmd+5);
    else if (!strncmp(cmd, "delete ", 7))
        delete_cmd(cmd+7);
    else if (!strncmp(cmd, "rmdir ", 6))
        rmdir_cmd(cmd+6);
    else if (!strcmp(cmd, "ls"))
        ls_cmd();
    else if (!strncmp(cmd, "cd ", 3))
        cd_cmd(cmd+3);
    else if (!strcmp(cmd, "pwd"))
        pwd_cmd();
    else if (!strcmp(cmd, "df"))
        df_cmd();
    else
        printf("Unknown command.\n");
}

void mkdir_cmd(char *name) {
    if (find_in_cwd(name)) {
        printf("Name already exists in current directory.\n");
        return;
    }

    FileNode *node = (FileNode*)malloc(sizeof(FileNode));
    node->isDirectory = 1;
    strncpy(node->name, name, MAX_NAME);
    node->next = node->prev = NULL;
    node->parent = cwd;
    node->children = NULL;
    node->blockCount = 0;
    node->fileSize = 0;

    if (!cwd->children) {
        cwd->children = node;
        node->next = node->prev = node;
    } else {
        FileNode *first = cwd->children;
        FileNode *last = cwd->children->prev;
        last->next = node;
        node->prev = last;
        node->next = first;
        first->prev = node;
    }
    printf("Directory '%s' created successfully.\n", name);
}

void create_cmd(char *name) {
    if (find_in_cwd(name)) {
        printf("Name already exists in current directory.\n");
        return;
    }

    FileNode *node = (FileNode*)malloc(sizeof(FileNode));
    node->isDirectory = 0;
    strncpy(node->name, name, MAX_NAME);
    node->next = node->prev = NULL;
    node->parent = cwd;
    node->children = NULL;
    node->blockCount = 0;
    node->fileSize = 0;

    if (!cwd->children) {
        cwd->children = node;
        node->next = node->prev = node;
    } else {
        FileNode *first = cwd->children;
        FileNode *last = cwd->children->prev;
        last->next = node;
        node->prev = last;
        node->next = first;
        first->prev = node;
    }
    printf("File '%s' created successfully.\n", name);
}

void write_cmd(char *filename, char *data) {
    FileNode *node = find_in_cwd(filename);
    if (!node || node->isDirectory) {
        printf("File not found.\n");
        return;
    }

    int needed_blocks = (strlen(data) + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (needed_blocks == 0)
        needed_blocks = 1;

    int free_count = 0;
    FreeBlock *fb = freeHead;
    while (fb) {
        free_count++;
        fb = fb->next;
    }

    if (free_count < needed_blocks) {
        printf("Disk full. Cannot write data.\n");
        return;
    }

    for (int i = 0; i < node->blockCount; i++) {
        FreeBlock *fb = (FreeBlock*)malloc(sizeof(FreeBlock));
        fb->index = node->blocks[i];
        fb->next = NULL;

        if (!freeTail) {
            freeHead = freeTail = fb;
            fb->prev = NULL;
        }
        else {
            freeTail->next = fb;
            fb->prev = freeTail;
            freeTail = fb;
        }
    }

    node->blockCount = needed_blocks;
    node->fileSize = strlen(data);

    for (int i = 0; i < needed_blocks; i++) {
        FreeBlock *block = freeHead;
        node->blocks[i] = block->index;
        freeHead = block->next;

        if (freeHead)
            freeHead->prev = NULL;
        if (block == freeTail)
            freeTail = NULL;

        free(block);
        memcpy(disk[node->blocks[i]], data + i*BLOCK_SIZE, BLOCK_SIZE);
    }

    printf("Data written successfully (size=%d bytes).\n", node->fileSize);
}

void read_cmd(char *filename) {
    FileNode *node = find_in_cwd(filename);
    if (!node || node->isDirectory) {
        printf("File not found.\n");
        return;
    }

    if (node->blockCount == 0) {
        printf("(empty)\n");
        return;
    }

    int remaining = node->fileSize;
    for (int i = 0; i < node->blockCount; i++) {
        int to_read = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fwrite(disk[node->blocks[i]], 1, to_read, stdout);
        remaining -= to_read;
    }
    printf("\n");
}

void delete_cmd(char *filename) {
    FileNode *node = find_in_cwd(filename);
    if (!node || node->isDirectory) {
        printf("File not found.\n");
        return;
    }

    for (int i = 0; i < node->blockCount; i++) {
        FreeBlock *fb = (FreeBlock*)malloc(sizeof(FreeBlock));
        fb->index = node->blocks[i];
        fb->next = NULL;

        if (!freeTail) {
            freeHead = freeTail = fb;
            fb->prev = NULL;
        }
        else {
            freeTail->next = fb;
            fb->prev = freeTail;
            freeTail = fb;
        }
    }

    FileNode *prev = node->prev, *next = node->next;

    if (prev == node)
        cwd->children = NULL;
    else {
        prev->next = next;
        next->prev = prev;
        if (cwd->children == node)
            cwd->children = next;
    }

    free(node);
    printf("File deleted successfully.\n");
}

void ls_cmd() {
    if (!cwd->children) {
        printf("(empty)\n");
        return;
    }

    FileNode *node = cwd->children;
    int first = 1;

    do {
        if (!first)
            printf(" ");

        if (node->isDirectory)
            printf("%s/", node->name);
        else
            printf("%s", node->name);

        first = 0;
        node = node->next;

    } while (node != cwd->children);

    printf("\n");
}

void cd_cmd(char *name) {
    if (cwd == NULL) {
        printf("Error: current directory is not set.\n");
        return;
      }
    if (!strcmp(name, "..")) {
        if (cwd->parent) {
            cwd = cwd->parent;
            if (cwd == root)
                printf("Moved to /\n");
            else
                printf("Moved to /%s\n", cwd->name);
        }
        return;
    }

    FileNode *node = find_in_cwd(name);
    if (!node || !node->isDirectory) {
        printf("Directory not found.\n");
        return;
    }

    cwd = node;
    if (cwd == root)
        printf("Moved to /\n");
    else
        printf("Moved to /%s\n", cwd->name);
}

void pwd_cmd() {
    FileNode *node = cwd;
    char paths[256] = "";

    while (node && node != root) {
        char temp[256]; 
        sprintf(temp, "/%s%s", node->name, paths);
        strcpy(paths, temp);
        node = node->parent;
    }

    if (strlen(paths) == 0)
        strcpy(paths, "/");

    printf("%s\n", paths);
}

void rmdir_cmd(char *name) {
    FileNode *node = find_in_cwd(name);
    if (!node || !node->isDirectory) {
        printf("Directory not found.\n");
        return;
    }

    if (node->children) {
        printf("Directory not empty. Remove files first.\n");
        return;
    }

    FileNode *prev = node->prev, *next = node->next;

    if (prev == node)
        cwd->children = NULL;
    else {
        prev->next = next;
        next->prev = prev;
        if (cwd->children == node)
            cwd->children = next;
    }

    free(node);
    printf("Directory removed successfully.\n");
}

void df_cmd() {
    int used = 0, freec = 0;
    FreeBlock *fb = freeHead;

    while (fb) {
        freec++;
        fb = fb->next;
    }

    used = NUM_BLOCKS - freec;

    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", used);
    printf("Free Blocks: %d\n", freec);
    printf("Disk Usage: %.2f%%\n", used * 100.0 / NUM_BLOCKS);
}

FileNode* find_in_cwd(char *name) {
    if (!cwd || !cwd->children)
        return NULL;

    FileNode *node = cwd->children;

    do {
        if (!strcmp(node->name, name))
            return node;

        node = node->next;

    } while (node != cwd->children);

    return NULL;
}

void free_memory() {
    while (freeHead) {
        FreeBlock *tmp = freeHead;
        freeHead = freeHead->next;
        free(tmp);
    }
}
