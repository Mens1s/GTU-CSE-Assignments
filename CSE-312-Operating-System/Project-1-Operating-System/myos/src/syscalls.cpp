
#include <syscalls.h>
#include <lib/stdlib.h>
 
using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;;

SyscallHandler::SyscallHandler(TaskManager *taskManager, InterruptManager* interruptManager, uint8_t InterruptNumber)
:    InterruptHandler(interruptManager, InterruptNumber  + interruptManager->HardwareInterruptOffset())
{
    this->taskManager = taskManager;
}

SyscallHandler::~SyscallHandler()
{
}
void SyscallHandler::sys_exit(){
    asm("int $0x80" : : "a" (1));
}

void SyscallHandler::sys_fork(){
    asm("int $0x80" : : "a" (2));
}

void SyscallHandler::sys_waitpid(int pid){
    asm("int $0x80" : : "a" (3), "b"(pid));
}

pid_t SyscallHandler::sys_getPid(){
    pid_t pid = 0;
    asm("int $0x80" : "=c" (pid) : "a" (5));
    return pid;
}

void SyscallHandler::sys_execve(void (*entrypoint)()) {
    asm("int $0x80" : : "a" (6), "b" (entrypoint));
}


uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState* cpu = (CPUState*)esp;
    

    switch(cpu->eax)
    {
        case 1:
            taskManager->Exit();
            break;
        case 2:
            cpu->ecx=taskManager->Fork(cpu);
            return InterruptHandler::HandleInterrupt(esp);
        case 3:
            taskManager->Wait(cpu->ebx);
            break;
        case 4:
            printf((char*)cpu->ebx);
            break;
        case 5:
            cpu->ecx = taskManager->getPid();
            break;
        case 6:
            taskManager->Execute((void (*)())cpu->ebx);
            break;
        default:
            break;
    }

    
    return esp;
}

