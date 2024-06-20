#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>

#define BUF_SIZE 400
#define INVALID_COMMAND "INVALID_COMMAND"
#define XLOAD_FAILURE "XLOAD_FAILURE"
#define DOWNLOAD_COMPLETE "DOWNLOAD_COMPLETE"
#define FILE_CANNOT_LOCK "FILE_CANNOT_LOCK"
#define FILE_NOT_FOUND "FILE_NOT_FOUND"
#define FILE_CANNOT_CREATED "FILE_CANNOT_CREATED"
#define FILE_LINE_COUNT_MATCH_ERROR "FILE_LINE_COUNT_MATCH_ERROR"
#define CLIENT_FIFO_PATH_TEMPLATE "/tmp/fifo_%d"
#define SERVER_FIFO_PATH_TEMPLATE "/tmp/biboServer_%d"
#define SERVER_FOLDER_PATH_TEMPLATE "/tmp/biboServer_%d"
#define CLIENT_FOLDER_PATH_TEMPLATE "/tmp/biboServer_%d"

// command <arg1> <arg2> <arg3>
typedef struct Request {
    int pid;
    char command[BUF_SIZE];
    char args[3][BUF_SIZE];
    int argsCount;
} Request;

typedef struct ClientList {
    int pid;
    int status; // 0-dead 1-live
} ClientList;

void createFifo(char *fifoLocation){

    if (mkfifo(fifoLocation, 0666) < 0) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    
}

int isNumber(const char* str){
    if(*str == '\0') return 0;
    // checking negative or positive number
    if(*str == '+' || *str == '-') str++;

    while(*str != '\0'){
        if(!isdigit(*str)) return 0;
        str++;
    }

    return 1;
}

int transferFile(char *fileDestinationPath, char *fileSourcePath, char *response) {
    int fdSourceFile;
    int fdDestinationFile;
    int transferSize = 0;
    int partialTransferSize;
    int isNameChanged = 0;

    fdSourceFile = open(fileSourcePath, O_RDWR);
    if (fdSourceFile == -1) {
        perror(FILE_NOT_FOUND);
        strcpy(response, FILE_NOT_FOUND);
        return -1;
    }

    struct flock sourceLock;
    memset(&sourceLock, 0, sizeof(sourceLock));
    sourceLock.l_type = F_WRLCK;
    sourceLock.l_whence = SEEK_SET;
    sourceLock.l_start = 0;
    sourceLock.l_len = 0;

    // Acquire the read lock on the source file
    if (fcntl(fdSourceFile, F_SETLKW, &sourceLock) == -1) {
        perror(FILE_CANNOT_LOCK);
        strcpy(response, FILE_CANNOT_LOCK);
        close(fdSourceFile);
        return -1;
    }

    // Control of destination path including same name file
    int counter = 1;
    while (access(fileDestinationPath, F_OK) == 0) {
        char renameHelperFirstPart[BUF_SIZE / 2];
        char renameHelperSecondPart[BUF_SIZE / 4];
        char fileDestinationPathCopy[BUF_SIZE];
        strcpy(fileDestinationPathCopy, fileDestinationPath);

        for (int idx = strlen(fileDestinationPathCopy) - 1; idx >= 0; idx--) {
            if (fileDestinationPathCopy[idx] == '.') {
                strncpy(renameHelperFirstPart, fileDestinationPath, idx);
                renameHelperFirstPart[idx] = '\0';

                strcpy(renameHelperSecondPart, fileDestinationPathCopy + idx);
                renameHelperSecondPart[strlen(fileDestinationPathCopy) - idx] = '\0';

                sprintf(fileDestinationPathCopy, "%s(%d)%s", renameHelperFirstPart, counter, renameHelperSecondPart);
                isNameChanged = 1;
                break;
            }
        }
        if (access(fileDestinationPathCopy, F_OK) != 0)
            strcpy(fileDestinationPath, fileDestinationPathCopy);

        counter++;
    }

    fdDestinationFile = open(fileDestinationPath, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fdDestinationFile == -1) {
        perror("Error creating destination file");
        strcpy(response, "Error: Destination file already exists or cannot be created");
        close(fdSourceFile);
        return -1;
    }

    struct flock destinationLock;
    memset(&destinationLock, 0, sizeof(destinationLock));
    destinationLock.l_type = F_WRLCK;
    destinationLock.l_whence = SEEK_SET;
    destinationLock.l_start = 0;
    destinationLock.l_len = 0;

    // Acquire the write lock on the destination file
    if (fcntl(fdDestinationFile, F_SETLKW, &destinationLock) == -1) {
        perror("Error locking destination file");
        strcpy(response, FILE_CANNOT_LOCK);
        close(fdSourceFile);
        close(fdDestinationFile);
        unlink(fileDestinationPath); // Remove the created file
        return -1;
    }

    // transfer to source to destination
    char line[BUF_SIZE];
    while ((partialTransferSize = read(fdSourceFile, line, BUF_SIZE)) > 0) {
        if (write(fdDestinationFile, line, partialTransferSize) == -1) {
            perror("Error writing to destination file");
            strcpy(response, XLOAD_FAILURE);
            break;
        }
        transferSize += partialTransferSize;
    }

    // Release the write lock on the destination file
    destinationLock.l_type = F_UNLCK;
    if (fcntl(fdDestinationFile, F_SETLKW, &destinationLock) == -1) {
        perror("Error unlocking destination file");
    }

    // Release the read lock on the source file
    sourceLock.l_type = F_UNLCK;
    if (fcntl(fdSourceFile, F_SETLKW, &sourceLock) == -1) {
        perror("Error unlocking source file");
    }

    close(fdDestinationFile);
    close(fdSourceFile);

    if (partialTransferSize == -1) {
        strcpy(response, XLOAD_FAILURE);
        unlink(fileDestinationPath); // Remove the partially transferred file
        return -1;
    }

    if(isNameChanged == 1) strcpy(response, fileDestinationPath);
    
    return transferSize;
}

void rmtree(const char path[]){
    size_t pathLen;
    char *fullPath;
    DIR *dir;
    struct stat statPath, statEntry;
    struct dirent *entry;

    // stat for the path
    stat(path, &statPath);

    // isExists
    if (S_ISDIR(statPath.st_mode) == 0) {
        fprintf(stderr, "%s: %s\n", "Is not directory", path);
        exit(-1);
    }

    // auth check
    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
        exit(-1);
    }

    pathLen = strlen(path);

    // iteration through entries in the directory - soverflow/linuxmanpage
    while ((entry = readdir(dir)) != NULL) {

        // skip entries "." and ".."
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // determinate a full path of an entry
        fullPath = calloc(pathLen + 1 + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(fullPath, path);
        strcat(fullPath, "/");
        strcat(fullPath, entry->d_name);

        // stat for the entry
        stat(fullPath, &statEntry);

        // recursively remove a nested directory
        if (S_ISDIR(statEntry.st_mode) != 0) {
            rmtree(fullPath);
            free(fullPath);
            continue;
        }

        // remove a file object
        if (unlink(fullPath) != 0)
            printf("Can't remove a file:\t%s\n", fullPath);

        free(fullPath);
    }

    // remove 
    if (rmdir(path) != 0)
        printf("Can't remove a directory:\t%s\n", path);

    closedir(dir);
}
