
#include <common/types.h>
#include <lib/buffer.h>
#include <lib/stdlib.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <drivers/ata.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>

#include <drivers/amd_am79c973.h>
#include <net/etherframe.h>
#include <net/arp.h>
#include <net/ipv4.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>

// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;
using namespace myos::net;







class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

class PrintfUDPHandler : public UserDatagramProtocolHandler
{
public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, common::uint8_t* data, common::uint16_t size)
    {
        char* foo = " ";
        for(int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
    }
};


class PrintfTCPHandler : public TransmissionControlProtocolHandler
{
public:
    bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket* socket, common::uint8_t* data, common::uint16_t size)
    {
        char* foo = " ";
        for(int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
        
        
        
        if(size > 9
            && data[0] == 'G'
            && data[1] == 'E'
            && data[2] == 'T'
            && data[3] == ' '
            && data[4] == '/'
            && data[5] == ' '
            && data[6] == 'H'
            && data[7] == 'T'
            && data[8] == 'T'
            && data[9] == 'P'
        )
        {
            socket->Send((uint8_t*)"HTTP/1.1 200 OK\r\nServer: MyOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>My Operating System</title></head><body><b>My Operating System</b> http://www.AlgorithMan.de</body></html>\r\n",184);
            socket->Disconnect();
        }
        
        
        return true;
    }
};


void sysprintf(char* str)
{
    asm("int $0x80" : : "a" (4), "b" (str));
}

void taskA()
{
    while(true)
        sysprintf("A");
}

void taskB()
{
    while(true)
        sysprintf("B");
}


using namespace myos::lib;




// test fork
void testFork(){
    printf("I am the parent...\n");
    pid_t pid = SyscallHandler::sys_getPid();
    printf("My pid is: ");
    char buffer[10];
    intToString(pid, buffer);
    printf(buffer);
    printf("\n");
    SyscallHandler::sys_fork();
    pid = SyscallHandler::sys_getPid();
    printf("My Child pid is: ");
    intToString(pid, buffer);
    printf(buffer);
    printf("\n");
    printf("I am the child but you have to see my parent as like me...\n");
    SyscallHandler::sys_exit();
    while(1);
}

// test fork return value
void testForkReturn(){
    printf("Started...\n");
    pid_t pid = SyscallHandler::sys_getPid();
    printf("My pid is: ");
    char buffer[10];
    intToString(pid, buffer);
    printf(buffer);
    printf("\n");
    SyscallHandler::sys_exit();
    
}

// PART A :: 
void testForkAndCollatzAndRunningLongProgram(){
    int parentPid = SyscallHandler::sys_getPid();
    SyscallHandler::sys_fork();

    if(SyscallHandler::sys_getPid() == parentPid){
        int childPid = SyscallHandler::sys_getPid();

        SyscallHandler::sys_fork();
        if(SyscallHandler::sys_getPid() == childPid){
            long_running_program(); // parentToChild third run
            collatz();
            SyscallHandler::sys_exit();
        } 
        else{
            long_running_program(); // parent first run
            collatz();
            SyscallHandler::sys_exit();
        }
        
    } else {
        long_running_program(); // child second run
        collatz();
        SyscallHandler::sys_exit();
    }

    while(1);
}

// PART B
void firstStrategy(){
    // init processes which is id=0
    int n = random(0, 4);
    char buffer[10];

    // 1 process fork 
    int parentPid = SyscallHandler::sys_getPid();

    SyscallHandler::sys_fork();

    if(parentPid == SyscallHandler::sys_getPid() ){
        // init processes id : 0
    } else{
        int c1= SyscallHandler::sys_getPid(); // id : 1
        SyscallHandler::sys_fork();

        if(c1 == SyscallHandler::sys_getPid()){  // id : 1
            SyscallHandler::sys_fork(); // 2 process come here
            if(n == 0) {printf("collatz");collatz();}
            else if(n == 1) {printf("long");long_running_program();}
            else if(n == 2) {printf("linear");linearSearch();}
            else if(n == 3) {printf("bSearh");binarySearch();}
        }else{ // id : 2
            SyscallHandler::sys_fork();
            SyscallHandler::sys_fork();
            SyscallHandler::sys_fork(); // 8 processes come here
            if(n == 0) {printf("collatz");collatz();}
            else if(n == 1) {printf("long");long_running_program();}
            else if(n == 2) {printf("linear");linearSearch();}
            else if(n == 3) {printf("bSearh");binarySearch();}
        }
        SyscallHandler::sys_exit();
    }   
    
    // there is no possibility to 10 because it is not a power of 2
    while(1);
}

