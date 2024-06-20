#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 512
#define MAX_THREADS 2000

typedef struct {
    char srcDir[MAX_FILENAME_LENGTH];
    char destDir[MAX_FILENAME_LENGTH];
} ThreadArgs;

typedef struct {
    char srcName[MAX_FILENAME_LENGTH];
    char destName[MAX_FILENAME_LENGTH];
    int srcFd;
    int destFd;
} FileData;

typedef struct {
    FileData *data; // file data buffer
    int in;
    int out;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t full;
} Buffer;

Buffer buffer;
pthread_t managerThread;
pthread_t workerThreads[MAX_THREADS];
pthread_barrier_t barrier;
int isCopyDone = 0;
int bufferSize;

// Statistics
int numFifoCopied = 0;
int numFilesCopied = 0;
int numDirsCreated = 0;
int numRegularFilesCopied = 0;
int numSymLinksCopied = 0;
double totalFileBytes = 0;
clock_t start_time, end_time;
pthread_mutex_t statsMutex;
pthread_mutex_t outputMutex;

void *manager(void *args);
void *worker(void *args);
void managerHelper(const char *srcDir, const char *destDir);
void produce(FileData data);
void copyFile(FileData data);
void handle_signal(int sig);
FileData consume();
void createDirectory(const char *destPath, int isCreated);


int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <buffer_size> <num_workers> <src_dir> <dest_dir>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bufferSize = atoi(argv[1]);
    int numWorkers = atoi(argv[2]);
    char srcDir[MAX_FILENAME_LENGTH];
    char destDir[MAX_FILENAME_LENGTH];

    if(numWorkers > MAX_THREADS){
        fprintf(stderr, "Number of workers must be less than %d\n", MAX_THREADS);
        exit(EXIT_FAILURE);

    }

    if(bufferSize <= 0 || numWorkers <= 0) {
        fprintf(stderr, "Buffer size and number of workers must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    strcpy(srcDir, argv[3]);
    strcpy(destDir, argv[4]);

    buffer.data = malloc(bufferSize * sizeof(FileData));
    if (buffer.data == NULL) {
        perror("Failed to allocate buffer");
        exit(EXIT_FAILURE);
    }
    buffer.in = 0;
    buffer.out = 0;
    buffer.count = 0;

    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.empty, NULL);
    pthread_cond_init(&buffer.full, NULL);
    pthread_mutex_init(&statsMutex, NULL);
    pthread_mutex_init(&outputMutex, NULL);
    pthread_barrier_init(&barrier, NULL, numWorkers + 1); // +1 for the manager thread

    signal(SIGINT, handle_signal);

    ThreadArgs *managerArgs = malloc(sizeof(ThreadArgs));
    if (managerArgs == NULL) {
        perror("Failed to allocate manager arguments");
        exit(EXIT_FAILURE);
    }
    
    strcpy(managerArgs->srcDir, srcDir);
    strcpy(managerArgs->destDir, destDir);

    // Create threads
    pthread_create(&managerThread, NULL, manager, (void *)managerArgs);
    for (int i = 0; i < numWorkers; i++) pthread_create(&workerThreads[i], NULL, worker, NULL);
    
    // Wait for threads
    pthread_join(managerThread, NULL);
    for (int i = 0; i < numWorkers; i++) pthread_join(workerThreads[i], NULL);
    
    double totalTime = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("All worker threads finished\n");
    int minute = totalTime / 60;
    int second = totalTime - (minute * 60);
    int millisecond = (totalTime - (minute * 60) - second) * 1000;
    // Print statistics
    printf("---------------------STATISTICS-----------------------\n");
    printf("Consumer: %d - Buffer Size: %d\n", numWorkers, bufferSize);
    printf("Number of Regular Files: %d\n", numRegularFilesCopied);
    printf("Number of FIFOS: %d\n", numFifoCopied);
    printf("Number of Directories: %d\n", numDirsCreated);
    printf("Number of Symbolic Links: %d\n", numSymLinksCopied);
    printf("Total bytes copied: %.0f\n", totalFileBytes);
    printf("Total copying time: %2d:%2d:%2d (min:sec:mili)\n", minute, second, millisecond);

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.empty);
    pthread_cond_destroy(&buffer.full);
    pthread_mutex_destroy(&statsMutex);
    pthread_mutex_destroy(&outputMutex);
    pthread_barrier_destroy(&barrier);
    free(buffer.data);

    return 0;
}

