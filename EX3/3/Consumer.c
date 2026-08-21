#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "shared.h"

int main() {
    int shmid;
    struct shared *shm;

    
    shmid = shmget(SHM_KEY, sizeof(struct shared), 0666);
    if (shmid < 0) {
        perror("Consumer: Shared memory segment not found. Run producer first");
        exit(1);
    }

    
    shm = (struct shared *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("Consumer: Attachment failed");
        exit(1);
    }

    printf("\nConsumer Reading Data:\n");
    
    
    if (shm->count == 0) {
        printf("Buffer is empty. Waiting for data...\n");
        sleep(2); 
    }

    while (shm->count > 0) {
        printf("Consumed %d from index %d\n", shm->buffer[shm->out], shm->out);
        shm->out = (shm->out + 1) % SIZE;
        shm->count--;
    }
    
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Consumer finished and shared memory cleaned up.\n");

    return 0;
}
