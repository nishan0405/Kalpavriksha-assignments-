#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2], arr[5] = {9, 2, 7, 4, 1};

    pipe(fd);

    if (fork() == 0) {
        close(fd[0]);
        write(fd[1], arr, sizeof(arr));
        close(fd[1]);
    } else {
        int data[5];
        close(fd[1]);
        read(fd[0], data, sizeof(data));

        for (int i = 0; i < 5; i++)
            for (int j = i + 1; j < 5; j++)
                if (data[i] > data[j]) {
                    int t = data[i];
                    data[i] = data[j];
                    data[j] = t;
                }

        printf("Sorted Data:\n");
        for (int i = 0; i < 5; i++)
            printf("%d ", data[i]);
    }
    return 0;
}
