#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_SIZE 2
#define MAX_SIZE 10
#define MAX_INTENSITY 256

void random_matrix(int *matrix, int n, int max_intensity);
void print_matrix(int *matrix, int n);
void rotate(int *matrix, int n);
void smoothing(int *matrix, int n);
int calculate_average(int *matrix, int n, int row, int col, int *original);

int main()
{
    int n;
    printf("Enter matrix size (%d–%d): ", MIN_SIZE, MAX_SIZE);
    scanf("%d", &n);
    if (n < MIN_SIZE || n > MAX_SIZE)
    {
        printf("The entered value %d is invalid\n", n);
        return 0;
    }

    int *matrix = (int *)malloc(n * n * sizeof(int));
    if (matrix == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    random_matrix(matrix, n, MAX_INTENSITY);

    printf("Original Randomly Generated Matrix:\n");
    print_matrix(matrix, n);

    rotate(matrix, n);
    printf("90 Degree Rotated Matrix:\n");
    print_matrix(matrix, n);

    smoothing(matrix, n);
    printf("Matrix after Applying 3x3 Smoothing Filter:\n");
    print_matrix(matrix, n);

    free(matrix);
    return 0;
}

void random_matrix(int *matrix, int n, int max_intensity)
{
    srand(time(0));
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            *(matrix + row * n + col) = rand() % max_intensity;
        }
    }
}

void print_matrix(int *matrix, int n)
{
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            printf("%3d ", *(matrix + row * n + col));
        }
        printf("\n");
    }
}

void rotate(int *matrix, int n)
{
    // Transpose
    for (int row = 0; row < n; row++)
    {
        for (int col = row + 1; col < n; col++)
        {
            int temp = *(matrix + row * n + col);
            *(matrix + row * n + col) = *(matrix + col * n + row);
            *(matrix + col * n + row) = temp;
        }
    }

    // Reverse each row
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n / 2; col++)
        {
            int temp = *(matrix + row * n + col);
            *(matrix + row * n + col) = *(matrix + row * n + (n - 1 - col));
            *(matrix + row * n + (n - 1 - col)) = temp;
        }
    }
}

int calculate_average(int *matrix, int n, int row, int col, int *original)
{
    int sum = 0, count = 0;

    for (int x = row - 1; x <= row + 1; x++)
    {
        for (int y = col - 1; y <= col + 1; y++)
        {
            if (x >= 0 && x < n && y >= 0 && y < n)
            {
                sum += *(original + x * n + y);
                count++;
            }
        }
    }

    return sum / count;
}

void smoothing(int *matrix, int n)
{
    int original[n * n];

    for (int i = 0; i < n * n; i++)
    {
        original[i] = *(matrix + i);
    }

    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            *(matrix + row * n + col) = calculate_average(matrix, n, row, col, original);
        }
    }
}
