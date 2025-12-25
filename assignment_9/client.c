#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock, choice, amount, result;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("1.Withdraw\n2.Deposit\n3.Check Balance\n");
    scanf("%d", &choice);

    write(sock, &choice, sizeof(int));

    if (choice == 1 || choice == 2) {
        printf("Enter amount: ");
        scanf("%d", &amount);
        write(sock, &amount, sizeof(int));
    }

    read(sock, &result, sizeof(int));

    if (result == -1)
        printf("Transaction Failed\n");
    else
        printf("Current Balance: %d\n", result);

    close(sock);
    return 0;
}