#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

pthread_mutex_t lock;

void *handle_client(void *arg)
{
    int client = *(int *)arg;
    free(arg);

    int choice, amount, balance = 0;
    FILE *fp;

    read(client, &choice, sizeof(int));

    pthread_mutex_lock(&lock);

    fp = fopen("accountDB.txt", "r+");
    if (fp == NULL)
    {
        fp = fopen("accountDB.txt", "w+");
        fprintf(fp, "0");
        fflush(fp);
        rewind(fp);
    }

    fscanf(fp, "%d", &balance);

    if (choice == 1)
    {
        read(client, &amount, sizeof(int));
        if (amount <= balance)
        {
            balance -= amount;
        }
        else
        {
            int fail = -1;
            write(client, &fail, sizeof(int));
            fclose(fp);
            pthread_mutex_unlock(&lock);
            close(client);
            return NULL;
        }
    }
    else if (choice == 2)
    {
        read(client, &amount, sizeof(int));
        balance += amount;
    }

    rewind(fp);
    fprintf(fp, "%d", balance);
    fflush(fp);

    write(client, &balance, sizeof(int));

    fclose(fp);
    pthread_mutex_unlock(&lock);
    close(client);

    return NULL;
}

int main()
{
    int server_fd, *client_fd;
    struct sockaddr_in addr;
    pthread_t tid;

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Server Running...\n");

    while (1)
    {
        client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);

        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}