#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    int sid, d, in, out, num, *buf, si, i;
    int size = 10;
    in = 0;
    out = 0;
    si = shmget(8394, sizeof(int) * size, IPC_CREAT | 0666);
    if (si == -1) {
        perror("shmget");
        exit(1);
    }
    buf = (int *) shmat(si, NULL, 0);
    if (buf == (int *) -1) {
        perror("shmat");
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid > 0) {
	printf("Consumer(Parent):\n");
        printf("Enter the data:\n");
        for (i = 0; i < size; i++) {
            scanf("%d", &num);
            buf[in % size] = num;
            in++;
        }
        shmdt(buf);
        wait(NULL);                       
        shmctl(si, IPC_RMID, NULL);
    }
    else {
        sleep(10); 
        printf("\nProducer(Child):\n");
        for (i = 0; i < size; i++) {
            num = buf[out % size];
            out++;
            printf("Data[%d]: %d\n", out, num);
        }
        shmdt(buf);
    }
    return 0;
}

