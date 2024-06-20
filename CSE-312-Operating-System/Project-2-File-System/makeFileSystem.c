#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILE_SYSTEM_SIZE (4 * 1024 * 1024) // 4 MB
#define BLOCK_SIZE_512 512
#define BLOCK_SIZE_1024 1024
#define MAX_FILE_COUNT 1024
#define MAX_DIRECTORY_COUNT 64
#define MAX_DIRECTORY_NAME 32
char ROOT_DEFAULT_PATH = '/'; // "
char ROOT_DEFAULT_PATHSTR[1] = "/"; // "

// LAYER OF DB
// ..................................................................................................
// . SUPERBLOCK   | FAT | MAX_FILE_COUNT * DIRECTORY_ENTRY | MAX_DIRECTORY_COUNT * DIRECTORY | Data .
// ..................................................................................................

typedef struct {
    int size;
    int dataSize;
    int startBlock;
    int entryOrder;
    char permissions[2]; // "RW" == 00 
    time_t creation_time;
    time_t modification_time;
    int isPasswordProtected;
    char parentDirectory[MAX_DIRECTORY_NAME];
    char password[32];
} DirectoryEntry;

typedef struct {
    char name[MAX_DIRECTORY_NAME];
    int size;
    int startBlock;
    int dirOrder;
    time_t creation_time;
    time_t modification_time;
} Directory;

typedef struct {
    int block_size;
    int total_blocks;
    int free_blocks;
    int fat_start;
    int entry_start;
    int total_entries;
    int directory_start;
    int total_directories;
    int data_start;
} SuperBlock;

void writeToBlock(FILE* fs, int block_size, int block_number, void* data) {
    fseek(fs, block_number * block_size, SEEK_SET);
    fwrite(data, block_size, 1, fs);
}

void createFileSystem(const char* filename, int block_size) {
    FILE* fs = fopen(filename, "wb");
    if (!fs) {
        perror("Unable to create file");
        exit(EXIT_FAILURE);
    }

    SuperBlock sb;
    sb.block_size = block_size; // 512 or 1024 assumne 1024
    sb.total_blocks = MAX_FILE_SYSTEM_SIZE / block_size; // 4096
    sb.fat_start = 1;
    sb.entry_start = sb.fat_start + (sb.total_blocks * sizeof(int) + block_size - 1) / block_size; // 1 + 4096 * 4 / 1024 = 17
    sb.directory_start = sb.entry_start + (MAX_FILE_COUNT * sizeof(DirectoryEntry)) / block_size; // 1 + 1024 * 64 / 1024 = 17
    sb.data_start = sb.directory_start + (MAX_DIRECTORY_COUNT * sizeof(Directory)) / block_size + MAX_DIRECTORY_COUNT; // 1 + 1024 * 64 / 1024 = 75
    sb.free_blocks = sb.total_blocks - sb.data_start; // 4096 - 73 = 4021
    sb.total_entries = 0;
    sb.total_directories = 1;
    // write super block
    // set cursor to the start of the super block
    fseek(fs, 0, SEEK_SET);
    fwrite(&sb, sizeof(SuperBlock), 1, fs);

    // initialize FAT and write it
    int* fat = (int*) calloc(sb.total_blocks, sizeof(int));
    if (!fat) {
        perror("Unable to allocate FAT");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < sb.total_blocks; i++) { // -1 means free block 0 means used block 1 means end of file
        fat[i] = -1;
    }
    for(int i = 0; i < sb.data_start; i++){
        fat[i] = 1;
    }
    // set cursor to the start of the FAT
    fseek(fs, sb.fat_start * block_size, SEEK_SET);
    for(int i = 0; i < sb.total_blocks; i++){
        fwrite(&fat[i], sizeof(int), 1, fs);
    }

    // initialize directory
    DirectoryEntry* directory = (DirectoryEntry*) calloc(MAX_FILE_COUNT, sizeof(DirectoryEntry));
    if (!directory) {
        perror("Unable to allocate directory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_FILE_COUNT; i++) {
        directory[i].size = 0;
    }
    // set cursor to the start of the directory
    fseek(fs, sb.entry_start * block_size, SEEK_SET);
    fwrite(directory, sizeof(DirectoryEntry), MAX_FILE_COUNT, fs);

    // initialize directory
    Directory* directory2 = (Directory*) calloc(MAX_DIRECTORY_COUNT, sizeof(Directory));
    if (!directory2) {
        perror("Unable to allocate directory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_DIRECTORY_COUNT; i++) {
        directory2[i].size = 0;
    }
    // initialize root directory
    strcpy(directory2[0].name, ROOT_DEFAULT_PATHSTR);
    directory2[0].size = 0;
    directory2[0].startBlock = 0;
    directory2[0].dirOrder = 1;
    // set cursor to the start of the directory
    fseek(fs, sb.directory_start * block_size, SEEK_SET);
    fwrite(directory2, sizeof(Directory), MAX_DIRECTORY_COUNT, fs);

    // write data blocks
    // set cursor to the start of the data blocks
    fseek(fs, sb.data_start * block_size, SEEK_SET);
    char* block = (char*) calloc(1, block_size);
    if (!block) {
        perror("Unable to allocate data block");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < sb.total_blocks - (1 + (sb.total_blocks * sizeof(int) + block_size - 1) / block_size); i++) {
        fwrite(block, block_size, 1, fs);
    }

    free(directory);
    free(fat);
    free(block);
    fclose(fs); 
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <block_size_in_KB> <file_system_name>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // argv[1] = 0.5 or 1
    float block_size_float = atof(argv[1]) * 1024;
    if (block_size_float != BLOCK_SIZE_512 && block_size_float != BLOCK_SIZE_1024) {
        fprintf(stderr, "Supported block sizes are 0.5 KB and 1 KB.\n");
        return EXIT_FAILURE;
    }

    createFileSystem(argv[2], block_size_float);
    return EXIT_SUCCESS;
}
