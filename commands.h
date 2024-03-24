#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#define BUFFER_SIZE 40

enum State {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct PCB_s PCB;
struct PCB_s {
    int pid;
    int sendPid;
    int pid; //Origianl PID
    int sendPid; //Receiver's PID
    int priority; //0,1,2
    enum State state;
    char message[BUFFER_SIZE];
    char sendMessage[BUFFER_SIZE];
    enum State state; //Process state
    char message[BUFFER_SIZE]; //Received Message
    char sendMessage[BUFFER_SIZE]; //Sending Message
};

typedef struct SEM_s sem;
struct SEM_s {
    int value;
    int semaphoreID;
    List* waitingList;
};

void CreateInit(void); //Called at the start of main.c
void CreateQs(void); //Called at the start of main.c
int Create (int priority);
int Fork (void);
bool Kill (int pid);
void Exit (void);
int Quantum (void);
void Send(int pid, char* msg); // return type uncertain
void Receive (void); //return type uncertain
bool Reply (int pid, char* msg);
bool NewSemaphore (int semaphoreID, int init_val);
bool SemaphoreP (int semaphoreID);
bool SemaphoreV (int semaphoreID);
void Procinfo (int pid);
