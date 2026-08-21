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
        shmdt(shared);
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
    shmid = shmget(24037, sizeof(struct SharedBuffer), IPC_CREAT | 0666);
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

    shared->in = 0;
    shared->out = 0;
    sem_init(&shared->mutex, 1, 1);
    sem_init(&shared->empty, 1, 10);
    sem_init(&shared->full, 1, 0);

    for (i = 0; i < SIZE; i++) {
        printf("Enter data: ");
        fflush(stdout);
        if (scanf("%d", &num) != 1) {
            break; 
        }

        sem_wait(&shared->empty); 
        sem_wait(&shared->mutex); 

        shared->buffer[shared->in] = num;          
        shared->in = (shared->in + 1) % 10; 

        sem_post(&shared->mutex); 
        sem_post(&shared->full);  
        
        sleep(1);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    shmdt(shared);
    printf("Terminated\n");
    return 0;
}
