#include "semaphore.h"

    void wait_sem(int sid, int sno) {
        struct sembuf s;
        s.sem_num = sno;
        s.sem_op = -1; 
        s.sem_flg = SEM_UNDO;
        if (semop(sid, &s, 1) == -1) {
            perror("Wait operation failed");
            exit(1);
        }
    }

    void signal_sem(int sid, int sno) {
        struct sembuf s;
        s.sem_num = sno;
        s.sem_op = 1;  
        s.sem_flg = SEM_UNDO;
        if (semop(sid, &s, 1) == -1) {
            perror("Signal operation failed");
            exit(1);
        }
    }
