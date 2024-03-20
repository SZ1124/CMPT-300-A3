#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include "list.h"

#define BUFFER_SIZE 256

//global var
enum State {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct PCB_s PCB;
struct PCB_s {
    int pid;
    int priority; //0,1,2
    enum State state;
    char message[BUFFER_SIZE];
};

bool CreateInit(); //Called at the start of main.c
int Create (int priority);
int Fork (void);
bool Kill (int pid);
void Exit (void)
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