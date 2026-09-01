#define _DEFAULT_SOURCE
#include "semaphore.h"

int main() {
    union semaphore snum;
    int sid, shmid, num, in = 0;
    int *buf;
    unsigned short a[3]; // Fixed initialization array syntax 

    key_t shm_key = ftok(".", 'A');
    key_t sem_key = ftok(".", 'B');

    // 1. Create Shared Memory Segment
    shmid = shmget(shm_key, sizeof(int) * SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Producer: shmget failed");
        exit(1);
    }

    // Attach to Shared Memory block
    buf = (int *)shmat(shmid, NULL, 0);
    if (buf == (void *)-1) {
        perror("Producer: shmat failed");
        exit(1);
    }

    // 2. Create Semaphore Array
    sid = semget(sem_key, 3, IPC_CREAT | 0666);
    if (sid == -1) {
        perror("Producer: semget failed");
        exit(1);
    }

    // Setup initial semaphore values
    a[MUTEX] = 1;
    a[FULL] = 0;
    a[EMPTY] = SIZE;
    snum.array = a;
    semctl(sid, 0, SETALL, snum);

    printf("--- Producer Started (Buffer Size: %d) ---\n", SIZE);
    printf("Enter values to produce. Enter -1 to terminate.\n");

    while (1) {
        printf("\nEnter data item: ");
        if (scanf("%d", &num) != 1) break;

        // Synchronize before writing
        wait_sem(sid, EMPTY); 
        wait_sem(sid, MUTEX); 

        // Write to circular buffer using modulo wrapping
        buf[in % SIZE] = num;
        printf("[Producer] Inserted %d at circular index %d\n", num, in % SIZE);
        in++;

        // Signal completion
        signal_sem(sid, MUTEX); 
        signal_sem(sid, FULL);  

        // If -1 is submitted, break out of loop to exit cleanly
        if (num == -1) {
            printf("[Producer] Termination value (-1) entered. Exiting.\n");
            break;
        }
    }

    shmdt(buf);
    return 0;
}