void managerHelper(const char *srcDir, const char *destDir) {
    DIR *dir = opendir(srcDir);

    if (dir == NULL) {
        perror("Error opening source directory");
        return;
    }

    DIR *dest = opendir(destDir);
    if (dest == NULL) {
        createDirectory(destDir, 1);
    } else {
        // Delete destination contents and recreate directory
        closedir(dest);
    }
    


    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char srcPath[MAX_FILENAME_LENGTH];
        char destPath[MAX_FILENAME_LENGTH];
        snprintf(srcPath, sizeof(srcPath), "%s/%s", srcDir, entry->d_name);
        snprintf(destPath, sizeof(destPath), "%s/%s", destDir, entry->d_name);

        struct stat statBuf;
        if (stat(srcPath, &statBuf) == -1) {
            perror("Error getting file status");
            continue;
        }

         if (S_ISDIR(statBuf.st_mode)) {
            createDirectory(destPath, 1); //
            managerHelper(srcPath, destPath);
        } else {
            FileData data = {0};

            if (S_ISFIFO(statBuf.st_mode)) {
                // FIFO specific handling
                strncpy(data.srcName, srcPath, MAX_FILENAME_LENGTH);
                strncpy(data.destName, destPath, MAX_FILENAME_LENGTH);
                data.srcFd = -1;
                data.destFd = -1;

                produce(data);

                pthread_mutex_lock(&statsMutex);
                numFilesCopied++;
                numFifoCopied++;
                pthread_mutex_unlock(&statsMutex);
            } else {
                // Handle regular files
                int srcFd = open(srcPath, O_RDONLY);
                if (srcFd == -1) {
                    perror("Error opening source file");
                    continue;
                }

                int destFd = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, statBuf.st_mode);
                if (destFd == -1) {
                    perror("Error opening destination file");
                    close(srcFd);
                    continue;
                }

                data.srcFd = srcFd;
                data.destFd = destFd;

                strncpy(data.srcName, srcPath, MAX_FILENAME_LENGTH);
                strncpy(data.destName, destPath, MAX_FILENAME_LENGTH);

                produce(data);

                pthread_mutex_lock(&statsMutex);
                numFilesCopied++;
                if (S_ISREG(statBuf.st_mode)) numRegularFilesCopied++;
                else if (S_ISLNK(statBuf.st_mode)) numSymLinksCopied++;
                pthread_mutex_unlock(&statsMutex);
            }
        }
    }
    closedir(dir);
}

void *manager(void *args) {
    start_time = clock();
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    managerHelper(threadArgs->srcDir, threadArgs->destDir); // Use manager helper because recursive operations can't be predictable as threads
    isCopyDone = 1;

    pthread_cond_broadcast(&buffer.empty);
    pthread_barrier_wait(&barrier); // Wait for all workers to finish copying files
    free(args);  // Free managerArgs

    end_time = clock();
    pthread_exit(NULL);
}

void *worker(void *args) {
    while (1) {
        FileData data = consume();

        if (strlen(data.srcName) == 0) break;
    
        copyFile(data);
        
        // Protect the printf statement with the output mutex
        pthread_mutex_lock(&outputMutex);
        printf("Copied %s to %s\n", data.srcName, data.destName);
        pthread_mutex_unlock(&outputMutex);
    }

    pthread_barrier_wait(&barrier); // Wait for all workers to finish copying files
}

