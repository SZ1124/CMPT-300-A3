#include "list.h"
#include "commands.h"
#include <stdio.h>
#include <string.h>

// need to check for init being killed
int main() {
    printf("Welcome to the OS sim!\n");

    char input[100];
    char command;
    int first_arg;
    int second_arg;
    char msg[100];
    CreateInit();
    CreateQs();

    printf("NOTE: ---Please enter one line commands, here are some formats--- \n");
    printf("      Create:  C priority# (such as 'C 1')\n");
    printf("      Kill:    K pid# (such as 'K 1')\n");
    printf("      Send:    S pid# Message (such as 'S 1 check')\n");
    printf("      Reply:   Y pid# Message (such as 'Y 1 check')\n");
    printf("      New Sem: N semID# initalVal# (such as 'N 1 1')\n");
    printf("      Sem P:   P semID# (such as 'P 1')\n");
    printf("      Sem V:   V semID# (such as 'V 1')\n");
    printf("      P info:  I pid# (such as 'P 1')\n");


    while(1){
        printf("\n$ ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (sscanf(input, "%c", &command) != 1) {
            printf("Invalid input format\n");
            return 1;
        }


        switch(command){
            case 'C':
                if (sscanf(input, "%*c %d", &first_arg) != 1) {
                    printf("Invalid input format for command 'I'\n");
                    break;
                }

                if(first_arg>2 || first_arg<0)
                {
                    printf("Invalid priority, please choose between 0,1,2");
                    break;
                }
                printf("Priority for Create: %d\n", first_arg);
                Create(first_arg);
                break;
            
            case 'F':
                printf("Forking...\n");
                Fork();
                break;

            case 'K':
                if (sscanf(input, "%*c %d", &first_arg) != 1) {
                    printf("Invalid input format for command 'I'\n");
                    break;
                }
                printf("PID to kill: %d\n", first_arg);
                Kill(first_arg);

                if(checkInit())
                {
                    return 1;
                }

                break;

            case 'E':
                printf("Exiting...\n");
                Exit();

                if(checkInit())
                {
                    return 1;
                }

                break;
            
            case 'Q':
                printf("Quantum...\n");
                Quantum();
                break;
            
            case 'S':
                if (sscanf(input, "%*c %d %[^\n]", &first_arg, msg) != 2) {
                    printf("Invalid input format for command 'S'\n");
                    break;
                }
                printf("PID: %d, Message: %s\n", first_arg, msg);
                Send(first_arg, msg);
                break;
            
            case 'R':
                printf("Receiving...\n");
                Receive();
                break;
            
            case 'Y':
                if (sscanf(input, "%*c %d %[^\n]", &first_arg, msg) != 2) {
                    printf("Invalid input format for command 'S'\n");
                    break;
                }
                printf("PID: %d, Message: %s\n", first_arg, msg);
                Reply(first_arg, msg);
                break;
            
            case 'N':
                if (sscanf(input, "%*c %d %d", &first_arg, &second_arg) != 2) {
                    printf("Invalid input format for command 'V'\n");
                    break;
                }
                printf("Semaphore: %d, Initial value: %d\n", first_arg, second_arg);
                NewSemaphore(first_arg, second_arg);
                break;
            
            case 'P':
                if (sscanf(input, "%*c %d", &first_arg) != 1) {
                    printf("Invalid input format for command 'I'\n");
                    break;
                }
                printf("Semaphore: %d\n", first_arg);
                SemaphoreP(first_arg);
                break;
            
            case 'V':
                if (sscanf(input, "%*c %d", &first_arg) != 1) {
                    printf("Invalid input format for command 'I'\n");
                    break;
                }
                printf("Semaphore: %d\n", first_arg);
                SemaphoreV(first_arg);
                break;

            case 'I':
                if (sscanf(input, "%*c %d", &first_arg) != 1) {
                    printf("Invalid input format for command 'I'\n");
                    break;
                }
                printf("Printing PID: %d info\n", first_arg);
                Procinfo(first_arg);
                break;

            case 'T':
                printf("Total info...\n");
                Totalinfo();
                break;

            default:
                printf("Invalid command entered.\n");
        }

        printf("\nCurrently running is: %d",Quantum());
        
    }
    
    return 0;
}                
