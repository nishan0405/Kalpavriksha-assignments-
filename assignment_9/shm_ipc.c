#include <stdio.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shm", 65);
    int shmid = shmget(key, sizeof(int) * 5, 0666 | IPC_CREAT);
    int *arr = (int *)shmat(shmid, NULL, 0);

    int temp[5] = {7, 2, 6, 4, 1};
    for (int i = 0; i < 5; i++) arr[i] = temp[i];

    for (int i = 0; i < 5; i++)
        for (int j = i + 1; j < 5; j++)
            if (arr[i] > arr[j]) {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }

    printf("Sorted Data:\n");
    for (int i = 0; i < 5; i++)
        printf("%d ", arr[i]);

    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
