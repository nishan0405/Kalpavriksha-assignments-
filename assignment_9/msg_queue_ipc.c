#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg {
    long type;
    int data[5];
};

int main() {
    key_t key = ftok("msg", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msg m;
    m.type = 1;
    int arr[5] = {8, 3, 5, 1, 9};

    for (int i = 0; i < 5; i++)
        m.data[i] = arr[i];

    msgsnd(msgid, &m, sizeof(m.data), 0);

    msgrcv(msgid, &m, sizeof(m.data), 1, 0);

    for (int i = 0; i < 5; i++)
        for (int j = i + 1; j < 5; j++)
            if (m.data[i] > m.data[j]) {
                int t = m.data[i];
                m.data[i] = m.data[j];
                m.data[j] = t;
            }

    printf("Sorted Array:\n");
    for (int i = 0; i < 5; i++)
        printf("%d ", m.data[i]);

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
