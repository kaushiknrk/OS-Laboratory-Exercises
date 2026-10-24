#include<stdio.h>
#include<stdlib.h>
int main() {
    int a[50], b[50], n, head, prev_head, size, ch;
    int i, j, cur, total, pos, temp, dir;
    int done[50];

    printf("=========================================\n");
    printf("         DISK SCHEDULING INPUTS          \n");
    printf("=========================================\n");
    printf("Enter number of requests : ");
    scanf("%d", &n);
    printf("Enter disk requests     : ");
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);
    printf("Enter initial head pos   : ");
    scanf("%d", &head);
    printf("Enter previous head pos  : "); // Added this input
    scanf("%d", &prev_head);
    printf("Enter disk size          : ");
    scanf("%d", &size);

    // Automatically calculate direction based on head movement
    if (head >= prev_head) {
        dir = 1; // High/Right
    } else {
        dir = 2; // Low/Left
    }

    do {
        printf("\n=========================================\n");
        printf("       DISK SCHEDULING ALGORITHMS        \n");
        printf("=========================================\n");
        printf(" [1] FCFS    [2] SSTF    [3] SCAN\n");
        printf(" [4] C-SCAN  [5] LOOK    [6] C-LOOK\n");
        printf(" [7] Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        if (ch == 7) {
            printf("\n>> Program terminated. Goodbye!\n");
            break;
        }

        if (ch < 1 || ch > 6) {
            printf("\n>> Invalid choice! Please select 1-7.\n");
            continue;
        }

        // Print the automatically determined direction for the user
        if (ch >= 3 && ch <= 6) {
            if (dir == 1)
                printf(">> Calculated Direction: High/Right (Since Head >= Prev Head)\n");
            else
                printf(">> Calculated Direction: Low/Left (Since Head < Prev Head)\n");
        }

        cur = head;
        total = 0;

        printf("\n-----------------------------------------\n");
        printf("                 RESULTS                 \n");
        printf("-----------------------------------------\n");
        printf("Seek Sequence:\n [%d]", cur);

        if (ch == 1) {
            for (i = 0; i < n; i++) {
                total += abs(cur - a[i]);
                cur = a[i];
                printf(" ➔ [%d]", cur);
            }
        }
        else if (ch == 2) {
            for (i = 0; i < n; i++) done[i] = 0;
            for (i = 0; i < n; i++) {
                int min = 999999, p = -1;
                for (j = 0; j < n; j++) {
                    if (!done[j] && abs(cur - a[j]) < min) {
                        min = abs(cur - a[j]);
                        p = j;
                    }
                }
                done[p] = 1;
                total += abs(cur - a[p]);
                cur = a[p];
                printf(" ➔ [%d]", cur);
            }
        }
        else {
            for (i = 0; i < n; i++) b[i] = a[i];

            // Sorting array
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    if (b[i] > b[j]) {
                        temp = b[i];
                        b[i] = b[j];
                        b[j] = temp;
                    }
                }
            }

            pos = 0;
            while (pos < n && b[pos] < head) pos++;

            if (dir == 1) {
                for (i = pos; i < n; i++) {
                    total += abs(cur - b[i]);
                    cur = b[i];
                    printf(" ➔ [%d]", cur);
                }
                if (ch == 3) {
                    if (cur != size - 1) {
                        total += abs(cur - (size - 1));
                        cur = size - 1;
                        printf(" ➔ [%d]", cur);
                    }
                    for (i = pos - 1; i >= 0; i--) {
                        total += abs(cur - b[i]);
                        cur = b[i];
                        printf(" ➔ [%d]", cur);
                    }
                }
                else if (ch == 4) {
                    if (cur != size - 1) {
                        total += abs(cur - (size - 1));
                        cur = size - 1;
                        printf(" ➔ [%d]", cur);
                    }
                    if (pos > 0) {
                        total += (size - 1);
                        cur = 0;
                        printf(" ➔ [%d]", cur);
                        for (i = 0; i < pos; i++) {
                            total += abs(cur - b[i]);
                            cur = b[i];
                            printf(" ➔ [%d]", cur);
                        }
                    }
                }
                else if (ch == 5) {
                    for (i = pos - 1; i >= 0; i--) {
                        total += abs(cur - b[i]);
                        cur = b[i];
                        printf(" ➔ [%d]", cur);
                    }
                }
                else if (ch == 6) {
                    if (pos > 0) {
                        total += abs(cur - b[0]);
                        cur = b[0];
                        printf(" ➔ [%d]", cur);
                        for (i = 1; i < pos; i++) {
                            total += abs(cur - b[i]);
                            cur = b[i];
                            printf(" ➔ [%d]", cur);
                        }
                    }
                }
            }
            else {
                for (i = pos - 1; i >= 0; i--) {
                    total += abs(cur - b[i]);
                    cur = b[i];
                    printf(" ➔ [%d]", cur);
                }
                if (ch == 3) {
                    if (cur != 0) {
                        total += abs(cur - 0);
                        cur = 0;
                        printf(" ➔ [%d]", cur);
                    }
                    for (i = pos; i < n; i++) {
                        total += abs(cur - b[i]);
                        cur = b[i];
                        printf(" ➔ [%d]", cur);
                    }
                }
                else if (ch == 4) {
                    if (cur != 0) {
                        total += abs(cur - 0);
                        cur = 0;
                        printf(" ➔ [%d]", cur);
                    }
                    if (pos < n) {
                        total += (size - 1);
                        cur = size - 1;
                        printf(" ➔ [%d]", cur);
                        for (i = n - 1; i >= pos; i--) {
                            total += abs(cur - b[i]);
                            cur = b[i];
                            printf(" ➔ [%d]", cur);
                        }
                    }
                }
                else if (ch == 5) {
                    for (i = pos; i < n; i++) {
                        total += abs(cur - b[i]);
                        cur = b[i];
                        printf(" ➔ [%d]", cur);
                    }
                }
                else if (ch == 6) {
                    if (pos < n) {
                        total += abs(cur - b[n - 1]);
                        cur = b[n - 1];
                        printf(" ➔ [%d]", cur);
                        for (i = n - 2; i >= pos; i--) {
                            total += abs(cur - b[i]);
                            cur = b[i];
                            printf(" ➔ [%d]", cur);
                        }
                    }
                }
            }
        }
        printf("\n\nTotal Head Movement : %d cylinders\n", total);
        printf("=========================================\n");
    } while (ch != 7);

    return 0;
}
