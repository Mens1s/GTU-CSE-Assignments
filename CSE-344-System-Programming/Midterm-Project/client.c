#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>

#include "include/common.h"

Request* request;
int isConnected = 0;

void sigint_handler(int signum) {
    printf("CTRL-C Detected... The Server Has Been Notified..\n");
    
    
    char clientFifoPath[BUF_SIZE];
    char tempClientFifoPath[BUF_SIZE] = CLIENT_FIFO_PATH_TEMPLATE;
    sprintf(clientFifoPath, tempClientFifoPath, getpid());

    // control of connection
    if(isConnected == 0) {
        free(request);
        unlink(clientFifoPath);
        exit(EXIT_SUCCESS);
    }

    int fdClient = open(clientFifoPath, O_WRONLY | O_NONBLOCK);
    if (fdClient == -1) {
        free(request);
        unlink(clientFifoPath);
        perror("open");
        exit(EXIT_FAILURE);
    }

    Request requestQuit;
    strcpy(requestQuit.command, "quit");
    requestQuit.pid = getpid();
    requestQuit.argsCount = 0;

    if (write(fdClient, &requestQuit, sizeof(Request)) == -1) {
        if (errno == EAGAIN) {
            free(request);
            unlink(clientFifoPath);
            printf("Error: Server is not ready to read from the FIFO\n");
            exit(EXIT_FAILURE);
        } else {
            free(request);
            unlink(clientFifoPath);
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    close(fdClient);

    char responseFromServer[BUF_SIZE];
    fdClient = open(clientFifoPath, O_RDONLY);
    if (fdClient == -1) {
        free(request);
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (read(fdClient, responseFromServer, sizeof(responseFromServer)) == -1) {
        free(request);
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Response from server: %s\n", responseFromServer);
    
    free(request);
    close(fdClient);
    exit(signum);
}

void mapUserInputToRequest(char command[BUF_SIZE], Request * request){    
    request->pid = getpid();
    request->argsCount = 0;

    if(strlen(command) == 0 || command == NULL) {strcpy(request->command, INVALID_COMMAND);}
    else{
    char * arg = strtok(command, " ");
    strcpy(request->command, arg);

    while(arg != NULL){
        arg = strtok(NULL, " ");
        if(arg == NULL) break;
        strcpy(request->args[request->argsCount++], arg);
    }
    if(strcmp(request->command, "help") == 0 && request->argsCount <= 1 ){}
    else if(strcmp(request->command, "list") == 0 && request->argsCount == 0){}
    else if(strcmp(request->command, "readF") == 0 && (request->argsCount == 2 || request->argsCount == 1)){}
    else if(strcmp(request->command, "writeT") == 0 && (request->argsCount == 3 || request->argsCount == 2)){}
    else if(strcmp(request->command, "upload") == 0 && request->argsCount == 1){}
    else if(strcmp(request->command, "download") == 0 && request->argsCount == 1){}
    else if(strcmp(request->command, "archServer") == 0 && request->argsCount == 1){}
    else if(strcmp(request->command, "killServer") == 0 && request->argsCount == 0){}
    else if(strcmp(request->command, "quit") == 0 && request->argsCount == 0){}
    else{strcpy(request->command, INVALID_COMMAND);}
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Invalid input! ./neHosClient <Connect/tryConnect> ServerPID\n");
        return 0;
    }

    int serverPid = atoi(argv[2]);
    int clientPid = getpid();
    char command[BUF_SIZE];
    strcpy(command, argv[1]);

    if(strcmp(command, "Connect") == 0 || strcmp(command, "tryConnect") == 0){
        printf(">> Waiting for Que.. ");
    }else{
        printf("Invalid input! ./neHosClient <Connect/tryConnect> ServerPID\n");
        return 0;
    }

    if(!isNumber(argv[2])){
        printf("Invalid input for server p_id! ./neHosClient <Connect/tryConnect> ServerPID\n");
    }
    // 
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    // set client-fifo name
    char clientFifoPath[BUF_SIZE];
    char tempClientFifoPath[BUF_SIZE] = CLIENT_FIFO_PATH_TEMPLATE;
    sprintf(clientFifoPath, tempClientFifoPath, clientPid);
    createFifo(clientFifoPath);

    // create special folder for upload-download
    char fdClientTemp[BUF_SIZE];
    char fdClientFolderName[BUF_SIZE*2];
    strcpy(fdClientTemp, "client");
    sprintf(fdClientFolderName, "%s_%d", fdClientTemp, clientPid);
    strcat(fdClientFolderName, "/");

    if (mkdir(fdClientFolderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
        if (errno != EEXIST){
            perror("Error during directory creation. Exiting...");
            unlink(clientFifoPath);
            return -1;
        }
    }

    // create special folder name for tar
    char archiveDir[BUF_SIZE];
    strcpy(archiveDir, fdClientFolderName);
    strcat(archiveDir, "archive/");


    // set server-fifo name
    char serverRequestPath[BUF_SIZE];
    char tempServerRequestPath[BUF_SIZE] = SERVER_FIFO_PATH_TEMPLATE;
    sprintf(serverRequestPath, tempServerRequestPath, serverPid);
    
    // create connection p*
    int fdServer;
    int fdClient;

    // first connection on server
    fdServer = open(serverRequestPath, O_WRONLY);
    if(fdServer < 0){
        printf("Server PID not found..\n");
        unlink(clientFifoPath);
        return 1;
    }
    
    request = (Request*)malloc(sizeof(Request));

    strcpy(request->command, command);
    request->pid = (int)getpid();

    write(fdServer, request, sizeof(Request));
    fdClient = open(clientFifoPath, O_RDONLY);
    
    char responseFromServer[BUF_SIZE];
    while(1){
        if(read(fdClient, responseFromServer, 256) == -1) {
            perror("read");
            close(fdClient);
            unlink(clientFifoPath);
            exit(EXIT_FAILURE);
        }
        close(fdClient);
        if(strcmp(responseFromServer, "CONNECTION_ESTABLISHED") == 0){
            printf("Connection established: \n");
            break;
        }else if(strcmp(command, "tryConnect") == 0 && strcmp(responseFromServer, "SERVER_IS_OUT_OF_CAPACITY") == 0){
            printf("server is out of capacity, closing....\n");
            close(fdClient);
            unlink(clientFifoPath);
            return 0;
        }
    }
    isConnected = 1;

    for(int i=0;i<BUF_SIZE;i++) responseFromServer[i]='\0';
    char userInput[BUF_SIZE];
    while(1){

        printf(">> ");
        fgets(userInput, BUF_SIZE, stdin);
        userInput[strcspn(userInput, "\n")] = '\0';

        mapUserInputToRequest(userInput, request);

        if(strcmp(request->command, INVALID_COMMAND) == 0){
            printf("%s\n", INVALID_COMMAND);
            continue;
        }

        // for tar utility
        if(strcmp(request->command, "archServer") == 0){
            int len = strlen(request->args[0]);
            const char *suffix = ".tar";

            if(len < strlen(suffix)){
                printf("%s not ended with  .tar\n", INVALID_COMMAND);
                continue;
            }
            if(strcmp(request->args[0]+len - strlen(suffix), suffix) != 0){
                printf("%s not ended with  .tar\n", INVALID_COMMAND);
                continue;
            }


            printf("Archiving the current contents of the server...\ncreating archive directory\n");
            if (mkdir(archiveDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
                if (errno != EEXIST){
                    perror("Error during directory creation. Exiting...");
                    unlink(clientFifoPath);
                    return -1;
                }
            }
        }

        fdClient = open(clientFifoPath, O_WRONLY);

        if(write(fdClient, request, sizeof(Request)) == -1){
            perror("write");
            unlink(clientFifoPath);
            exit(EXIT_FAILURE);
        }
        close(fdClient);

        
        fdClient = open(clientFifoPath, O_RDONLY);

        if(read(fdClient, responseFromServer, 256) == -1) {
            perror("read");
            unlink(clientFifoPath);
            exit(EXIT_FAILURE);
        }
        if(strcmp(responseFromServer, "quit") == 0) {
            close(fdClient);
            break;
        }
    
        // for tar utility
        if(strcmp(request->command, "archServer") == 0){
            printf("Calling tar utility..");
            printf("%s\n", responseFromServer);
            
            int forkId = fork();
            if(forkId == 0){
                char fileName[BUF_SIZE];
                strcpy(fileName, request->args[0]);

                free(request);
                request = (Request*)malloc(sizeof(Request));

                printf("child PID %d\n", getpid());

                if (chdir(archiveDir) == -1) {
                    perror("chdir");
                    exit(EXIT_FAILURE);
                }

                char *args[] = {"tar", "-cf", fileName, ".", NULL};
                free(request);

                if (execvp("tar", args) == -1) {
                    free(request);
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
                
                for(int i=0;i<BUF_SIZE;i++) responseFromServer[i]='\0';

                free(request);
                exit(EXIT_SUCCESS);
            }else if(forkId == -1){
                perror("fork");    
                unlink(clientFifoPath);
                exit(EXIT_FAILURE);
            }else{
                wait(NULL);
                char fileName[BUF_SIZE];
                strcpy(fileName, request->args[0]);

                // prepare custom request
                strcpy(request->command, "upload");
                strcpy(request->args[0], archiveDir);
                strcat(request->args[0], fileName);

                request->argsCount = 2;

                // copying local tar to server
                printf("copying the achive file..\n");
                fdClient = open(clientFifoPath, O_WRONLY);
                
                if(write(fdClient, request, sizeof(Request)) == -1){
                    perror("write");
                    unlink(clientFifoPath);
                    exit(EXIT_FAILURE);
                }
                close(fdClient);

                fdClient = open(clientFifoPath, O_RDONLY);

                if(read(fdClient, responseFromServer, 256) == -1) {
                    perror("read");
                    unlink(clientFifoPath);
                    exit(EXIT_FAILURE);
                }
                close(fdClient);
                // copy file client to
                char dest_path[BUF_SIZE*3];
                char *last_slash = strrchr(request->args[0], '/');
                if (last_slash == NULL) {
                    fprintf(stderr, "Error: No parent directory found for %s\n", request->args[0]);
                    exit(EXIT_FAILURE);
                }
                snprintf(dest_path, sizeof(dest_path), "%s/%s", fdClientFolderName, last_slash + 1);

                // Execute cp command to copy file
                char *args[] = {"cp", request->args[0], dest_path, NULL};
                int f = fork();
                if(f == 0){
                    free(request);
                    if (execvp("cp", args) == -1) {
                        perror("execvp");
                    }
                    exit(EXIT_FAILURE);
                }else{
                    wait(NULL);
                }
                
                // remove
                printf("removing achieve directory...\n"); 
                rmtree(archiveDir);
                
                printf("SUCCESS Server side files are achived in %s\n", responseFromServer);
            }
        }else{
            printf("%s\n", responseFromServer);
        }
        for(int i=0;i<BUF_SIZE;i++) responseFromServer[i]='\0';
        close(fdClient);
    }
    close(fdClient);
    close(fdServer);

    printf("Client server ended!..\n");
    free(request);
    return 0;
}