#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <termios.h>

struct SharedBuffer {
    int buffer[10];
    int in;
    int out;
    sem_t full;
    sem_t empty;
    sem_t mutex;
};

int shmid = -1;
struct SharedBuffer *shared = NULL;
struct termios old_tio;

void handle_termination(int sig) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    printf("Terminated\n");
    if (shared != NULL && shared != (void *)-1) {
        sem_destroy(&shared->mutex);
        sem_destroy(&shared->empty);
        sem_destroy(&shared->full);
        shmdt(shared);
    }
    if (shmid != -1) {
        shmctl(shmid, IPC_RMID, NULL);
    }
    exit(0);
}

int main() {
    int i, num;
    int SIZE = 100;
    struct termios new_tio;

    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    signal(SIGINT, handle_termination);

    // Matching key 24037
    shmid = shmget(24037, sizeof(struct SharedBuffer), 0666);
    if (shmid == -1) {
        perror("shmget");
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
        return 1;
    }

    shared = (struct SharedBuffer *)shmat(shmid, NULL, 0);
    if (shared == (void *)-1) {
        perror("shmat");
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
        return 1;
    }

    printf("Data Read:\n");
    fflush(stdout); 

    for (i = 0; i < SIZE; i++) {
        sem_wait(&shared->full);  
        sem_wait(&shared->mutex); 

        num = shared->buffer[shared->out];   
        shared->out = (shared->out + 1) % 10; 

        sem_post(&shared->mutex); 
        sem_post(&shared->empty); 

        printf("%d\n", num);
        fflush(stdout); 
        
        sleep(3); 
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    sem_destroy(&shared->mutex);
    sem_destroy(&shared->empty);
    sem_destroy(&shared->full);
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Terminated\n");
    return 0;
}
