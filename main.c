#include "commands.h"

#include <stdio.h>

int main() {
    char command;
    printf("Welcome to the OS sim!\n");
    while(1){
        printf("Enter a command: ");
        scanf("%c", &command);
        switch(command){
            case 'C':
                int priority;
                printf("Enter the priority of the new process: ");
                scanf("%d", &priority);
                Create(priority);
                break;
            
            case 'F':
                Fork();
                break;

            case 'K':
                int pid;
                printf("Enter the pid of the process you are trying to kill: ");
                scanf("%d", &pid);
                Kill(pid);
                break;

            case 'E':
                Exit();
                break;
            
            case 'Q':
                Quantum();
                break;
            
            case 'S':
                int pid;
                char str[100];
                printf("Enter the pid of the receiving process: ");
                scanf("%d", &pid);
                printf("Enter the message you are trying to send: ");
                scanf("%s", str);
                Send(pid, str);#include "commands.h"

#include <stdio.h>

int main() {
    char command;
    printf("Welcome to the OS sim!\n");
    while(1){
        printf("Enter a command: ");
        scanf("%c", &command);
        switch(command){
            case 'C':
                int priority;
                printf("Enter the priority of the new process: ");
                scanf("%d", &priority);
                Create(priority);
                break;
            
            case 'F':
                Fork();
                break;

            case 'K':
                int pid;
                printf("Enter the pid of the process you are trying to kill: ");
                scanf("%d", &pid);
                Kill(pid);
                break;

            case 'E':
                Exit();
                break;
            
            case 'Q':
                Quantum();
                break;
            
            case 'S':
                int pid;
                char str[100];
                printf("Enter the pid of the receiving process: ");
                scanf("%d", &pid);
                printf("Enter the message you are trying to send: ");
                scanf("%s", str);
                Send(pid, str);
                break;
            
            case 'R':
                Receive();
                break;
            
            case 'Y':
                int pid;
                char str[100];
                printf("Enter the pid of the receiving process: ");
                scanf("%d", &pid);
                printf("Enter the message you are trying to send: ");
                scanf("%s", str)
                Reply(pid, str);
                break;
            
            case 'N':
                int semaphore;
                int initVal;
                printf("Enter the semaphore id: ");
                scanf("%d", &semaphore);
                printf("Enter the initial value of the semaphore: ");
                scanf("%d", &initVal);
                NewSemaphore(semaphore, initVal);
                break;
            
            case 'P':
                int semaphore;
                printf("Enter the semaphore id: ");
                scanf("%d", &semaphore);
                SemaphoreP(semaphore);
                break;
            
            case 'V':
                int semaphore;
                printf("Enter the semaphore id: ");
                scanf("%d", &semaphore);
                SemaphoreV(semaphore);
                break;

            case 'I':
                int pid;
                printf("Enter the pid of the process you are trying to know: ");
                scanf("%d", &pid);
                Procinfo(pid);
                break;

            case 'T':
                Totalinfo();
                break;

            default:
                printf("Invalid grade entered.\n");
        }
    }
    
    return 0;
}
                printf("Enter the pid of the receiving process: ");
                scanf("%d", &pid);
                printf("Enter the message you are trying to send: ");
                scanf("%s", str)
                Reply(pid, str);
                break;
            
            case 'N':
                int semaphore;
                int initVal;
                printf("Enter the semaphore id: ");
                scanf("%d", &semaphore);
                printf("Enter the initial value of the semaphore: ");
                scanf("%d", &initVal);
                NewSemaphore(semaphore, initVal);
                break;
            
            case 'P':
                int semaphore;
                printf("Enter the semaphore id: ");
                scanf("%d", &semaphore);
                SemaphoreP(semaphore);
                break;
            
            case 'V':
                int semaphore;
                printf("Enter the semaphore id: ");
                scanf("%d", &semaphore);
                SemaphoreV(semaphore);
                break;

            case 'I':
                int pid;
                printf("Enter the pid of the process you are trying to know: ");
                scanf("%d", &pid);
                Procinfo(pid);
                break;

            case 'T':
                Totalinfo();
                break;

            default:
                printf("Invalid grade entered.\n");
        }
    }
    
    return 0;
}