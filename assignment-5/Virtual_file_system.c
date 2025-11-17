#include "vfs.h"

unsigned char disk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeHead = NULL;
FreeBlock *freeTail = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;

int main(void)
{
    init_disk();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char line[256];
    while (true)
    {
        print_prompt();

        if (!fgets(line, sizeof(line), stdin))
        {
            break;
        }

        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "exit") == 0)
        {
            break;
        }

        handle_command(line);
    }

    free_memory();
    printf("Memory released. Exiting program...\n");
    return 0;
}

void print_prompt(void)
{
    if (cwd == root)
    {
        printf("/ > ");
    }
    else
    {
        printf("%s > ", cwd->name);
    }
}

void init_disk(void)
{
    int i;
    freeHead = NULL;
    freeTail = NULL;
    bool isFreeHeadSet = false;

    for (i = 0; i < NUM_BLOCKS; i++)
    {
        FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
        block->index = i;
        block->next = NULL;
        block->prev = freeTail;

        if (freeTail != NULL)
        {
            freeTail->next = block;
        }
        freeTail = block;

        if (!isFreeHeadSet)
        {
            freeHead = block;
            isFreeHeadSet = true;
        }
    }

    root = (FileNode *)malloc(sizeof(FileNode));
    root->isDirectory = 1;
    strcpy(root->name, "/");
    root->next = root;
    root->prev = root;
    root->parent = NULL;
    root->children = NULL;
    root->blockCount = 0;
    root->fileSize = 0;
    cwd = root;
}

void handle_command(char *cmd)
{
    if (strncmp(cmd, "mkdir ", 6) == 0)
    {
        mkdir_cmd(cmd + 6);
    }
    else if (strncmp(cmd, "create ", 7) == 0)
    {
        create_cmd(cmd + 7);
    }
    else if (strncmp(cmd, "write ", 6) == 0)
    {
        char *p = strchr(cmd + 6, ' ');
        if (p == NULL)
        {
            printf("Invalid write command.\n");
            return;
        }
        *p = 0;
        write_cmd(cmd + 6, p + 1);
    }
    else if (strncmp(cmd, "read ", 5) == 0)
    {
        read_cmd(cmd + 5);
    }
    else if (strncmp(cmd, "delete ", 7) == 0)
    {
        delete_cmd(cmd + 7);
    }
    else if (strncmp(cmd, "rmdir ", 6) == 0)
    {
        rmdir_cmd(cmd + 6);
    }
    else if (strcmp(cmd, "ls") == 0)
    {
        ls_cmd();
    }
    else if (strncmp(cmd, "cd ", 3) == 0)
    {
        cd_cmd(cmd + 3);
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        pwd_cmd();
    }
    else if (strcmp(cmd, "df") == 0)
    {
        df_cmd();
    }
    else
    {
        printf("Unknown command.\n");
    }
}

