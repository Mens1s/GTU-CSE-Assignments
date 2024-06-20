
#include <multitasking.h>
#include <lib/stdlib.h>

using namespace myos;
using namespace myos::common;


Task::Task(GlobalDescriptorTable *gdt, void entrypoint(), Priority priority)
{
    runningTime = 0;
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;

    this->pid = 0;
    this->ppid = 0;
    this->priority = priority;
    this->state = State::READY;
    this->parentWait = false;
}

Task::~Task()
{
}

Task::Task()
{
    runningTime = 0;
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    cpustate -> eax = 0; // environmetal audio extension
    cpustate -> ebx = 0; // base register
    cpustate -> ecx = 0; // counter register
    cpustate -> edx = 0; // data register

    cpustate -> esi = 0; // source index
    cpustate -> edi = 0; // destination index
    cpustate -> ebp = 0; // base pointer
    
    /*
    cpustate -> gs = 0; // general segment
    cpustate -> fs = 0; // file segment
    cpustate -> es = 0; // extra segment
    cpustate -> ds = 0; // data segment
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ; // stack pointer
    // cpustate -> ss = ;  // stack segment
    cpustate -> eflags = 0x202;

    this->pid = pid;
    this->ppid = ppid;
    this->priority = Priority::LOW; // default 0 -- LOW for TASK B 3 PART here is HIGH
    this->state = State::READY;
    this->parentWait = false;
}

TaskManager::TaskManager(GlobalDescriptorTable *gdt)
{
    this->gdt = gdt;
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;
    
    // copy task to tasks[numTasks] it is initialized in Task constructor but i do not get it why if i do not impl like these, it does not work
    tasks[numTasks].cpustate->eax = task->cpustate->eax;
    tasks[numTasks].cpustate->ebx = task->cpustate->ebx;
    tasks[numTasks].cpustate->ecx = task->cpustate->ecx;
    tasks[numTasks].cpustate->edx = task->cpustate->edx;
    tasks[numTasks].cpustate->esi = task->cpustate->esi;
    tasks[numTasks].cpustate->edi = task->cpustate->edi;
    tasks[numTasks].cpustate->ebp = task->cpustate->ebp;
    tasks[numTasks].cpustate->eip = task->cpustate->eip;
    tasks[numTasks].cpustate->cs = task->cpustate->cs;
    tasks[numTasks].cpustate->eflags = task->cpustate->eflags;
    tasks[numTasks].cpustate->esp = task->cpustate->esp;
    tasks[numTasks].cpustate->ss = task->cpustate->ss;
    tasks[numTasks].pid = numTasks;
    tasks[numTasks].ppid = task->ppid;
    tasks[numTasks].priority = task->priority;
    tasks[numTasks].state = READY;
    tasks[numTasks].parentWait = false;
    tasks[numTasks].cpustate = (CPUState *)(tasks[numTasks].stack + 4096 - sizeof(CPUState));

    ++numTasks;
    return true;
}

// robin round scheduler Part A 

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if (numTasks <= 0)
        return cpustate;
    
    if (currentTask >= 0)
        tasks[currentTask].cpustate = cpustate;

    // current task go to sleep
    if (tasks[currentTask].state == RUNNING)
        tasks[currentTask].state = READY;

    // get most ready task to run after current task
    do { 
        if (++currentTask >= numTasks)
            currentTask %= numTasks;
    } while (tasks[currentTask].state != READY);
    
    tasks[currentTask].state = RUNNING;

    //if(currentTask != 0) printProcessesTable();
    return tasks[currentTask].cpustate;
}


/*
// priority based preemptive scheduler Part B
CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if (numTasks <= 0)
        return cpustate;
    
    if (currentTask >= 0)
        tasks[currentTask].cpustate = cpustate;

    // current task go to sleep
    if (tasks[currentTask].state == RUNNING)
        tasks[currentTask].state = READY;

    // priority based scheduler
    int possibleTask = -1;
    Priority possibilePriority = Priority::VERY_LOW;
    for(int i = 0; i < numTasks; ++i){
        if(tasks[i].state == READY && tasks[i].priority > possibilePriority){
            possibilePriority = tasks[i].priority;
            possibleTask = i;
        }
    }

    if(possibleTask != -1)
        currentTask = possibleTask;
    
    
    tasks[currentTask].state = RUNNING;

    printProcessesTable();
    return tasks[currentTask].cpustate;
}
*/

// TASK 4 icin scheduler
/*
CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if (numTasks <= 0)
        return cpustate;
    
    if (currentTask >= 0)
        tasks[currentTask].cpustate = cpustate;
    // current task if goes after 1 times of running, it is priority is increased
    if(tasks[currentTask].runningTime >= 5){
        if(tasks[currentTask].priority == Priority::LOW)
            tasks[currentTask].priority = Priority::MEDIUM;
        else if(tasks[currentTask].priority == Priority::MEDIUM)
            tasks[currentTask].priority = Priority::HIGH;
        tasks[currentTask].runningTime = 0;
    }

    // current task go to sleep
    if (tasks[currentTask].state == RUNNING)
        tasks[currentTask].state = READY;

    // get most ready task to run after current task
    do { 
        if (++currentTask >= numTasks)
            currentTask %= numTasks;
    } while (tasks[currentTask].state != READY);
    
    tasks[currentTask].state = RUNNING;

    if(currentTask != 0) printProcessesTable();
    tasks[currentTask].runningTime++;
    return tasks[currentTask].cpustate;
}
*/

// TASK C - Part 2
void TaskManager::IncreasePriority()
{
    Buffer* buffer = new Buffer();
    if(buffer->isCriticalRegion()){
        if(tasks[currentTask].priority == Priority::LOW)
            tasks[currentTask].priority = Priority::MEDIUM;
        else if(tasks[currentTask].priority == Priority::MEDIUM)
            tasks[currentTask].priority = Priority::HIGH;
        tasks[currentTask].runningTime = 0;
    }    
}

pid_t TaskManager::Fork(CPUState* cpustate) {

    if(numTasks >= 256) // MAX_TASKS önceden tanımlanmış bir sabit
        return -1;
    
    // Çocuk işlem için bir PID atanması
    pid_t child_pid = numTasks++;

    // Yeni işlem bilgilerinin kopyalanması
    tasks[child_pid].state = READY;
    tasks[child_pid].ppid = tasks[currentTask].pid;
    tasks[child_pid].pid = child_pid;
    // priority is same as parent
    //tasks[child_pid].priority = tasks[currentTask].priority; // comment while you do not in task b strategy 3

    // Yığının kopyalanması
    for(unsigned int i = 0; i < sizeof(tasks[currentTask].stack); ++i)
        tasks[child_pid].stack[i] = tasks[currentTask].stack[i];
    
    // CPU durumunun güncellenmesi
    uint32_t currentTaskOffSet = (((uint32_t)cpustate) - ((uint32_t)tasks[currentTask].stack));
    tasks[child_pid].cpustate = (CPUState*)(((uint32_t)tasks[child_pid].stack) + currentTaskOffSet);

    tasks[child_pid].cpustate->ecx = 0;

    return tasks[child_pid].pid;
}

void TaskManager::Exit(){ // exit current task
    // bekleyeni var mı? cocuk ölmesi
    for(int i = 0; i < numTasks; ++i){
        if(tasks[i].parentWait && tasks[i].pid == tasks[currentTask].ppid){
            tasks[i].state = State::READY;
            tasks[currentTask].parentWait = false; // gereksiz ölüyorsun zaten
        }
    }
    // beklediği varken exit calismaz o yüzden gerek yok
    tasks[currentTask].state = State::TERMINATED; // burada bi terminated yaptıklarımı silmem ardından yer açmam lazım gibi... 256 tasktan fazlasi test caselerde yok :D
    printProcessesTable();
}

void TaskManager::Wait(pid_t pid){ // wait for child to exit if(pid != 0) wait(pid);
    tasks[currentTask].state = State::BLOCKED;
    // parentin tum cocukları terminated olana kadar beklemesi lazım
    for(int i = 0; i < numTasks; ++i){
        if(tasks[i].ppid == tasks[currentTask].pid && tasks[i].state != State::TERMINATED){
            tasks[i].parentWait = true;
            tasks[currentTask].state = State::BLOCKED;
        }
    }
}

void TaskManager::Execute(void entrypoint()){
    Task *task = new Task(gdt, entrypoint, Priority::HIGH);
    AddTask(task);
}

void TaskManager::printProcessesTable(){
    char *temp = new char[10];
    printf("PID - PPID - PRIORITY - STATE - PARENTWAIT\n");
    for(int i = 0; i < numTasks; ++i){
            // PID - PPID - PRIORITY - STATE - PARENTWAIT
            intToString(tasks[i].pid, temp);
            printf(temp);
            printf(" -- ");
            intToString(tasks[i].ppid, temp);
            printf(temp);
            printf(" -- ");
            if(tasks[i].priority == Priority::LOW)
                printf("LOW");
            else if(tasks[i].priority == Priority::MEDIUM)
                printf("MEDIUM");
            else if(tasks[i].priority == Priority::HIGH)
                printf("HIGH");
            printf(" -- ");
            if(tasks[i].state == State::BLOCKED)
                printf("BLOCKED");
            else if(tasks[i].state == State::READY)
                printf("READY");
            else if(tasks[i].state == State::RUNNING)
                printf("RUNNING");
            else if(tasks[i].state == State::TERMINATED)
                printf("TERMINATED");
            printf(" -- ");
            if(tasks[i].parentWait)
                printf("TRUE");
            else
                printf("FALSE");
            printf("\n");
    }
}

pid_t TaskManager::getPid(){
    return tasks[currentTask].pid;
}