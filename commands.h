#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include "list.h"

#define BUFFER_SIZE 256

List* priorityQ0;
List* priorityQ1;
List* priorityQ2;

List* waitSend;
List* waitReceive;


//global var
enum state {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct PCB_s PCB;
struct PCB_s {
    int pid;
    int priority; //0,1,2
    enum state;
    char message[BUFFER_SIZE];
};


int Create (int priority);
int Fork (void);
bool Kill (int pid);
//Exit (void); 
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