void produce(FileData data) {
    pthread_mutex_lock(&buffer.mutex);

    while (buffer.count >= bufferSize) pthread_cond_wait(&buffer.full, &buffer.mutex);
    
    if(isCopyDone) {
        pthread_mutex_unlock(&buffer.mutex);
        return;
    }

    buffer.data[buffer.in] = data;
    buffer.in = (buffer.in + 1) % bufferSize;
    buffer.count++;

    pthread_cond_signal(&buffer.empty);
    pthread_mutex_unlock(&buffer.mutex);
}

FileData consume() {
    pthread_mutex_lock(&buffer.mutex);

    while (buffer.count <= 0 && !isCopyDone) pthread_cond_wait(&buffer.empty, &buffer.mutex);
    
    
    if (buffer.count <= 0 && isCopyDone) {
        pthread_mutex_unlock(&buffer.mutex);
        FileData data = {"", ""};
        return data;
    }

    FileData data = buffer.data[buffer.out];
    buffer.out = (buffer.out + 1) % bufferSize;
    buffer.count--;

    pthread_cond_signal(&buffer.full);
    pthread_mutex_unlock(&buffer.mutex);

    return data;
}

void copyFile(FileData data) {
    struct stat statBuf;
    if (stat(data.srcName, &statBuf) == -1) {
        perror("Error getting file status");
        close(data.srcFd);
        close(data.destFd);
    }
   
    if ( S_ISFIFO(statBuf.st_mode) || (data.srcFd == -1 && data.destFd == -1)) {
        close(data.srcFd);
        close(data.destFd);

        if (unlink(data.destName) == -1 && errno != ENOENT) {
            perror("Error deleting existing destination file");
            return;
        }


        if (mkfifo(data.destName, statBuf.st_mode) == -1) {
            perror("Error creating FIFO at destination");
            return;
        }

        int srcFd = open(data.srcName, O_RDONLY | O_NONBLOCK);
        if (srcFd == -1) {
            perror("Error opening source FIFO");
            return;
        }

        int destFd = open(data.destName, O_WRONLY | O_NONBLOCK);
        if (destFd == -1) {
            perror("Error opening destination FIFO");
            close(srcFd);
            return;
        }

        char buf[4096];
        ssize_t bytes;

        while ((bytes = read(srcFd, buf, sizeof(buf))) > 0) {
            if (write(destFd, buf, bytes) != bytes) {
                perror("Error writing to destination FIFO");
                break;
            }
            pthread_mutex_lock(&statsMutex);
            totalFileBytes += bytes;
            pthread_mutex_unlock(&statsMutex);
        }

        if (bytes == -1 && errno != EAGAIN) {
            perror("Error reading from source FIFO");
        }

        close(srcFd);
        close(destFd);
        return;
    }

    char buf[4096];
    ssize_t bytes;

    while ((bytes = read(data.srcFd, buf, sizeof(buf))) > 0) {
        if (write(data.destFd, buf, bytes) != bytes) {
            perror("Error writing to destination file");
            break;
        }
        pthread_mutex_lock(&statsMutex);
        totalFileBytes += bytes;
        pthread_mutex_unlock(&statsMutex);
    }

    if (bytes == -1) perror("Error reading from source file");

    close(data.srcFd);
    close(data.destFd);
}

void handle_signal(int sig) {
    isCopyDone = 1;
    pthread_cond_broadcast(&buffer.empty);
    printf("Termination signal received. Cleaning up...\n");
}

void createDirectory(const char *destPath, int isCreated) {
    if (mkdir(destPath, 0777) == -1 && errno != EEXIST) {
        perror("Error creating directory");
    } else {
        if(isCreated == 1){
            pthread_mutex_lock(&statsMutex);
            numDirsCreated++;
            pthread_mutex_unlock(&statsMutex);
        }
       
    }
}