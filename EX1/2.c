#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main() {
    char str[100];
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    
    pid_t pid1, pid2 = -1; 
    
    pid1 = fork();
    if (pid1 > 0) {
        pid2 = fork(); 
    }
    
    if (pid1 == 0) {
       
        int vowels = 0;
	int i;
        for (i = 0; str[i] != '\0'; i++) {
            char ch = tolower(str[i]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowels++;
            }
        }
        printf("P2: number of vowels = %d\n", vowels);
    } 
    else if (pid1 > 0 && pid2 == 0) {
        
        int special = 0;
	int i;
        for (i = 0; str[i] != '\0'; i++) {
            
            if (!isalnum((unsigned char)str[i]) && str[i] != ' ' && str[i] != '\n') {
                special++;
            }
        }
        printf("P3: Number of special characters = %d\n", special);
    } 
    else if (pid1 > 0 && pid2 > 0) {
        
        wait(NULL); 
        wait(NULL); 
        printf("P1: parent process completed:\n");
    }
    
    return 0;
}