void secondStrategy(){
    int n1 = random(0, 3);
    int n2 = random(0, 3);
    if(n1==n2) n2 = (n2+2) % 4;
    void (*func[4])(void) = {collatz, long_running_program, linearSearch, binarySearch};

    if(n1 == 0) {printf("Firrst Function: collatz\n");}
    else if(n1 == 1) {printf("Firrst Function: long\n");}
    else if(n1 == 2) {printf("Firrst Function: linear\n");}
    else if(n1 == 3) {printf("Firrst Function: bSearh\n");}

    if(n2 == 0) {printf("Second Function: collatz\n");}
    else if(n2 == 1) {printf("Second Function: long\n");}
    else if(n2 == 2) {printf("Second Function: linear\n");}
    else if(n2 == 3) {printf("Second Function: bSearh\n");}

    int parentPid = SyscallHandler::sys_getPid();

    SyscallHandler::sys_fork();

    if(SyscallHandler::sys_getPid() == parentPid){ // first run
        SyscallHandler::sys_fork();
        if(SyscallHandler::sys_getPid() == parentPid){
            // init processes id : 0
        }else{
            func[n1](); // first run
            func[n2]();
            SyscallHandler::sys_exit();
        }
    } else {
        int sPid = SyscallHandler::sys_getPid();
        SyscallHandler::sys_fork();
        
        func[n1](); // second and third run
        func[n2]();
    
        SyscallHandler::sys_exit();
    }

    while(1);
}

void thirdStrategy(){
    int parentPid = SyscallHandler::sys_getPid();

    SyscallHandler::sys_fork();

    if(SyscallHandler::sys_getPid() == parentPid)
        collatzVeryLong(); // i thought it is long running program... 5 interrupt i think
    
    else{
        SyscallHandler::sys_fork();
        binarySearch();
    }
    SyscallHandler::sys_exit();
    while(1);
}

// TODO : özel scheduler yapildi
void fourthdStrategy(){
    int parentPid = SyscallHandler::sys_getPid();
    SyscallHandler::sys_fork();

    if(SyscallHandler::sys_getPid() == parentPid){
        SyscallHandler::sys_fork();
        if(SyscallHandler::sys_getPid() == parentPid){
            // init processes id : 0
        }
        else{
            collatz();
            SyscallHandler::sys_exit(); 
        }
    }
    else{
        collatzVeryLong();
        SyscallHandler::sys_exit();
    }

    while(1);
}

void taskCFirst(){
    int n1 = random(0, 3);
    int n2 = random(0, 3);
    if(n1==n2) n2 = (n2+2) % 4;
    void (*func[4])(void) = {collatz, long_running_program, linearSearch, binarySearch};

    if(n1 == 0) {printf("Firrst Function: collatz\n");}
    else if(n1 == 1) {printf("Firrst Function: long\n");}
    else if(n1 == 2) {printf("Firrst Function: linear\n");}
    else if(n1 == 3) {printf("Firrst Function: bSearh\n");}

    if(n2 == 0) {printf("Second Function: collatz\n");}
    else if(n2 == 1) {printf("Second Function: long\n");}
    else if(n2 == 2) {printf("Second Function: linear\n");}
    else if(n2 == 3) {printf("Second Function: bSearh\n");}

    int parentPid = SyscallHandler::sys_getPid();

    SyscallHandler::sys_fork();

    if(SyscallHandler::sys_getPid() == parentPid){ // first run
        SyscallHandler::sys_fork();
        if(SyscallHandler::sys_getPid() == parentPid){
            // init processes id : 0
        }else{

            Buffer *buffer = new Buffer();
            buffer->setCriticalRegion(true);
            printf("Press 'Enter' to get new task...\n");
            buffer->setDone(false);
            while(!buffer->isDone());
            buffer->setDone(false);
            buffer->setCriticalRegion(false); 
                      
            func[n1](); // first run
            func[n2]();
            SyscallHandler::sys_exit();
        }
    } else {
        int sPid = SyscallHandler::sys_getPid();
        
        Buffer *buffer = new Buffer();
        buffer->setCriticalRegion(true);
        printf("Press 'Enter' to get new task...\n");
        buffer->setDone(false);
        while(!buffer->isDone());
        buffer->setDone(false);
        buffer->setCriticalRegion(false); 

        func[n1](); // second  run
        func[n2]();
    
        SyscallHandler::sys_exit();
    }
    while(1);
}

