#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>

int main()
{
    int pipe1[2], pipe2[2];
    int arr[100], odd[100];
    int n, count = 0, i;

    pipe(pipe1);
    pipe(pipe2);

    /* ----------- P2 ----------- */
    if (fork() == 0)
    {
        close(pipe1[0]);

        printf("\nP2\n");

        printf("Enter the number of elements: ");
        scanf("%d", &n);

        printf("Enter the array elements:\n");

        for (i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        write(pipe1[1], &n, sizeof(int));
        write(pipe1[1], arr, n * sizeof(int));

        printf("P2: Array sent to P1 through Pipe1\n");

        close(pipe1[1]);
        exit(0);
    }

    wait(NULL);

    /* ----------- P1 ----------- */
    if (fork() == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], &n, sizeof(int));
        read(pipe1[0], arr, n * sizeof(int));

        printf("\nP1 \n");
        printf("P1: Received array from P2\n");

        printf("P1: Odd elements are: ");

        for (i = 0; i < n; i++)
        {
            if (arr[i] % 2 != 0)
            {
                odd[count++] = arr[i];
                printf("%d ", arr[i]);
            }
        }

        printf("\n");

        write(pipe2[1], &count, sizeof(int));
        write(pipe2[1], odd, count * sizeof(int));

        printf("P1: Sent odd elements to P3 through Pipe2\n");

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    wait(NULL);

    /* ----------- P3 ----------- */
    if (fork() == 0)
    {
        int first = INT_MIN;
        int second = INT_MIN;

        close(pipe2[1]);

        read(pipe2[0], &count, sizeof(int));
        read(pipe2[0], odd, count * sizeof(int));

        printf("\nP3 \n");
        printf("P3: Received odd elements from P1\n");

        for (i = 0; i < count; i++)
        {
            if (odd[i] > first)
            {
                second = first;
                first = odd[i];
            }
            else if (odd[i] > second && odd[i] != first)
            {
                second = odd[i];
            }
        }

        if (count < 2 || second == INT_MIN)
            printf("P3: Second Maximum element not found\n");
        else
            printf("P3: Second Maximum element in the array= %d\n", second);

        close(pipe2[0]);
        exit(0);
    }

    wait(NULL);

    return 0;
}

