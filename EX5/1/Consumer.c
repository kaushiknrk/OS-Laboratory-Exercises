#include "semaphore.h"

int main() {
    int sid, shmid, num, out = 0;
    int *buf;

    key_t shm_key = ftok(".", 'A');
    key_t sem_key = ftok(".", 'B');

    // 1. Locate Shared Memory Block
    shmid = shmget(shm_key, sizeof(int) * SIZE, 0666);
    if (shmid == -1) {
        perror("Consumer: shmget failed. Run producer first.");
        exit(1);
    }

    buf = (int *)shmat(shmid, NULL, 0);
    if (buf == (void *)-1) {
        perror("Consumer: shmat failed");
        exit(1);
    }

    // 2. Access the Semaphore Array
    sid = semget(sem_key, 3, 0666);
    if (sid == -1) {
        perror("Consumer: semget failed");
        exit(1);
    }

    printf("--- Consumer Started ---\n\n");

    while (1) {
        // Synchronize before reading
        wait_sem(sid, FULL);  
        wait_sem(sid, MUTEX); 

        // Read from circular buffer using modulo wrapping
        num = buf[out % SIZE];
        printf("[Consumer] Read %d from circular index %d\n", num, out % SIZE);
        out++;

        // Signal that the slot has been consumed and is available for reuse
        signal_sem(sid, MUTEX); 
        signal_sem(sid, EMPTY); 

        // If data is -1, break loop and perform IPC cleanup
        if (num == -1) {
            printf("[Consumer] Termination signal received. Clearing IPC.\n");
            break;
        }
        
         // Simulate consumption processing time delay
    }

    // 3. Remove Shared Memory and Semaphores completely from system tables
    shmdt(buf);
    semctl(sid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);

    printf("\nIPC Shared Memory and Semaphore structures cleared successfully.\n");
    return 0;
}
[2