void mkdir_cmd(char *name)
{
    if (name == NULL || strlen(name) == 0)
    {
        printf("Error: directory name cannot be empty.\n");
        return;
    }

    while (*name == ' ')
    {
        name++;
    }

    if (*name == '\0')
    {
        printf("Error: directory name cannot be blank.\n");
        return;
    }

    int i;
    for (i = 0; name[i] != '\0'; i++)
    {
        if (strchr("/\\:*?\"<>|", name[i]) != NULL)
        {
            printf("Error: directory name contains invalid characters.\n");
            return;
        }
    }

    if (find_in_cwd(name) != NULL)
    {
        printf("Name already exists in current directory.\n");
        return;
    }

    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    if (node == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    node->isDirectory = 1;
    strncpy(node->name, name, MAX_NAME);
    node->name[MAX_NAME - 1] = '\0';
    node->next = NULL;
    node->prev = NULL;
    node->parent = cwd;
    node->children = NULL;
    node->blockCount = 0;
    node->fileSize = 0;

    if (cwd->children == NULL)
    {
        cwd->children = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        FileNode *first = cwd->children;
        FileNode *last = cwd->children->prev;
        last->next = node;
        node->prev = last;
        node->next = first;
        first->prev = node;
    }

    printf("Directory '%s' created successfully.\n", name);
}

void create_cmd(char *name)
{
    if (find_in_cwd(name) != NULL)
    {
        printf("Name already exists in current directory.\n");
        return;
    }

    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    if (node == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    node->isDirectory = 0;
    strncpy(node->name, name, MAX_NAME);
    node->name[MAX_NAME - 1] = '\0';
    node->next = NULL;
    node->prev = NULL;
    node->parent = cwd;
    node->children = NULL;
    node->blockCount = 0;
    node->fileSize = 0;

    if (cwd->children == NULL)
    {
        cwd->children = node;
        node->next = node;
        node->prev = node;
    }
    else
    {
        FileNode *first = cwd->children;
        FileNode *last = cwd->children->prev;
        last->next = node;
        node->prev = last;
        node->next = first;
        first->prev = node;
    }

    printf("File '%s' created successfully.\n", name);
}

int countFreeBlocks(void)
{
    int count = 0;
    FreeBlock *currentBlock = freeHead;
    while (currentBlock != NULL)
    {
        count++;
        currentBlock = currentBlock->next;
    }
    return count;
}

void returnBlocksToFreeList(FileNode *file)
{
    int i;
    for (i = 0; i < file->blockCount; i++)
    {
        int blockIndex = file->blocks[i];

        if (blockIndex < 0 || blockIndex >= NUM_BLOCKS)
        {
            continue;
        }

        FreeBlock *newFreeBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        if (newFreeBlock == NULL)
        {
            printf("Memory allocation failed while freeing blocks.\n");
            return;
        }
        newFreeBlock->index = blockIndex;
        newFreeBlock->next = NULL;

        if (freeTail == NULL)
        {
            freeHead = newFreeBlock;
            freeTail = newFreeBlock;
            newFreeBlock->prev = NULL;
        }
        else
        {
            freeTail->next = newFreeBlock;
            newFreeBlock->prev = freeTail;
            freeTail = newFreeBlock;
        }
        file->blocks[i] = -1;
    }
    file->blockCount = 0;
    file->fileSize = 0;
}

void write_cmd(char *filename, char *data)
{
    FileNode *file = find_in_cwd(filename);
    if (file == NULL || file->isDirectory != 0)
    {
        printf("File not found.\n");
        return;
    }

    int dataLength = (int)strlen(data);
    int requiredBlocks = (dataLength + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (requiredBlocks == 0)
    {
        requiredBlocks = 1;
    }

    int availableFreeBlocks = countFreeBlocks();
    if (availableFreeBlocks < requiredBlocks)
    {
        printf("Disk full. Cannot write data.\n");
        return;
    }
    returnBlocksToFreeList(file);

    int i;
    for (i = 0; i < requiredBlocks; i++)
    {
        if (freeHead == NULL)
        {
            printf("Error: free list corrupted.\n");
            return;
        }

        FreeBlock *block = freeHead;
        int blockIndex = block->index;

        file->blocks[i] = blockIndex;
        file->blockCount++;

        freeHead = block->next;
        if (freeHead != NULL)
        {
            freeHead->prev = NULL;
        }
        else
        {
            freeTail = NULL;
        }

        free(block);

        int offset = i * BLOCK_SIZE;
        int bytesToCopy = dataLength - offset;
        if (bytesToCopy > BLOCK_SIZE)
        {
            bytesToCopy = BLOCK_SIZE;
        }

        memcpy(disk[blockIndex], data + offset, bytesToCopy);

        if (bytesToCopy < BLOCK_SIZE)
        {
            memset(disk[blockIndex] + bytesToCopy, 0, BLOCK_SIZE - bytesToCopy);
        }
    }

    file->fileSize = dataLength;

    printf("Data written successfully (size=%d bytes).\n", file->fileSize);
}

void read_cmd(char *filename)
{
    FileNode *node = find_in_cwd(filename);
    if (node == NULL || node->isDirectory != 0)
    {
        printf("File not found.\n");
        return;
    }

    if (node->blockCount == 0)
    {
        printf("(empty)\n");
        return;
    }

    int remaining = node->fileSize;
    int i;
    for (i = 0; i < node->blockCount; i++)
    {
        int toRead = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fwrite(disk[node->blocks[i]], 1, toRead, stdout);
        remaining -= toRead;
    }
    printf("\n");
}

void delete_cmd(char *filename)
{
    FileNode *node = find_in_cwd(filename);
    if (node == NULL || node->isDirectory != 0)
    {
        printf("File not found.\n");
        return;
    }

    int i;
    for (i = 0; i < node->blockCount; i++)
    {
        FreeBlock *newFreeBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        if (newFreeBlock == NULL)
        {
            printf("Memory allocation failed while deleting file.\n");
            return;
        }
        newFreeBlock->index = node->blocks[i];
        newFreeBlock->next = NULL;

        if (freeTail == NULL)
        {
            freeHead = newFreeBlock;
            freeTail = newFreeBlock;
            newFreeBlock->prev = NULL;
        }
        else
        {
            freeTail->next = newFreeBlock;
            newFreeBlock->prev = freeTail;
            freeTail = newFreeBlock;
        }
    }

    FileNode *prevNode = node->prev;
    FileNode *nextNode = node->next;

    if (prevNode == node)
    {
        cwd->children = NULL;
    }
    else
    {
        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        if (cwd->children == node)
        {
            cwd->children = nextNode;
        }
    }

    free(node);
    printf("File deleted successfully.\n");
}

void ls_cmd(void)
{
    if (cwd->children == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode *node = cwd->children;

    do
    {
        if (node->isDirectory != 0)
        {
            printf("%s/\n", node->name);
        }
        else
        {
            printf("%s\n", node->name);
        }
        node = node->next;
    } while (node != cwd->children);
}

void cd_cmd(char *name)
{
    if (cwd == NULL)
    {
        printf("Error: current directory is not set.\n");
        return;
    }

    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent != NULL)
        {
            cwd = cwd->parent;
        }
    }
    else
    {
        FileNode *node = find_in_cwd(name);
        if (node == NULL || node->isDirectory == 0)
        {
            printf("Directory not found.\n");
            return;
        }
        cwd = node;
    }

    FileNode *node = cwd;
    char path[PATH_SIZE];
    path[0] = '\0';

    while (node != NULL && node != root)
    {
        char temp[512];
        int written = snprintf(temp, sizeof(temp), "/%s%s", node->name, path);
        if (written <= 0 || written >= (int)sizeof(temp))
        {
            printf("Path too long.\n");
            return;
        }
        strcpy(path, temp);
        node = node->parent;
    }

    if (strlen(path) == 0)
    {
        strcpy(path, "/");
    }

    printf("Moved to %s\n", path);
}

void pwd_cmd(void)
{
    FileNode *node = cwd;
    char path[PATH_SIZE];
    path[0] = '\0';

    while (node != NULL && node != root)
    {
        char temp[PATH_SIZE];
        int written = snprintf(temp, sizeof(temp), "/%s%s", node->name, path);
        if (written <= 0 || written >= (int)sizeof(temp))
        {
            printf("Path too long to display safely.\n");
            return;
        }

        strcpy(path, temp);
        node = node->parent;
    }

    if (strlen(path) == 0)
    {
        strcpy(path, "/");
    }

    printf("%s\n", path);
}

void rmdir_cmd(char *name)
{
    FileNode *node = find_in_cwd(name);
    if (node == NULL || node->isDirectory == 0)
    {
        printf("Directory not found.\n");
        return;
    }

    if (node->children != NULL)
    {
        printf("Directory not empty. Remove files first.\n");
        return;
    }

    FileNode *prevNode = node->prev;
    FileNode *nextNode = node->next;

    if (prevNode == node)
    {
        cwd->children = NULL;
    }
    else
    {
        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        if (cwd->children == node)
        {
            cwd->children = nextNode;
        }
    }

    free(node);
    printf("Directory removed successfully.\n");
}

void df_cmd(void)
{
    int usedBlocks;
    int freeBlocks = 0;
    FreeBlock *currentFreeBlock = freeHead;

    while (currentFreeBlock != NULL)
    {
        freeBlocks++;
        currentFreeBlock = currentFreeBlock->next;
    }

    usedBlocks = NUM_BLOCKS - freeBlocks;

    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usedBlocks * 100.0 / NUM_BLOCKS);
}

FileNode* find_in_cwd(char *name)
{
    if (cwd == NULL || cwd->children == NULL)
    {
        return NULL;
    }

    FileNode *node = cwd->children;

    do
    {
        if (strcmp(node->name, name) == 0)
        {
            return node;
        }
        node = node->next;
    } while (node != cwd->children);

    return NULL;
}

void free_file_tree(FileNode *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->isDirectory != 0 && node->children != NULL)
    {
        FileNode *start = node->children;
        FileNode *curr = start;

        do
        {
            FileNode *next = curr->next;
            curr->next = NULL;
            curr->prev = NULL;
            free_file_tree(curr);
            curr = next;
        } while (curr != start);
    }

    free(node);
}

void free_memory(void)
{
    while (freeHead != NULL)
    {
        FreeBlock *tmp = freeHead;
        freeHead = freeHead->next;
        free(tmp);
    }
    freeTail = NULL;

    free_file_tree(root);
    root = NULL;
}


