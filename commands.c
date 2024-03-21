#include "commands.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

static List* priorityQ0;
static List* priorityQ1;
static List* priorityQ2;
static List* waitSend;
static List* waitReceive;
static List* semaphoreList;

static int idNum=1;

static PCB* init;
static PCB* runningProcess;


void CreateInit()
{
    init->pid = 0;
    init->priority = -1;
    init->state = RUNNING;

    runningProcess = init;

    return;
}

void CreateQs()
{
    priorityQ0 = List_create();
    priorityQ1 = List_create();
    priorityQ2 = List_create();
    waitSend = List_create();
    waitReceive = List_create();
    semaphoreList = List_create();

    return;
}


int Create (int priority);
{
    PCB* newProcess;
    newProcess->pid = idNum;
    newProcess->priority = priority;
    newProcess->state = READY;

    int successOrFailure = -1;

    if(priority == 0)
    {
        successOrFailure = List_append(priorityQ0, newProcess);
    }
    else if(priority == 1)
    {
        successOrFailure = List_append(priorityQ1, newProcess);
    }
    else if(priority == 2)
    {
        successOrFailure = List_append(priorityQ2, newProcess);
    }

    if(successOrFailure == 0)
    {   
        //increment pid for the next process
        idNum += 1;
        return newProcess->pid;
    }

    return 0;
}




int Fork (void)
{
    //case for the "init" process ("init" would be the very first process, thus its pid is 0)
    if(runningProcess->pid == 0)
    {
        printf("***FORK UNSUCCESSFUL, RUNNING PROCESS IS init***");
        return 0;
    }

    int successOrFailure = -1;

    //append current running process to the corresponding ready Q
    if(runningProcess->priority == 0)
    {
        successOrFailure = List_append(priorityQ0, runningProcess);
    }
    else if(runningProcess->priority == 1)
    {
        successOrFailure = List_append(priorityQ1, runningProcess);
    }
    else if(runningProcess->priority == 2)
    {
        successOrFailure = List_append(priorityQ2, runningProcess);
    }

    // i dont know if this is true since we're doing round robin
    // i actually just dont understand those cases at all
    if(successOrFailure == 0)
    {   
        runningProcess->state = READY;

        //Selecting new current running process
        if(List_count(priorityQ0) > 0)
        {
            runningProcess = List_trim(priorityQ0);
        }
        else if(List_count(priorityQ1) > 0)
        {
            runningProcess = List_trim(priorityQ1);
        }
        else if(List_count(priorityQ2) > 0)
        {
            runningProcess = List_trim(priorityQ2);
        }

        //edge case, if there is no program left besides the 1 program just got forked and init, what should we do

        return runningProcess->pid;
    }

    return 0;
}



bool stateComparator(void* pItem, void* pComparisonArg) 
{
    PCB* item = (PCB*)pItem;
    State comparisonState = *(State*)pComparisonArg;
    return item->state == comparisonState;
}

bool allBlocked()
{
    State ready = READY;
    State running = RUNNING;
    
    if(List_search(priorityQ0, stateComparator, &ready) || List_search(priorityQ0, stateComparator, &running))
    {
        return false;
    }
    else if(List_search(priorityQ1, stateComparator, &ready) || List_search(priorityQ1, stateComparator, &running))
    {
        return false;
    }
    else if(List_search(priorityQ2, stateComparator, &ready) || List_search(priorityQ2, stateComparator, &running))
    {
        return false;
    }

    return true;
}

bool pidComparator(void* pItem, void* pComparisonArg) 
{
    PCB* item = (PCB*)pItem;
    int comparisonPid = *(int*)pComparisonArg;
    return item->pid == comparisonPid;
}

bool Kill (int pid)
{
    //kill the running process
    if(runningProcess->pid == pid && pid != 0)
    {       
        runningProcess=NULL;

        //check if just killed the last read/running process, init needs to be runningall if the other processes are blocked 
        if(allBlocked())
        {
            runningProcess = init;
        }
        //Selecting new current running process
        else if(List_count(priorityQ0) > 0)
        {
            runningProcess = List_trim(priorityQ0);
        }
        else if(List_count(priorityQ1) > 0)
        {
            runningProcess = List_trim(priorityQ1);
        }
        else if(List_count(priorityQ2) > 0)
        {
            runningProcess = List_trim(priorityQ2);
        }

        return true;
    }
    //Search ready Q's to find the corresponding pid
    else if(List_search(priorityQ0, pidComparator, &pid))
    {
        List_remove(priorityQ0);
        return true;
    }
    else if(List_search(priorityQ1, pidComparator, &pid))
    {
        List_remove(priorityQ1);
        return true;
    }
    else if(List_search(priorityQ2, pidComparator, &pid))
    {
        List_remove(priorityQ2);
        return true;
    }
    //case for the "init" process ("init" would be the very first process, thus its pid is 0)
    else if(pid == 0)
    {
        if(List_count(priorityQ0) == 0 && List_count(priorityQ1) == 0 && List_count(priorityQ2) == 0)
        {
            free(init);
            printf("***init IS KILLED, SIMULATION TERMINATED***");
            return true;
        }
        else
        {
            printf("***init IS RUNNING AND THERE ARE OTHER PROCESSES LEFT, KILL UNSUCCESSFUL***");
            return false;
        }
    }

    return false;
}





