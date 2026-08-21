#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared.h"

int main() {
    int i, shmid, num;
    struct shared *shm;

    // Create or get the shared memory segment
    shmid = shmget(SHM_KEY, sizeof(struct shared), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Producer: Shared memory creation failed");
        exit(1);
    }

    // Attach to shared memory
    shm = (struct shared *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("Producer: Attachment failed");
        exit(1);
    }

    // Initialize variables
    shm->in = 0;
    shm->out = 0;
    shm->count = 0;

    printf("Producer Started:\n");
    for (i = 0; i < SIZE; i++) {
        if (shm->count == SIZE) {
            printf("Buffer Full\n");
            break;
        }
        
        printf("Enter data [%d/%d]: ", i + 1, SIZE);
        fflush(stdout);
        if (scanf("%d", &num) != 1) {
            break; 
        }

        shm->buffer[shm->in] = num;
        printf("Produced %d at index %d\n", num, shm->in);
        
        shm->in = (shm->in + 1) % SIZE;
        shm->count++;
    }

    // Detach from shared memory
    shmdt(shm);
    printf("Producer finished entering data.\n");
    return 0;
}
