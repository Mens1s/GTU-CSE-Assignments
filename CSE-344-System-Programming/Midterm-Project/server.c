#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/file.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "include/common.h"

int clientCount=0;
int clientPidList[BUF_SIZE];
char serverRequestPath[BUF_SIZE];

sem_t *semControlClientConnection;
ClientList *sharedList;

void sigint_handler(int signum);

void sendResponse(Request request, char *message) {
    char clientFifoPath[256];
    sprintf(clientFifoPath, CLIENT_FIFO_PATH_TEMPLATE, request.pid);

    int fdClient = open(clientFifoPath, O_WRONLY);
    if (fdClient == -1) {
        perror("openrer");
        exit(EXIT_FAILURE);
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    // write lock
    if (fcntl(fdClient, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Write the messages to client
    if (write(fdClient, message, strlen(message) + 1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // unlock
    lock.l_type = F_UNLCK;
    if (fcntl(fdClient, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    close(fdClient);
}

void logger(Request request, char *fdServerPath, char *message) {
    char fdServerFilePath[256];
    strcpy(fdServerFilePath, fdServerPath);
    strcat(fdServerFilePath, "/");
    strcat(fdServerFilePath, "log.txt");

    int fd = open(fdServerFilePath, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        sendResponse(request, FILE_NOT_FOUND);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        close(fd);
        return;
    }

    char logMessage[512];
    snprintf(logMessage, sizeof(logMessage), "PID-%d %s\n", request.pid, message);
    if (write(fd, logMessage, strlen(logMessage)) == -1) {
        perror("write");
        close(fd);
        return;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        close(fd);
        return;
    }

    close(fd);
}

void loggerInServer(char *fdServerPath, char *message) {
    char fdServerFilePath[256];
    strcpy(fdServerFilePath, fdServerPath);
    strcat(fdServerFilePath, "/");
    strcat(fdServerFilePath, "log.txt");

    int fd = open(fdServerFilePath, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("open");
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    // Acquire the write lock
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        close(fd);
        return;
    }

    // Write the log message to the file
    char logMessage[512];
    snprintf(logMessage, sizeof(logMessage), "SERVER %s\n", message);
    if (write(fd, logMessage, strlen(logMessage)) == -1) {
        perror("write");
        close(fd);
        return;
    }

    // Release the lock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        close(fd);
        return;
    }

    close(fd);
}

void helpWithoutArgument(Request request, char *fdServerPath, char *response){
    logger(request, fdServerPath, "helpWithoutArgument command worked.");
    sendResponse(request, response);
}

void helpWithArgument(Request request, char *fdServerPath){
    char response[BUF_SIZE];

    if (strcmp("help", request.args[0]) == 0) 
        strcpy(response, "\nhelp <command:optional>\n\tdisplay the list of possible client requests\n\n");
    else if (strcmp("list", request.args[0]) == 0) 
        strcpy(response, "\nlist\n\tsends a request to display the list of files in Servers directory\n\t(also displays the list received from the Server)\n\n");
    else if (strcmp("readF", request.args[0]) == 0) 
        strcpy(response, "\nreadF <file> <line #>\n\trequests to display the # line of the <file>, if no line number is given\n\tthe whole contents of the file is requested (and displayed on the client side)\n\n");
    else if (strcmp("writeT", request.args[0]) == 0) 
        strcpy(response, "\nwriteT <file> <line #> <string>\n\trequest to write the content of “string” to the #th line the <file>, if the line # is not given\n\twrites to the end of file. If the file does not exists in Servers directory creates and edits the\n\tfile at the same time\n\n");
    else if (strcmp("upload", request.args[0]) == 0) 
        strcpy(response, "\nupload <file>\n\tuploads the file from the current working directory of client to the Servers directory\n\n");
    else if (strcmp("download", request.args[0]) == 0) 
        strcpy(response, "\n\tdownload <file>\n\t\trequest to receive <file> from Servers directory to client side\n\n");
    else if (strcmp("killServer", request.args[0]) == 0) 
        strcpy(response, "\nkillServer\n\tSends a kill request to the Server\n\n");
    else if (strcmp("archServer", request.args[0]) == 0) 
        strcpy(response, "\narchServer <fileName>.tar\n\tUsing fork, exec and tar utilities create a child process that will collect all the files currently \n\tavailable on the the Server side and store them in the <filename>.tar archive\n\n");
    else if (strcmp("quit", request.args[0]) == 0)
        strcpy(response, "\nquit\n\tSend write request to Server side log file and quits\n\n");
    else
        strcpy(response, INVALID_COMMAND);

    logger(request, fdServerPath, "helpWithArgument command worked.");
    sendResponse(request, response);
}

void listServerDir(Request request, char fdServerPath[BUF_SIZE], char *response){
    char ls[BUF_SIZE];
    char command[128] = "ls -a ";

    strcat(command, fdServerPath);

    FILE *ls_output = popen(command, "r");
    if (ls_output == NULL) {
        perror("Error-LS");
        exit(EXIT_FAILURE);
    }

    while (fgets(ls, BUF_SIZE, ls_output) != NULL) strcat(response, ls);
    
    pclose(ls_output);
    logger(request, fdServerPath, "listServerDir command worked.");
    sendResponse(request, response);
}

void readFull(Request request, char *fdServerPath, char *clientFifoPath, char *response){
    // no need sem
    char fileName[256];
    int fd;

    char fdServerFilePath[256];
    strcpy(fdServerFilePath, fdServerPath);
    strcat(fdServerFilePath, "/");
    strcat(fdServerFilePath, request.args[0]);

    int pid = fork();
    if (pid == 0) {
        fd = open(clientFifoPath, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        // Acquire the write lock
        if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror("fcntl");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Redirect stdout to the client FIFO
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd);

        // Release the write lock
        lock.l_type = F_UNLCK;

        if (fcntl(STDOUT_FILENO, F_SETLK, &lock) == -1) {
            perror("fcntl unlock");
        }

        logger(request, fdServerPath, "readFull command tries to work..");
        fflush(stdout);
        // Execute the cat command not print all control... TODO
        execvp("cat", (char *[]){"cat", fdServerFilePath, NULL});

        // If execvp returns, an error occurred
        logger(request, fdServerPath, "readFull command failed.");
        sendResponse(request, "NOT_VALID_FILE_NAME");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}   

void readPartial(Request request, char *fdServerPath, char *clientFifoPath, char *response) {
    int targetLine = atoi(request.args[1]);
    if(targetLine < 1){
        strcpy(response, FILE_LINE_COUNT_MATCH_ERROR);
        logger(request, fdServerPath, "readPartial command worked.");
        sendResponse(request, response);
    }
    
    char fdServerFilePath[256];
    strcpy(fdServerFilePath, fdServerPath);
    strcat(fdServerFilePath, "/");
    strcat(fdServerFilePath, request.args[0]);

    int fd = open(fdServerFilePath, O_RDWR);
    if (fd == -1) {
        logger(request, fdServerPath, "readPartial command notWorked. ErrCode: FILE_NOT_FOUND");
        sendResponse(request, FILE_NOT_FOUND);
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    // Acquire the read lock
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        logger(request, fdServerPath, "readPartial command notWorked. ErrCode: fcntl");
        perror("fcntl");
        close(fd);
        return;
    }
 
    FILE *fp = fdopen(fd, "r");
    if (fp == NULL) {
        logger(request, fdServerPath, "readPartial command notWorked. ErrCode: FILE_NOT_FOUND");
        sendResponse(request, FILE_NOT_FOUND);
        close(fd);
        return;
    }

    char *target = NULL;
    size_t len = 0;
    ssize_t charLen;
    while ((charLen = getline(&target, &len, fp)) != -1 && targetLine >= 1) {
        if (targetLine == 1) {
            strcpy(response, target);
            logger(request, fdServerPath, "readPartial command worked.");
            sendResponse(request, response);
        }
        targetLine--;
    }
    if (targetLine >= 1) {
        strcpy(response, FILE_LINE_COUNT_MATCH_ERROR);
        logger(request, fdServerPath, "readPartial command worked.");
        sendResponse(request, response);
    }

    if (target) free(target);

    // Release the read lock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        logger(request, fdServerPath, "readPartial command notWorked. ErrCode: fcntl");
        perror("fcntl");
    }

    fclose(fp);
    
}

void writeBasedLocation(Request request, char *fdServerPath, char *clientFifoPath, char *response) {
    int write_line = -1;
    char newline[BUF_SIZE];

    if (request.argsCount == 3) {
        write_line = atoi(request.args[1]);
        strcpy(newline, request.args[2]);
    } else {
        strcpy(newline, request.args[1]);
    }

    char fdServerFilePath[256];
    strcpy(fdServerFilePath, fdServerPath);
    strcat(fdServerFilePath, "/");
    strcat(fdServerFilePath, request.args[0]);

    char temp_filename[BUF_SIZE*2];
    char temp_filenameBuf[BUF_SIZE];
    strcpy(temp_filenameBuf, fdServerPath);
    strcat(temp_filenameBuf, "/temp____");
    snprintf(temp_filename, sizeof(temp_filename), "%s_%d\n", temp_filenameBuf, request.pid);
    strcat(temp_filename, request.args[0]);

    int fd = open(fdServerFilePath, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: open");
        perror("open");
        return;
    }

    int fdTemp = open(temp_filename, O_RDWR | O_CREAT, 0666);
    if (fdTemp == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: open");
        perror("open");
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;


    struct flock lockTemp;
    memset(&lockTemp, 0, sizeof(lock));
    lockTemp.l_type = F_WRLCK;

    // Acquire the write lock
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fcntl");
        perror("fcntl");
        close(fd);
        close(fdTemp);
        return;
    }

    if (fcntl(fdTemp, F_SETLKW, &lockTemp) == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fcntl");
        perror("fcntl");
        close(fd);
        close(fdTemp);
        return;
    }

    FILE *file = fdopen(fd, "r+");
    if (file == NULL) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fopen");
        perror("fdopen");
        close(fd);
        return;
    }

    FILE *temp = fdopen(fdTemp, "r+");
    if (temp == NULL) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fopen");
        perror("fopen");
        fclose(file);
        close(fd);
        return;
    }

    char buffer[BUF_SIZE];
    bool keep_reading = true;
    int current_line = 1;
    bool isWroted = false;

    while (keep_reading) {
        if (fgets(buffer, BUF_SIZE, file) == NULL) {
            if (feof(file)) {
                if (write_line == -1) {
                    fputs(newline, temp);
                    fputs("\n", temp);
                    isWroted = true;
                }
                keep_reading = false;
            } else {
                logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fgets");
                perror("fgets");
                break;
            }
        } else {
            if (current_line == write_line) {
                fputs(newline, temp);
                fputs("\n", temp);
                isWroted = true;
            }
            fputs(buffer, temp);
            current_line++;
        }
        for(int i = 0; i < BUF_SIZE; i++) buffer[i]='\0';
    }
    // set curson to first location
    rewind(file);
    rewind(temp);

    keep_reading = true;
    current_line = 1;

    while (keep_reading) {
        if (fgets(buffer, BUF_SIZE, temp) == NULL) {
            if (feof(temp)) {
                keep_reading = false;
                if(!isWroted){
                    fputs(newline, file);
                    fputs("\n", file);
                }
            } else {
                logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fgets");
                perror("fgets");
                break;
            }
        } else {
            fputs(buffer, file);
            current_line++;
        }
        for(int i = 0; i < BUF_SIZE; i++) buffer[i]='\0';
    }
   
    // Release the write lock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fcntl");
        perror("fcntl");
    }

    // Release the write lock
    lockTemp.l_type = F_UNLCK;
    if (fcntl(fdTemp, F_SETLKW, &lockTemp) == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: fcntl");
        perror("fcntl");
    }

    fclose(temp);
    fclose(file);
    close(fd);
    close(fdTemp);

    if (unlink(temp_filename) == -1) {
        logger(request, fdServerPath, "writeBasedLocation command notWorked. ErrCode: unlink");
        perror("unlink");
    }

    logger(request, fdServerPath, "writeBasedLocation command worked.");

    strcpy(response, "Write operation done.\n");
    sendResponse(request, response);
}

void xloadFile(Request request, char *fdServerPath, int type, char *response){
    char fileName[BUF_SIZE];
    char fileSourcePath[BUF_SIZE];
    char fdClientTemp[BUF_SIZE];
    char fileDestinationPath[BUF_SIZE*2];
    
    strcpy(fileName, request.args[0]);
    strcpy(fdClientTemp, "client");
    sprintf(fileDestinationPath, "%s_%d", fdClientTemp, request.pid);
    strcat(fileDestinationPath, "/");
    strcat(fileDestinationPath, fileName);
    
    if(type == 2){
        strcpy(fileDestinationPath, fileName);

        strcpy(fileSourcePath, fdServerPath);
        strcat(fileSourcePath, "/");
        // clear file name until clearHere/takeHere
        char neededPart[BUF_SIZE];

        for(int i = strlen(fileName) - 1; i >= 0; i--){
            if(fileName[i] == '/') {
                strcpy(neededPart, &fileName[i+1]);
                break;
            }
        }
        strcat(fileSourcePath, neededPart);
    }else{
        strcpy(fileSourcePath, fdServerPath);
        strcat(fileSourcePath, "/");
        strcat(fileSourcePath, fileName);
    }

    int xloadSize;

    if(type == 0) // download
        xloadSize = transferFile(fileDestinationPath, fileSourcePath, response);
    else if(type == 1)
        xloadSize = transferFile(fileSourcePath, fileDestinationPath, response);
    else    
        xloadSize = transferFile(fileSourcePath, fileDestinationPath, response);

    if(xloadSize >= 0 && type == 0) sprintf(response, "File downloaded succesfully.\n %d bytes transfered", xloadSize);
    if(xloadSize >= 0 && type == 1) sprintf(response, "File uploaded succesfully.\n %d bytes transfered", xloadSize);

    logger(request, fdServerPath, "xLoadFile command worked.");
    sendResponse(request, response);
}

void archServer(Request request, char *fdServerPath, int type, char *response){
    char fdClientTemp[BUF_SIZE];
    char fdClientFolderName[BUF_SIZE*2];
    char archiveDir[BUF_SIZE];
    char fileSourcePath[BUF_SIZE];
    char tempSourceFile[BUF_SIZE];
    char tempDestinationFile[BUF_SIZE];

    int xloadSize = 0;
    int folderCount = 0;

    // server
    strcpy(fileSourcePath, fdServerPath);
    strcat(fileSourcePath, "/");

    // client
    strcpy(fdClientTemp, "client");
    sprintf(fdClientFolderName, "%s_%d", fdClientTemp, request.pid);
    strcat(fdClientFolderName, "/");
    strcpy(archiveDir, fdClientFolderName);
    strcat(archiveDir, "archive/");

    // download all files in server to there
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    dir = opendir(fileSourcePath);
    if (dir == NULL) {
        logger(request, fdServerPath, "archServer command not worked.ErrCode : opendir");
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Process each file
        strcpy(tempSourceFile, fileSourcePath);
        strcat(tempSourceFile, entry->d_name);

        strcpy(tempDestinationFile, archiveDir);
        strcat(tempDestinationFile, entry->d_name);

        xloadSize += transferFile(tempDestinationFile, tempSourceFile, response);
        folderCount++;
    }

    // Close the directory
    closedir(dir);
    sprintf(response, "%d File downloaded succesfully.\n %d bytes transfered", folderCount, xloadSize);

    logger(request, fdServerPath, "archServer command worked.");
    sendResponse(request, response);
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Invalid input! ./neHosServer <dirname> <max. #ofClients>\n");
        return 0;
    }

    int serverPid = getpid();
    char fdServerName[BUF_SIZE];
    strcpy(fdServerName, argv[1]);

    char dir[256];
    strcpy(dir, fdServerName);
    strcat(fdServerName, "/");
    strcat(dir, "/");

    if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
        if (errno != EEXIST){
            perror("Error during directory c reation. Exiting...");                   
            return -1;
        }
    }

    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) == -1){
        perror("Error setting SIGINT handler");
        return 0;
    }

    if(!isNumber(argv[2])){
        printf("Invalid input for #ofClients! ./neHosServer <dirname> <max. #ofClients>\\n");
    }

    int maxClientNumber = atoi(argv[2]);
    if(maxClientNumber <= 0){
        printf("No valid #ofClients\n");
        return 0;
    }

    // share semephores
    semControlClientConnection = (sem_t *) mmap( NULL, sizeof(sem_t),
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS,
                            -1, 0);
    sem_init(semControlClientConnection, 1, maxClientNumber);

    int shmid;
    if ((shmid = shmget(IPC_PRIVATE, maxClientNumber * sizeof(ClientList), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if ((sharedList = shmat(shmid, NULL, 0)) == (ClientList *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < maxClientNumber; i++) {
        sharedList[i].pid = 0;
        sharedList[i].status = 0;
    }

    // set server-fifo name
    char tempServerRequestPath[BUF_SIZE] = SERVER_FIFO_PATH_TEMPLATE;
    sprintf(serverRequestPath, tempServerRequestPath, serverPid);
    createFifo(serverRequestPath);

    // request params
    int fdServer;
    int fdClient;
    int forkPid;
    int serverResponseLen;
    Request request;
    char response[BUF_SIZE];
    char serverLogMessage[BUF_SIZE];

    // get requests
    sprintf(serverLogMessage, "Server Started PID %d...\nWaiting for clients...\n", serverPid);
    printf("%s", serverLogMessage);
    loggerInServer(argv[1], serverLogMessage);

    fdServer = open(serverRequestPath, O_RDONLY);
    if(fdServer == -1){
        perror("openFifoError");
        sem_destroy(semControlClientConnection);
        return EXIT_FAILURE;
    }
    while(1){
        // get first connection request from client
        serverResponseLen = read(fdServer, &request, sizeof(Request));
        if(serverResponseLen == -1){
            perror("errorReadingFirstRequests-cont");
            continue;
        }
        if(serverResponseLen == 0) continue;
        // set client-fifo name
        char clientFifoPath[BUF_SIZE];
        char tempClientFifoPath[BUF_SIZE] = CLIENT_FIFO_PATH_TEMPLATE;
        sprintf(clientFifoPath, tempClientFifoPath, request.pid);

        int clientCounterSem;
        if (sem_getvalue(semControlClientConnection, &clientCounterSem) != 0) 
            exit(EXIT_FAILURE);

        if (clientCounterSem == 0 && strcmp(request.command, "Connect") == 0){
            sprintf(serverLogMessage, "Connection request PID %d... Que FULL\n", request.pid);
            printf("%s", serverLogMessage);
            loggerInServer(argv[1], serverLogMessage);
        }
        else if (strcmp(request.command, "tryConnect") == 0 && clientCounterSem == 0){
            sprintf(clientFifoPath, CLIENT_FIFO_PATH_TEMPLATE, request.pid);
            sendResponse(request, "SERVER_IS_OUT_OF_CAPACITY");
            sprintf(serverLogMessage, "Try Connect request PID %d... Que FULL... exiting\n", request.pid);
            printf("%s", serverLogMessage);
            loggerInServer(argv[1], serverLogMessage);
            continue;
        }

        clientPidList[clientCount++] = request.pid;

        forkPid = fork();
        if(forkPid == 0){
            sharedList[clientCount].pid = request.pid;
            sharedList[clientCount].status = 1;

            sem_wait(semControlClientConnection);
            
            sprintf(clientFifoPath, CLIENT_FIFO_PATH_TEMPLATE, request.pid);
            // checking client is waiting or ctrl-c
            if (access(clientFifoPath, F_OK) == -1) {
                sharedList[clientCount].status = 0;
                sem_post(semControlClientConnection);
                exit(EXIT_SUCCESS);
            }
            sendResponse(request, "CONNECTION_ESTABLISHED");

            int clientEnterOrder = clientCount;
            sprintf(serverLogMessage, "Client PID %d connected as \"client%d\"\n", request.pid, clientCount);
            loggerInServer(argv[1], serverLogMessage);

            printf("%s",serverLogMessage);
            // get requests of client 
            while(1){
                // clear response
                for(int i=0;i<BUF_SIZE;i++)response[i]='\0';

                sprintf(clientFifoPath, CLIENT_FIFO_PATH_TEMPLATE, request.pid);
                fdClient = open(clientFifoPath, O_RDONLY);
                if(read(fdClient, &request, sizeof(Request)) == -1){
                    perror("read");
                    unlink(clientFifoPath);
                    sharedList[clientCount].status = 0;
                    exit(EXIT_FAILURE);
                }
                close(fdClient);
                
                if(strcmp(request.command, "quit") == 0){
                    strcpy(response, "quit");
                    sendResponse(request, response);
                    unlink(clientFifoPath);
                    sprintf(serverLogMessage, "Client PID %d quited\n", request.pid);
                    loggerInServer(argv[1], serverLogMessage);
                    break;
                }
                else if (strcmp(request.command, "help") == 0) {                    
                    if(request.argsCount == 0){
                        strcpy(response, "\nAvailable commands are :\n\nhelp, list, readF, writeT, upload, download, archServer, killServer, quit\n\n");
                        helpWithoutArgument(request, argv[1], response);
                    }
                    else if(request.argsCount == 1){
                        helpWithArgument(request, argv[1]);
                    }
                    else{
                        printf("%s\n",INVALID_COMMAND); // handle case
                    }
                }
                else if(strcmp(request.command, "list") == 0){
                    listServerDir(request, argv[1], response);
                }
                else if(strcmp(request.command, "readF") == 0){                    
                    if(request.argsCount == 1){
                        readFull(request, argv[1], clientFifoPath, response);
                    }
                    else if(request.argsCount == 2){
                        readPartial(request, argv[1], clientFifoPath, response);
                    }
                    else{
                        printf("%s\n",INVALID_COMMAND); // handle case
                    }
                }
                else if(strcmp(request.command, "writeT") == 0){                
                    writeBasedLocation(request, argv[1], clientFifoPath, response);
                    
                    if( !(request.argsCount == 2 ||request.argsCount == 3) ){
                        printf("%s\n",INVALID_COMMAND); // handle case
                    }
                }
                else if(strcmp(request.command, "upload") == 0){
                    if(request.argsCount == 1)
                        xloadFile(request, argv[1], 1, response);
                    else
                        xloadFile(request, argv[1], 2, response);
                }
                else if(strcmp(request.command, "download") == 0){
                    xloadFile(request, argv[1], 0, response);
                }
                else if(strcmp(request.command, "archServer") == 0){
                    archServer(request, argv[1], 0, response);
                }
                else if(strcmp(request.command, "killServer") == 0){
                    unlink(serverRequestPath);
                    kill(serverPid, SIGINT);
                }
                else{
                    printf("%s\n",INVALID_COMMAND);
                }
            }
            printf("client%d disconnected...\n", clientEnterOrder);
            sharedList[clientCount].status = 0;
            sem_post(semControlClientConnection);
            _exit(EXIT_SUCCESS);
        }else{
            //parent
        }
    }   
    sem_destroy(semControlClientConnection);
    shmdt(sharedList);
    unlink(serverRequestPath);
}

void sigint_handler(int signum) {
    printf("Exiting from server (signum): (%d)\n", signum);

    for(int i = 1; i <= clientCount; i++){
        if(sharedList[i].status == 1)
            kill(sharedList[i].pid, SIGKILL);
    }
    for (int i = 0; i < getdtablesize(); i++) close(i);
    
    unlink(serverRequestPath);
    shmdt(sharedList);
    exit(signum);
}