void Exit (void)
{
    //Kill the process
    Kill(runningProcess->pid);

    return;
}




void Quantum (void)
{
    int successOrFailure=-1;

    //Quantum time over, append current running process to the corresponding ready Q
    if(runningProcess->priority == 0)
    {
        successOrFailure = List_append(priorityQ0, runningProcess);
    }
    else if(runningProcess->priority == 1)
    {
        successOrFailure = List_append(priorityQ1, runningProcess);
    }
    else if(runningProcess->priority == 2)
    {
        successOrFailure = List_append(priorityQ2, runningProcess);
    }

    //Restore process succeed
    if(successOrFailure == 0)
    {   
        runningProcess->state = READY;

        //Selecting new current running process
        if(List_count(priorityQ0) > 0)
        {
            runningProcess = List_trim(priorityQ0);
        }
        else if(List_count(priorityQ1) > 0)
        {
            runningProcess = List_trim(priorityQ1);
        }
        else if(List_count(priorityQ2) > 0)
        {
            runningProcess = List_trim(priorityQ2);
        }
    }

    return;
}

void Send (int pid, char* msg)
{
    if(msg==NULL)
    {
        printf("***SEND MSG IS NULL***");
        return;
    }
    else if(strlen(msg)>40)
    {
        printf("***SEND MSG EXCEEDS MAXIMUM SIZE***");
        return;
    }

    //finding the process to send to
    if(List_search(priorityQ0, pidComparator, &pid))
    {
        PCB* temp=List_curr(priorityQ0);
        strcpy(temp->message, msg);

    }
    else if(List_search(priorityQ1, pidComparator, &pid))
    {
        PCB* temp=List_curr(priorityQ1);
        strcpy(temp->message, msg);
    }
    else if(List_search(priorityQ2, pidComparator, &pid))
    {
        PCB* temp=List_curr(priorityQ2);
        strcpy(temp->message, msg);
    }

    


}

void Receive (void)
{

}

bool Reply (int pid, char* msg)
{

}

// Semaphore comparator based on semaphoreID
bool semComparator(void* pItem, void* pComparisonArg) 
{
    sem* item = (sem*)pItem;
    int comparisonSem = *(int*)pComparisonArg;
    return item->semaphoareID == comparisonSem;
}

bool NewSemaphore (int semaphoreID, int init_val);
{   
    // Max number of semaphores
    if(semaphoreList->count == 5){
        return false;
    }

    // Multiple semaphores with the same ID
    if(semaphoreID < semaphoreList->count){
        return false;
    }
    
    sem* semaphore = (sem*)malloc(sizeof(sem));
    semaphore->value = init_val; // Possibly check for range (0 or higher)
    semaphore->semaphoreID = sempahoreID; // Possibly check for range (0 to 4)
    semaphore->waitingList = NULL;

    List_append(semaphoreList, semaphore);
    return true;
}

bool SemaphoreP (int semaphoreID)
{
    sem* semaphore = List_search(semaphoreList, semComparator, semaphoreID);
    if(semaphore != NULL){
        if(semaphore->value <= 0){
            if(semaphore->waitingList == NULL){
                semaphore->waitingList = List_create();
            }
            List_append(semaphore->waitingList, runningProcess->pid);
            runningProcess->state = BLOCKED;
            return false;
        }
        else{
            semaphore->value --;
            return true;
        }
    }
    else{
        return false;
    }
    
}

bool SemaphoreV (int semaphoreID)
{  
    sem* semaphore = List_search(semaphoreList, semComparator, semaphoreID);
    if(semaphore != NULL){
        semaphore->value ++;
        if(semaphore->waitingList != NULL && semaphore->waitingList->count > 0){
            Node* temp = List_trim(semaphoreList);
            PCB* firstProcess = (PCB*)temp->pItem;
            firstProcess->state = READY;
            free(temp);
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

void Procinfo (int pid)
{

}

void Totalinfo (void)
{

}