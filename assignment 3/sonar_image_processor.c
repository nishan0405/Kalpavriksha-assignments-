#include <stdio.h>
#include <string.h>
#include <time.h>

void random_matrix(int *matrix, int n);
void print_matrix(int *matrix, int n);
void rotate(int *matrix, int n);
void smoothing(int *matrix, int n);

int main()
{
    int n;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &n);
    if (n < 2 || n > 10)
    {
        printf("The entered value %d is invalid", n);
        return 0;
    }

    int matrix[n * n];
    random_matrix(matrix, n);

    printf("Original Randomly Generated Matrix:\n");
    print_matrix(matrix, n);

    rotate(matrix, n);
    printf("90 Degree Rotated Matrix:\n");
    print_matrix(matrix, n);

    smoothing(matrix, n);
    printf("Matrix after Applying 3×3 Smoothing Filter:\n");
    print_matrix(matrix, n);

    return 0;
}

void random_matrix(int *matrix, int n)
{
    srand(time(0));
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            *(matrix + row * n + col) = rand() % 256;
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

void smoothing(int *matrix, int n)
{
    int temp[n * n];

    // Copy matrix into temp
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            *(temp + row * n + col) = *(matrix + row * n + col);
        }
    }

    // Apply smoothing filter
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            int sum = 0, count = 0;
            for (int x = row - 1; x <= row + 1; x++)
            {
                for (int y = col - 1; y <= col + 1; y++)
                {
                    if (x >= 0 && x < n && y >= 0 && y < n)
                    {
                        sum += *(temp + x * n + y);
                        count++;
                    }
                }
            }
            *(matrix + row * n + col) = sum / count;
        }
    }
}