void taskCSecond(){
    int parentPid = SyscallHandler::sys_getPid();

    SyscallHandler::sys_fork();

    if(SyscallHandler::sys_getPid() == parentPid){
        // init processes id : 0
    }
    else{
        int c1= SyscallHandler::sys_getPid(); // id : 1
        SyscallHandler::sys_fork();

        if(c1 == SyscallHandler::sys_getPid()){  // id : 1 start low
            SyscallHandler::sys_fork();

            if(c1 == SyscallHandler::sys_getPid()) // id : 1 start low
                collatz(); // end with medium priority

            else{ // id : 3
                linearSearch(); 
                collatz(); // end with high priority
            }
        }else{ // id : 2 start low
            long_running_program(); // end with low priority
        }
        SyscallHandler::sys_exit();
    }
    while(1);
}

void doLinearSearch(){
    linearSearch();
    SyscallHandler::sys_exit();
    while(1);
}

void doBinarySearch(){
    binarySearch();
    SyscallHandler::sys_exit();
    while(1);
}

void doCollatz(){
    collatz();
    SyscallHandler::sys_exit();
    while(1);
}

void doLongRunningProgram(){
    long_running_program();
    SyscallHandler::sys_exit();
    while(1);
}



// TODO : remove third
void thirdStrategyHigh(){
    long_running_program();
    SyscallHandler::sys_fork();
    binarySearch();
    SyscallHandler::sys_exit();
    while(1);
}

void thirdStrategyMedium(){
    SyscallHandler::sys_fork();
    linearSearch();
    SyscallHandler::sys_exit();
    while(1);
}

void thirdStrategyLow(){
    collatz();
    SyscallHandler::sys_exit();
    while(1);
}

void twoFork(){
    SyscallHandler::sys_fork();
    SyscallHandler::sys_fork();
    printf("fork\n");
    SyscallHandler::sys_exit();
    while(1);
}



void init(){
    while(1);
}



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("MENS-OS\n");

    GlobalDescriptorTable gdt;
    TaskManager taskManager(&gdt);
    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */

    InterruptManager interrupts(0x20, &gdt, &taskManager);
    SyscallHandler syscalls(&taskManager, &interrupts, 0x80);


    DriverManager drvManager;

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
        

    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvManager.AddDriver(&mouse);
        
    drvManager.ActivateAll();
    
    // DEBUG AREA //
    //taskManager.AddTask(new Task(&gdt, init, Priority::MEDIUM));

    //taskManager.AddTask(new Task(&gdt, readBuffer, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, testFork, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, testFork, Priority::MEDIUM));
    // works as expected
    //taskManager.AddTask(new Task(&gdt, twoFork, Priority::MEDIUM));
    
    // task A 
    taskManager.AddTask(new Task(&gdt, testForkAndCollatzAndRunningLongProgram, Priority::MEDIUM));  

    // task b

    // task b first strategy
    //taskManager.AddTask(new Task(&gdt, init, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, firstStrategy, Priority::MEDIUM));

    // task b second strategy
    //taskManager.AddTask(new Task(&gdt, init, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, secondStrategy, Priority::MEDIUM));

    // task b third strategy
    //taskManager.AddTask(new Task(&gdt, init, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, thirdStrategy, Priority::LOW));

    // expected linear-linear-long-binary-binary-collatz 
    // i arrenged like the vice versa so if there is problem i will see it in scheduler (priority based)
    //taskManager.AddTask(new Task(&gdt, doCollatz, Priority::LOW));
    //taskManager.AddTask(new Task(&gdt, doBinarySearch, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, doBinarySearch, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, doLinearSearch, Priority::HIGH));
    //taskManager.AddTask(new Task(&gdt, doLinearSearch, Priority::HIGH));
    //taskManager.AddTask(new Task(&gdt, doLongRunningProgram, Priority::HIGH));


    //taskManager.AddTask(new Task(&gdt, linearSearch, Priority::MEDIUM));
    //taskManager.AddTask(new Task(&gdt, linearSearch, Priority::MEDIUM));
    
    // task b fourth strategy
    //taskManager.AddTask(new Task(&gdt, fourthdStrategy, Priority::HIGH));
    //taskManager.AddTask(new Task(&gdt, doLinearSearch, Priority::HIGH));

    // task c first strategy
    //taskManager.AddTask(new Task(&gdt, taskCFirst, Priority::MEDIUM));
    
    // task c second strategy
    //taskManager.AddTask(new Task(&gdt, taskCSecond, Priority::HIGH));
    
    

    interrupts.Activate();

    while (1);
    
}
