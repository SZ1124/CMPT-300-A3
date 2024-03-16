#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

typedef struct PCB_s PCB;
struct PCB_s {
    int pid;
    int priority;
    char state [5];
};

int Create (int priority);
int Fork (void);
bool Kill (int pid);
//Exit, use 
//Quantum
//Send
//Receive
bool Reply (int pid, char* msg);
bool NewSemaphore (int semaphore, int init_val);
bool SemaphoreP (int semaphore);
bool SemaphoreV (int semaphore);
void Procinfo (int pid);
void Totalinfo (void);



#endif 