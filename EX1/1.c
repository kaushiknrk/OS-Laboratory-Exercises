#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1 = -1, pid2 = -1, pid3 = -1, pid4 = -1;
    
    pid1 = fork();
    if (pid1 > 0) {
        pid2 = fork();
    }
    
    if (pid1 > 0 && pid2 == 0) {
        pid3 = fork();
        if (pid3 > 0) {
            pid4 = fork(); 
        }
    }

    
    if (pid1 > 0 && pid2 > 0) {
        
        printf("[Process P1 - Main Parent]  PID = %d, PPID = %d\n", getpid(), getppid());
    } 
    else if (pid1 == 0) {
        
        printf("[Process P2 - Child of P1]  PID = %d, PPID = %d\n", getpid(), getppid());
    } 
    else if (pid1 > 0 && pid2 == 0 && pid3 > 0 && pid4 > 0) {
      
        printf("[Process P3 - Child of P1]  PID = %d, PPID = %d\n", getpid(), getppid());
    } 
    else if (pid1 > 0 && pid2 == 0 && pid3 == 0) {
        
        printf("[Process P4 - Child of P3]  PID = %d, PPID = %d\n", getpid(), getppid());
    } 
    else if (pid1 > 0 && pid2 == 0 && pid3 > 0 && pid4 == 0) {
       
        printf("[Process P5 - Child of P3]  PID = %d, PPID = %d\n", getpid(), getppid());
   }
    if (pid1 > 0 || pid3 > 0) {
        wait(NULL);
        wait(NULL);
    }

    return 0;
}

