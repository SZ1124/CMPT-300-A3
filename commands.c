#include "commands.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static List* priorityQ0;
static List* priorityQ1;
static List* priorityQ2;

static List* waitSend;
static List* waitReceive;

static int idNum=1;

static PCB* init;
static PCB* runningProcess;

void CreateInit()
{
    init->pid=0;
    init->priority=-1;
    init->state=RUNNING;

    runningProcess=init;

    return;
}


int Create (int priority);
{
    PCB* newProcess;
    newProcess->pid=idNum;
    newProcess->priority=priority;
    newProcess->state=READY;

    int successOrFailure=-1;

    if(priority==0)
    {
        successOrFailure=List_append(priorityQ0, newProcess);
    }
    else if(priority==1)
    {
        successOrFailure=List_append(priorityQ1, newProcess);
    }
    else if(priority==2)
    {
        successOrFailure=List_append(priorityQ2, newProcess);
    }

    if(successOrFailure==0)
    {   
        idNum+=1;
        return pid;
    }

    return 0;
}




int Fork (void)
{
    //case for the "init" process ("init" would be the very first process, thus its pid is 0)
    if(runningProcess->pid==0)
    {
        printf("***FORK UNSUCCESSFUL, RUNNING PROCESS IS init***");
        return 0;
    }

    int successOrFailure=-1;

    //append current running process to the corresponding ready Q
    if(runningProcess->priority==0)
    {
        successOrFailure=List_append(priorityQ0, runningProcess);
    }
    else if(runningProcess->priority==1)
    {
        successOrFailure=List_append(priorityQ1, runningProcess);
    }
    else if(runningProcess->priority==2)
    {
        successOrFailure=List_append(priorityQ2, runningProcess);
    }

    if(successOrFailure==0)
    {   
        runningProcess->state=READY;

        //Selecting new current running process
        if(List_count(priorityQ0)>0)
        {
            runningProcess=List_trim(priorityQ0);
        }
        else if(List_count(priorityQ1)>0)
        {
            runningProcess=List_trim(priorityQ1);
        }
        else if(List_count(priorityQ2)>0)
        {
            runningProcess=List_trim(priorityQ2);
        }

        //edge case, if there is no program left besides the 1 program just got forked and init, what should we do

        return runningProcess->pid;
    }

    return 0;
}



//Helper functions for List_search
bool checkState(void* state, void* pComparisonArg)
{
    if(temp==pComparisonArg)
    {
        return 1;
    }

    return 0;
}
bool allBlocked()
{
    State ready=READY;
    State running=RUNNING;
    
    if(List_search(priorityQ0, checkState, &ready) || List_search(priorityQ0, checkState, &running))
    {
        return false;
    }
    else if(List_search(priorityQ1, checkState, &ready) || List_search(priorityQ1, checkState, &running))
    {
        return false;
    }
    else if(List_search(priorityQ2, checkState, &ready) || List_search(priorityQ2, checkState, &running))
    {
        return false;
    }

    return true;
}
bool checkPid(void* pid, void* pComparisonArg)
{
    if(pid==pComparisonArg)
    {
        return 1;
    }

    return 0;
}

bool Kill (int pid)
{
    //kill the running process
    if(runningProcess->pid==pid && pid!=0)
    {       
        runningProcess=NULL;

        //check if just killed the last read/running process, init needs to be runningall if the other processes are blocked 
        if(allBlocked())
        {
            runningProcess=init;
        }
        //Selecting new current running process
        else if(List_count(priorityQ0)>0)
        {
            runningProcess=List_trim(priorityQ0);
        }
        else if(List_count(priorityQ1)>0)
        {
            runningProcess=List_trim(priorityQ1);
        }
        else if(List_count(priorityQ2)>0)
        {
            runningProcess=List_trim(priorityQ2);
        }

        return true;
    }
    //Search ready Q's to find the corresponding pid
    else if(List_search(priorityQ0, checkPid, &pid))
    {
        List_remove(priorityQ0);
        return true;
    }
    else if(List_search(priorityQ1, checkPid, &pid))
    {
        List_remove(priorityQ1);
        return true;
    }
    else if(List_search(priorityQ2, checkPid, &pid))
    {
        List_remove(priorityQ2);
        return true;
    }
    //case for the "init" process ("init" would be the very first process, thus its pid is 0)
    else if(pid==0)
    {
        if(List_count(priorityQ0)==0 && List_count(priorityQ1)==0 && List_count(priorityQ2)==0)
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
    if(runningProcess->priority==0)
    {
        successOrFailure=List_append(priorityQ0, runningProcess);
    }
    else if(runningProcess->priority==1)
    {
        successOrFailure=List_append(priorityQ1, runningProcess);
    }
    else if(runningProcess->priority==2)
    {
        successOrFailure=List_append(priorityQ2, runningProcess);
    }

    //Restore process succeed
    if(successOrFailure==0)
    {   
        runningProcess->state=READY;

        //Selecting new current running process
        if(List_count(priorityQ0)>0)
        {
            runningProcess=List_trim(priorityQ0);
        }
        else if(List_count(priorityQ1)>0)
        {
            runningProcess=List_trim(priorityQ1);
        }
        else if(List_count(priorityQ2)>0)
        {
            runningProcess=List_trim(priorityQ2);
        }
    }

    return;
}

//Send
//Receive

bool Reply (int pid, char* msg)
{

}

bool NewSemaphore (int semaphore, int init_val);
{

}

bool SemaphoreP (int semaphore)
{

}

bool SemaphoreV (int semaphore)
{

}

void Procinfo (int pid)
{

}

void Totalinfo (void)
{

}
