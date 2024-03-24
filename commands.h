#include "commands.h"

List* priorityQ0;
List* priorityQ1;
List* priorityQ2;
List* waitSendQ;
List* waitReceiveQ;
List* semaphoreList;

int idNum = 1;

PCB* init;
PCB* runningProcess;

void CreateInit()
{
    init = malloc(sizeof(PCB));
    init->pid = 0;
    init->sendPid = -1;
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
    waitSendQ = List_create();
    waitReceiveQ = List_create();
    semaphoreList = List_create();

    return;
}

int prependProcess(PCB* process)
{
    switch(process->priority){
        case 0:
            return List_prepend(priorityQ0, process);
            break;

        case 1:
            return List_prepend(priorityQ1, process);
            break;

        case 2:
            return List_prepend(priorityQ2, process);
            break;
        
        default:
            printf("Wrong priority input...\n");
            return -1;

    }
}

int Create (int priority)
{
    PCB* newProcess = malloc(sizeof(PCB));
    newProcess->pid = idNum;
    init->sendPid = -1;
    newProcess->priority = priority;
    newProcess->state = READY;

    // First process
    if(newProcess->pid == 1)
    {
        runningProcess->state = READY;
        runningProcess = newProcess;
        newProcess->state = RUNNING;
        idNum += 1;

        return newProcess->pid;
    }

    int flag = prependProcess(newProcess);
    
    if(flag == 0)
    {   
        idNum += 1;
        return newProcess->pid;
    }

    //returning 0 indicating fail
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

    int returnPid = Create(runningProcess->priority);

    if(returnPid != 0)
    {   
        return returnPid;
    }

    return 0;
}

bool stateComparator(void* pItem, void* pComparisonArg) 
{
    PCB* item = (PCB*)pItem;
    enum State comparisonState = *(enum State*)pComparisonArg;
    return item->state == comparisonState;
}

