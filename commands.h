#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include "list.h"

#define BUFFER_SIZE 40

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

typedef struct SEM_s sem;
struct SEM_s {
    int value;
    int semaphoreID;
    List* waitingList;
};

bool CreateInit(); //Called at the start of main.c
void CreateQs() //Called at the start of main.c
int Create (int priority);
int Fork (void);
bool Kill (int pid);
void Exit (void);
void Quantum (void);
void Send(int pid, char* msg); // return type uncertain
void Receive (void); //return type uncertain
bool Reply (int pid, char* msg);
bool NewSemaphore (int semaphoreID, int init_val);
bool SemaphoreP (int semaphoreID);
bool SemaphoreV (int semaphoreID);
void Procinfo (int pid);
void Totalinfo (void);



#endif 