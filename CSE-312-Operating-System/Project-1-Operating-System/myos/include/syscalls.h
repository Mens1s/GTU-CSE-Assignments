 
#ifndef __MYOS__SYSCALLS_H
#define __MYOS__SYSCALLS_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <multitasking.h>

namespace myos
{
    
    class SyscallHandler : public hardwarecommunication::InterruptHandler
    {
        
    public:
        // i want to access scheduler without create or doing smth else
        TaskManager* taskManager;
        // static because we want to call them without creating an object of SyscallHandler
        static void sys_exit();
        static void sys_fork();
        static void sys_execve(void (*entrypoint)());
        static void sys_waitpid(int pid);
        static pid_t sys_getPid();
        SyscallHandler(TaskManager* taskManager, hardwarecommunication::InterruptManager* interruptManager, myos::common::uint8_t InterruptNumber);
        ~SyscallHandler();
        
        virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);

    };
    
    
}


#endif