bool allBlocked()
{
    enum State ready = READY;
    
    List_first(priorityQ0);
    List_first(priorityQ1);
    List_first(priorityQ2);

    //check Q0
    if(List_search(priorityQ0, stateComparator, &ready))
    {
        return false;
    }

    //check Q1
    if(List_search(priorityQ1, stateComparator, &ready))
    {
        return false;
    }

    //check Q2
    if(List_search(priorityQ2, stateComparator, &ready))
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

bool selectNewProcess()
{
    List_first(priorityQ0);
    List_first(priorityQ1);
    List_first(priorityQ2);

    //check if just killed the last ready/running process, init needs to be running if the other processes are blocked 
    if((List_count(priorityQ0) == 0 && List_count(priorityQ1) == 0 && List_count(priorityQ2) == 0) || allBlocked())
    {
        runningProcess = init;
        runningProcess->state = RUNNING;

        return true;
    }

    //Selecting new current running process
    //Check Q0
    if(List_count(priorityQ0) > 0)
    {
        List_last(priorityQ0);
        while(((PCB*)List_curr(priorityQ0))->state != READY)
        {
            //when the current item is beyond the start of the list
            if(List_prev(priorityQ0) == NULL)
            {
                List_first(priorityQ0);
                break;
            }
            else
            {
                List_prev(priorityQ0);
            }
        }

        if(((PCB*)List_curr(priorityQ0))->state == READY)
        {
            runningProcess = (PCB*)List_remove(priorityQ0);
            runningProcess->state = RUNNING;

            return true;
        }
    }

    //Check Q1
    if(List_count(priorityQ1) > 0)
    {
        List_last(priorityQ1);
        while(((PCB*)List_curr(priorityQ1))->state != READY)
        {
            //when the current item is beyond the start of the list
            if(List_prev(priorityQ1) == NULL)
            {
                List_first(priorityQ1);
                break;
            }
            else
            {
                List_prev(priorityQ1);
            }
        }

        if(((PCB*)List_curr(priorityQ1))->state == READY)
        {
            runningProcess = (PCB*)List_remove(priorityQ1);
            runningProcess->state = RUNNING;

            return true;
        }
    }

    //Check Q2
    if(List_count(priorityQ2) > 0)
    {
        List_last(priorityQ2);
        while(((PCB*)List_curr(priorityQ2))->state != READY)
        {
            //when the current item is beyond the start of the list
            if(List_prev(priorityQ2) == NULL)
            {
                List_first(priorityQ2);
                break;
            }
            else
            {
                List_prev(priorityQ2);
            }
        }

        if(((PCB*)List_curr(priorityQ2))->state == READY)
        {
            runningProcess = (PCB*)List_remove(priorityQ2);
            runningProcess->state = RUNNING;

            return true;
        }
    }
    return false;
}

bool Kill (int pid)
{
    //kill running processs and it is not "init" process
    if(runningProcess->pid == pid && pid != 0)
    {
        
        PCB* temp = runningProcess; 
        runningProcess = NULL;  
        free(temp);    

        return selectNewProcess();
    }
    //Search ready Q's to find the corresponding pid
    List_first(priorityQ0);
    List_first(priorityQ1);
    List_first(priorityQ2);

    if(List_search(priorityQ0, pidComparator, &pid))
    {
        PCB* temp = (PCB*)List_remove(priorityQ0);; 
        free(temp);
        return true;
    }
    else if(List_search(priorityQ1, pidComparator, &pid))
    {
        PCB* temp = (PCB*)List_remove(priorityQ1);
        free(temp);
        return true;
    }
    else if(List_search(priorityQ2, pidComparator, &pid))
    {
        PCB* temp = List_remove(priorityQ2);
        free(temp);
        return true;
    }
    //case for the "init" process ("init" would be the very first process, thus its pid is 0)
    else if(pid == 0)
    {
        //when init is the only process left, and it is running
        if(runningProcess->pid == 0 && List_count(priorityQ0) == 0 && List_count(priorityQ1) == 0 && List_count(priorityQ2) == 0)
        {
            free(init);
            printf("***init IS KILLED, SIMULATION TERMINATED***");
            return true;
        }
        else
        {
            printf("***THERE ARE OTHER PROCESSES LEFT, KILL OF init UNSUCCESSFUL***");
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

int Quantum (void)
{   
    
    //if there's no process left or all the other processes are blocked, except the currently running process:
    //Based on Round Robin, the currently running process will still be running until it finishes
    if((List_count(priorityQ0) == 0 && List_count(priorityQ1) == 0 && List_count(priorityQ2) == 0) || allBlocked())
    {
        return runningProcess->pid;
    }


    int flag;
    PCB* currProcess = runningProcess;

    //Quantum time over, prepend current running process to the corresponding ready Q, or stop "init" from running
    if(runningProcess->pid == 0)
    {
        runningProcess->state = READY;

        if(init->state == READY)
        {
            flag = 0;
        }      
    }
    else{
        flag = prependProcess(runningProcess);
    }

    //Restore process succeed
    if(flag == 0)
    {   
        selectNewProcess();
        currProcess->state = READY;

    }

    return runningProcess->pid;
}

bool sendPidComparator(void* pItem, void* pComparisonArg) 
{
    PCB* item = (PCB*)pItem;
    int comparisonSendPid = *(int*)pComparisonArg;
    return item->sendPid == comparisonSendPid;
}

void Send (int pid, char* msg)
{
    List_first(waitSendQ);

    if(msg == NULL)
    {
        printf("***SEND MESSAGE IS NULL***");
        return;
    }
    else if(strlen(msg) > 40)
    {
        printf("***SEND MESSAGE EXCEEDS MAXIMUM SIZE***");
        return;
    }
    //when there's already a process send message to the process with the pid in parameter
    else if(List_search(waitSendQ, sendPidComparator, &pid))
    {
        printf("***A MESSAGE ALREADY SENT TO THE RECEIVER, PLEASE TRY AGAIN AFTER THAT MESSAGE HAS BEEN RECEIVED***");
        return;
    }
    //cannot send to "init", in order to prevent "init" from being blocked at receive
    else if(pid == 0)
    {
        printf("***CANNOT SEND TO init***");
        return;
    }
    //cannot use "init" to send
    else if(runningProcess->pid == 0)
    {
        printf("***init IS RUNNNING, CANNOT SEND WITH init***");
        return;
    }

    int flag = -1;

    //Store the receiver info
    runningProcess->sendPid = pid;
    strcpy(runningProcess->sendMessage, msg);
    runningProcess->state = BLOCKED;

    //prepend the running process to the blocked Q for send
    flag = List_prepend(waitSendQ, runningProcess);
    
    //Prepend process to blocked Q succeed
    if(flag == 0)
    {   
        List_first(waitReceiveQ);

        //if the receiver is already blocked and placed in the blocked Q for receive
        if(List_search(waitReceiveQ, pidComparator, &pid))
        {
            //unblock the receiver
            PCB* temp = List_remove(waitReceiveQ);
            temp->state = READY;
            strcpy(temp->message, msg);


            flag = prependProcess(temp);

            if(flag == 0)
            {   
                printf("***SUCCESSFULLY UNBLOCKED THE RECEIVER BY SENDING IT A MESSAGE***");
                return;
            }

            printf("***UNBLOCK RECEIVER UNSUCCESSFUL***");
            return;
        }

        selectNewProcess();
    }

    return;
}

void Receive (void)
{
    //prevent init being blocked by receive
    if(runningProcess->pid == 0)
    {
        printf("***init IS RUNNING, CANNOT RECEIVE WITH init***");
        return;
    }

    int receiverPid = runningProcess->pid;
    List_first(waitSendQ);

    //check the waitSendQ
    if(List_search(waitSendQ, sendPidComparator, &receiverPid))
    {
        PCB* temp = List_curr(waitSendQ);
        strcpy(runningProcess->message, temp->sendMessage);
    }
    //if there's no message
    else
    {
        //block the currently running process
        int flag= -1;
        runningProcess->state = BLOCKED;
        flag = List_prepend(waitReceiveQ, runningProcess);

        //prepend process to blocked Q succeed
        if(flag == 0)
        {
            selectNewProcess();
        }
    }

    return;
}

bool Reply (int pid, char* msg)
{
    if(msg == NULL)
    {
        printf("***REPLY MSG IS NULL***");
        return false;
    }
    else if(strlen(msg) > 40)
    {
        printf("***REPLY MSG EXCEEDS MAXIMUM SIZE***");
        return false;
    }

    //check if the pid never did a send. Or if the replied sender is already unblocked and is inside the ready Q, update message
    if(List_search(priorityQ0, pidComparator, &pid))
    {
        PCB* temp = (PCB*)List_curr(priorityQ0);

        if(strlen(temp->sendMessage) == 0)
        {
            printf("***ENTERED PID NEVER DID A SEND BEFORE***");
            return false;
        }

        //Send reply
        strcpy(temp->message, msg);

    }
    else if(List_search(priorityQ1, pidComparator, &pid))
    {
        PCB* temp = (PCB*)List_curr(priorityQ1);

        if(strlen(temp->sendMessage) == 0)
        {
            printf("***ENTERED PID NEVER DID A SEND BEFORE***");
            return false;
        }

        //Send reply
        strcpy(temp->message, msg);

    }
    else if(List_search(priorityQ2, pidComparator, &pid))
    {
        PCB* temp = (PCB*)List_curr(priorityQ2);

        if(strlen(temp->sendMessage) == 0)
        {
            printf("***ENTERED PID NEVER DID A SEND BEFORE***");
            return false;
        }

        //Send reply
        strcpy(temp->message, msg);

    }

    List_first(waitSendQ);

    //if the sender is blocked and placed in the blocked Q
    if(List_search(waitSendQ, pidComparator, &pid))
    {
        //unblock the sender
        PCB* temp = List_remove(waitSendQ);
        temp->state = READY;
        strcpy(temp->message, msg); //STORE REPLY IN THE SENDER'S MESSAGE BUFFER, MIGHT CONSIDER ADD ANOTHER MESSAGE BUFFER FOR REPLY

        int flag;
        flag = prependProcess(temp);

        if(flag == 0){
            //print the replier info and the reply message in sender's message buffer
            printf("***SUCCESSFULLY UNBLOCKED THE SENDER BY SENDING IT A REPLY***\n");
            return true;
        }
        else{
            printf("fail to prepend process: %d\n",temp->pid);
        }
    }

    printf("***UNBLOCK SENDER UNSUCCESSFUL***");
    return false;
}

// Semaphore comparator based on semaphoreID
bool semComparator(void* pItem, void* pComparisonArg) 
{
    sem* item = (sem*)pItem;
    int comparisonSem = *(int*)pComparisonArg;
    return item->semaphoreID == comparisonSem;
}

bool NewSemaphore (int semaphoreID, int init_val)
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
    semaphore->semaphoreID = semaphoreID; // Possibly check for range (0 to 4)
    semaphore->waitingList = List_create();

    List_prepend(semaphoreList, semaphore);
    return true;
}

bool SemaphoreP (int semaphoreID)
{
    List_first(semaphoreList);
    sem* semaphore = List_search(semaphoreList, semComparator, (void*)&semaphoreID);
    if(semaphore != NULL){  
        printf("Semaphore ID: %d, Value: %d\n", semaphore->semaphoreID, semaphore->value);
    }
    if(semaphore != NULL){
        if(semaphore->value <= 0){
            if(semaphore->waitingList == NULL){
                semaphore->waitingList = List_create();
            }
            List_prepend(semaphore->waitingList, runningProcess);
            runningProcess->state = BLOCKED;
            printf("Blocking process with pid: %d\n", runningProcess->pid);
            prependProcess(runningProcess);
            selectNewProcess();
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
    List_first(semaphoreList);

    sem* semaphore = List_search(semaphoreList, semComparator, (void*)&semaphoreID);
    if(semaphore != NULL){  
        printf("Semaphore ID: %d, Value: %d\n", semaphore->semaphoreID, semaphore->value);
    }
    if(semaphore != NULL){
        semaphore->value ++;
        if(semaphore->waitingList != NULL && semaphore->waitingList->count > 0){
            PCB* temp = List_trim(semaphore->waitingList);
            temp->state = READY;
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

const char* stateToString(enum State state){
    switch(state){
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case BLOCKED:
            return "BLOCKED";
        default:
            return "UNKNOWN STATE";
    }

}

void printInfo(PCB* process)
{
    printf("PID: %d\n", process->pid);
    printf("Priority: %d\n", process->priority);
    printf("State: %s\n", stateToString(process->state));
    printf("Message: %s\n", process->message);
    printf("sendMessage: %s\n", process->sendMessage);
}

void Procinfo (int pid)
{
    PCB* process = NULL;

    //Re-positioning list pointers
    List_first(priorityQ0);
    List_first(priorityQ1);
    List_first(priorityQ2);

    // Seraching through three queues
    if(List_search(priorityQ0, pidComparator, &pid)){
        process = (PCB*)List_curr(priorityQ0);
    }
    else if(List_search(priorityQ1, pidComparator, &pid)){
        process = (PCB*)List_curr(priorityQ1);
    }
    else if(List_search(priorityQ2, pidComparator, &pid)){
        process = (PCB*)List_curr(priorityQ2);
    }
    
    if(runningProcess->pid == pid){
        printInfo(runningProcess);
        return;
    }

    if(process == NULL){
        printf("No process with such PID exists.\n");
        return;
    }

    if(pid == 0){
        printInfo(init);
    }
    else{
        printInfo(process);
    }
}

void Totalinfo (void)
{
    PCB* temp;
    List_first(priorityQ0);
    List_first(priorityQ1);
    List_first(priorityQ2);
    List_first(waitSendQ);
    List_first(waitReceiveQ);
    while(List_curr(priorityQ0) != NULL){
        temp = (PCB*)List_curr(priorityQ0);
        printInfo(temp);
        List_next(priorityQ0);
    }
    while(List_curr(priorityQ1) != NULL){
        temp = (PCB*)List_curr(priorityQ1);
        printInfo(temp);
        List_next(priorityQ1);
    }
    while(List_curr(priorityQ2) != NULL){
        temp = (PCB*)List_curr(priorityQ2);
        printInfo(temp);
        List_next(priorityQ2);
    }
    while(List_curr(waitSendQ) != NULL){
        temp = (PCB*)List_curr(waitSendQ);
        printInfo(temp);
        List_next(waitSendQ);
    }
    while(List_curr(waitReceiveQ) != NULL){
        temp = (PCB*)List_curr(waitReceiveQ);
        printInfo(temp);
        List_next(waitReceiveQ);
    }
    printInfo(init);
    if(runningProcess->pid != 0){
        printInfo(runningProcess);
    }
}
