#include <stdio.h>
#include <stdlib.h>

int main() {
    int arr[5] = {5, 3, 1, 4, 2};
    FILE *fp;
    fp = fopen("data.txt", "w");
    for (int i = 0; i < 5; i++)
        fprintf(fp, "%d ", arr[i]);
    fclose(fp);

    printf("Before Sorting:\n");
    for (int i = 0; i < 5; i++)
        printf("%d ", arr[i]);

    fp = fopen("data.txt", "r");
    for (int i = 0; i < 5; i++)
        fscanf(fp, "%d", &arr[i]);
    fclose(fp);

    for (int i = 0; i < 5; i++)
        for (int j = i + 1; j < 5; j++)
            if (arr[i] > arr[j]) {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }

    fp = fopen("data.txt", "w");
    for (int i = 0; i < 5; i++)
        fprintf(fp, "%d ", arr[i]);
    fclose(fp);

    printf("\nAfter Sorting:\n");
    for (int i = 0; i < 5; i++)
        printf("%d ", arr[i]);

    return 0;
}
