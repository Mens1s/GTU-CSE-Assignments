#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int counter = 1;

void sigchld_handler(int signo) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {

        if (WIFEXITED(status)) 
            printf("Child process %d terminated - expected status: %d\n", pid, WEXITSTATUS(status));
        else 
            printf("Child process %d terminated - not expected\n", pid);
        
        counter--;
    }
    /*
    printf("Counter NOT DECREAMEANTED ------ : %d\n", counter);
    while (waitpid(-1, NULL, WNOHANG) > 0);
    counter--;
    printf("Counter DECREAMEANTED ------ : %d\n", counter);
    */
}

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int userArgument = atoi(argv[1]);

    int sum_result = 0;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);
    
    int arr1[userArgument];
    int arr2[userArgument];
    srand(time(NULL)); // generation of random numbers i used it in the parent process to generate random numbers
    for(int i = 0; i < userArgument; i++){
        arr1[i] = rand() % 10;
        // for reporting purposes
        //printf("Random number: %d:%d\n", i, arr1[i]);
    }

    srand(time(NULL)); // generation of random numbers i used it in the parent process to generate random numbers
    for(int i = 0; i < userArgument; i++){
        arr2[i] = rand() % 10 + 1;
        // for reporting purposes
        //printf("Random number: %d:%d\n", i, arr2[i]);
    }

    int fd1_read = open(FIFO1, O_RDONLY | O_NONBLOCK);
    if (fd1_read == -1) {
        unlink(FIFO1);
        error_exit("open FIFO1 in parent");
    }

    int fd1_write = open(FIFO1, O_WRONLY);
    if (fd1_write == -1) {
        unlink(FIFO1);
        error_exit("open FIFO1 in parent");
    }

    // Generate random numbers
    for(int i = 0; i < userArgument; i++){
        int randomNumbers = arr1[i];
        // for reporting purposes
        printf("Writing FIFO1 number: %d:%d\n",i ,randomNumbers);
        if (write(fd1_write, &randomNumbers, sizeof(int)) == -1) {
            perror("write FIFO1 in parent");
            exit(EXIT_FAILURE);
        }
    }

    int fd2_read = open(FIFO2, O_RDONLY | O_NONBLOCK);
    if (fd2_read == -1) {
        unlink(FIFO2);
        error_exit("open FIFO2 in parent");
    }

    int fd2_write = open(FIFO2, O_WRONLY);
    if (fd2_write == -1) {
        unlink(FIFO2);
        error_exit("open FIFO2 in parent");
    }
    
    char command[] = "multiply";
    if(write(fd2_write, command, sizeof(command)) == -1){
        perror("write FIFO2 in parent");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < userArgument; i++){
        int randomNumbers = arr2[i];
        // for reporting purposes
        printf("Writting FIFO2 number: %d:%d\n", i, randomNumbers);
        if (write(fd2_write, &randomNumbers, sizeof(int)) == -1) {
            perror("write FIFO2 in parent");
            exit(EXIT_FAILURE);
        }
    }

    //signal(SIGCHLD, sigchld_handler);
    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &sig, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    pid_t pid = fork();
    
    if (pid == 0){
        // Child Process 1
        sleep(10);

        int fd1 = open(FIFO1, O_RDONLY);
        if (fd1 == -1) error_exit("open FIFO1 in child");

        // read random nums from FIFO1
        int num, result = 0;

        for(int i = 0; i < userArgument; i++){
            if(read(fd1, &num, sizeof(int)) == -1){
                perror("read FIFO1 in child");
                exit(EXIT_FAILURE);
            }
            // for reporting purposes
            printf("Reading FIFO1 number: %d:%d\n",i ,num);
            result += num;
        }
        sum_result += result; 

        printf("Result of sum from first child is: %d\n", result);
        close(fd1);

        int fd2 = open(FIFO2, O_WRONLY);
        if (fd2 == -1) error_exit("open FIFO2 in child");

        // Write result to FIFO2
        if (write(fd2, &result, sizeof(int)) == -1) {
            perror("write FIFO2 in child");
            exit(EXIT_FAILURE);
        }
        printf("WROTED FIFO2 number CHIILD1: %d\n", result);

        close(fd2);
        close(fd1_read);
        close(fd1_write);
        close(fd2_read);
        close(fd2_write);
        exit(EXIT_SUCCESS);
    }else if(pid > 0){
        pid_t pid2 = fork();

        if(pid2 == 0){
            // Child Process 2
            sleep(10);
            
            int fd2 = open(FIFO2, O_RDONLY);
            if (fd2 == -1) error_exit("open FIFO2 in child");

            // Read command, perform multiplication, write result to FIFO2
            char command[9];
            if (read(fd2, command, sizeof(command)) == -1) {
                perror("read FIFO2 in child");
                exit(EXIT_FAILURE);
            }

            if (strcmp(command, "multiply") != 0) {
                fprintf(stderr, "Invalid command received: %s\n", command);
                fd2 = open(FIFO2, O_RDONLY);
                close(fd2);
                exit(EXIT_FAILURE);
            }

            int num, result = 1;

            for(int i = 0; i < userArgument; i++){
                if(read(fd2, &num, sizeof(int)) == -1){
                    perror("read FIFO2 in child");
                    exit(EXIT_FAILURE);
                }
                // for reporting purposes
                printf("Reading FIFO2 number: %d:%d\n",i ,num);
                result *= num;
            }

            printf("Result of multiplication from second child is: %d\n", result);
            sum_result += result;
            close(fd2);

            int fifoMultResult = 0;
            fd2 = open(FIFO2, O_RDONLY);
            if (fd2 == -1) error_exit("open FIFO2 in child");
            // Write result to FIFO1

            if (read(fd2, &fifoMultResult, sizeof(int)) == -1) {
                perror("write FIFO1 in child");
                exit(EXIT_FAILURE);
            }

            // for reporting purposes
            printf("Reading FIFO2 number CHIILD1: %d\n", fifoMultResult);
            sum_result = fifoMultResult + result;
            printf("Sum of results: %d\n", sum_result);

            close(fd2);

            close(fd1_read);
            close(fd1_write);
            close(fd2_read);
            close(fd2_write);
            exit(EXIT_SUCCESS);
        }else if(pid2 > 0){
            counter++;
            //printf("Parent Process\n");
            
        }else{
            error_exit("fork");
        }
    }else{
        error_exit("fork");
    }

    if(counter == 0){
        error_exit("fork");
    }
    while(counter > 0){
        printf("Proceeding...\n");
        sleep(2);
    }

    close(fd1_read);
    close(fd1_write);
    close(fd2_read);
    close(fd2_write);
    
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}
