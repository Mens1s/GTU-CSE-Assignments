#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


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

void findEmptyBlock(FILE* fs, SuperBlock* sb, int* block_number) { 
    int* fat = (int*)malloc(sb->total_blocks * sizeof(int));
    fseek(fs, sb->fat_start * sb->block_size, SEEK_SET);
    fread(fat, sizeof(int), sb->total_blocks, fs);
    for (int i = sb->data_start; i < sb->total_blocks; i++) {
        if (fat[i] == -1) {
            *block_number = i;
            break;
        }
    }
    free(fat);
}

int compareFilename(FILE *fs, SuperBlock *sb, const char* filename, DirectoryEntry *entry){
    // find EntryDirectory for filename
    
    int initialBlock = entry->startBlock;

    char *data = (char*)malloc(sb->block_size);
    if (data == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }
    memset(data, 0, sb->block_size);

    // get datas from initialBlock
    fseek(fs, initialBlock * sb->block_size, SEEK_SET);
    fread(data, sb->block_size, 1, fs);
    // check if filename is in the data
    if(strncmp(data, filename, strlen(filename)) == 0){
        if(data[strlen(filename)] == '>'){
            free(data);
            return 1;
        }
        free(data);
        return -1;
    }
    free(data);
    return -1;
}

void putParentDirectory(const char* targetDirectory, char* result){
    // get parent directory
    char *parentDirectory = (char*)malloc(strlen(targetDirectory));
    if (parentDirectory == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    memset(parentDirectory, 0, strlen(targetDirectory));

    int lastSlash = -1;
    for(int k = 0; k < strlen(targetDirectory); k++){
        if(targetDirectory[k] == ROOT_DEFAULT_PATH)
            lastSlash = k;
        parentDirectory[k] = targetDirectory[k];
    }
    if(lastSlash == -1){
        free(parentDirectory);
        return;
    }

    // clear result
    memset(result, 0, 32);
    
    if(lastSlash == 0){
        result[0] = ROOT_DEFAULT_PATH;
        free(parentDirectory);
        return;
    }
    // results is 32 bytes
    for(int k = 0; k < lastSlash; k++){
        result[k] = parentDirectory[k];
    }
    free(parentDirectory);
}

int controlOfParentDirectoryIsPresent(FILE *fs, SuperBlock *sb, const char* targetDirectory){
    
    int directoryStart = sb->directory_start;
    int totalDirectories = sb->total_directories;

    // get parent directory
    char *parentDirectory = (char*)malloc(strlen(targetDirectory));
    if (parentDirectory == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }
    memset(parentDirectory, 0, strlen(targetDirectory));

    int lastSlash = -1;
    for(int k = 0; k < strlen(targetDirectory); k++){
        if(targetDirectory[k] == ROOT_DEFAULT_PATH)
            lastSlash = k;
        parentDirectory[k] = targetDirectory[k];
    }
    if(lastSlash == -1){
        return -1;
    }
    int size = lastSlash + 1;
    // copy parentDirectory to parentDirectory without last slash
    char *parentDirectoryToLastSlash = (char*)malloc(size);
    if (parentDirectoryToLastSlash == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }
    memset(parentDirectoryToLastSlash, 0, strlen(parentDirectoryToLastSlash));
    for(int k = 0; k < lastSlash+1;k++){
        parentDirectoryToLastSlash[k] = parentDirectory[k];
    }

    Directory directory;
    int i = -1;
    int f = 0;
    while(f < totalDirectories){

        fseek(fs, directoryStart * sb->block_size, SEEK_SET);
        fread(&directory, sizeof(Directory), 1, fs);

        if(strlen(directory.name) == strlen(parentDirectoryToLastSlash)){
            if(strncmp(directory.name, parentDirectoryToLastSlash, strlen(parentDirectoryToLastSlash)) == 0){
                i = 1;
                break;
            }
        }

        directoryStart++;
        f++;
    }
    free(parentDirectory);
    free(parentDirectoryToLastSlash);
    return i;
}

int controlOfDirectoryIsPresent(FILE *fs, SuperBlock *sb, const char* targetDirectory){
    
    int directoryStart = sb->directory_start;
    int totalDirectories = sb->total_directories;
    // control of root
    if(strcmp(targetDirectory, ROOT_DEFAULT_PATHSTR) == 0){
        return 1;
    }
    //    strcat(directory.name, "/");

    Directory directory;
    int i = -1;
    int f = 0;
    while(f < totalDirectories){

        fseek(fs, directoryStart * sb->block_size, SEEK_SET);
        fread(&directory, sizeof(Directory), 1, fs);
        if(strlen(directory.name) - 1 == strlen(targetDirectory)){
            if(strncmp(directory.name, targetDirectory, strlen(targetDirectory)) == 0){
                i = 1;
                break;
            }
        }

        directoryStart++;
        f++;
    }

    return i;
}

void printFileName(FILE *fs, SuperBlock *sb, int startBlock){
    char *data = (char*)malloc(sb->block_size);
    if (data == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    memset(data, 0, sb->block_size);
    // get datas from initialBlock
    fseek(fs, startBlock * sb->block_size, SEEK_SET);
    fread(data, sb->block_size, 1, fs);

    // get rid of filename
    int j = 0;
    while(data[j] != '>'){
        j++;
    }
    for(int i = 0; i < j; i++){
        printf("%c", data[i]);
    }
    printf("\n");
    free(data);
}

DirectoryEntry findEntry(FILE *fs, SuperBlock *sb, const char* sourceDirectory){
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;

    DirectoryEntry entry;
    entry.size = -1;
    int i = -1;
    int f = 0;

    while(f < totalEntries){
        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);
        i = compareFilename(fs, sb, sourceDirectory, &entry);

        if(i == 1){
            if(entry.permissions[1] == 'W'){
                break;
            }else{
                printf("No permission to write\n");
                return entry;
            }
        }
        entryStart++;
        f++;
    }
    if(i == -1){
        entry.size = -1;
        return entry;
    }

    return entry;
}

void write(FILE *fs, SuperBlock *sb, const char* sourceDirectory, const char* targetDirectory){
    // find EntryDirectory for targetDirectory
    int entryStart = sb->entry_start;

    if(entryStart > sb->directory_start){
        printf("No space for new entry\n");
        return; 
    }   

    // check if parent directory is present
    if(controlOfParentDirectoryIsPresent(fs, sb, targetDirectory) == -1){
        printf("Parent directory not found\n");
        return;
    }

    // check name is valid
    for(int i = 0; i < strlen(targetDirectory); i++){
        if(targetDirectory[i] == '<' || targetDirectory[i] == '>' || targetDirectory[i] == '\\' || targetDirectory[i] == '|' || targetDirectory[i] == ':' || targetDirectory[i] == '*' || targetDirectory[i] == '?' || targetDirectory[i] == '"' || targetDirectory[i] == ' '){
            printf("Invalid character in filename.. You cannot use in filename: \n\t\t%s\n", "< > / \\ | : * ? \" and space");
            return;
        }
    }

    // check if target directory is present
    if(controlOfDirectoryIsPresent(fs, sb, targetDirectory) == 1){
        printf("Given file name is used as directory.. Please use another name\n");
        return;
    }
   
    // find empty block
    int block_number = -1;
    findEmptyBlock(fs, sb, &block_number);
    if(block_number == -1){
        printf("No space for new directory\n");
        return;
    }  
    // write the source directory to the file system
    FILE *source = fopen(sourceDirectory, "rb");
    if(!source){
        printf("Source directory not found\n");
        return;
    }
    fseek(source, 0, SEEK_END);
    int size = ftell(source);
    fseek(source, 0, SEEK_SET);

    char *data = (char*)malloc(size + strlen(targetDirectory) + 2);
    if (data == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    memset(data, 0, size + strlen(targetDirectory) + 2); 

    int start = block_number;
    int end = block_number + (size+strlen(targetDirectory)+2) / sb->block_size;
    int i = 0;

    DirectoryEntry entry;
    entry.creation_time = time(NULL);
    entry.modification_time = time(NULL);
    entry.size = end - start;
    entry.startBlock = block_number;
    entry.isPasswordProtected = 0;
    entry.dataSize = size;
    entry.entryOrder = sb->total_entries;
    putParentDirectory(targetDirectory, entry.parentDirectory);

    struct stat fileStat;
    stat(sourceDirectory, &fileStat);

    strcpy(entry.permissions, "");
    strcat(entry.permissions, (fileStat.st_mode & S_IRUSR) ? "R" : "-");
    strcat(entry.permissions, (fileStat.st_mode & S_IWUSR) ? "W" : "-");

    int controlSize = size;

    DirectoryEntry controlEntry = findEntry(fs, sb, targetDirectory);
    if(controlEntry.size != -1){
        controlSize = controlEntry.dataSize - entry.dataSize;

        entry.creation_time = controlEntry.creation_time;
        entry.modification_time = time(NULL);
        entry.startBlock = controlEntry.startBlock;
        entry.size = controlEntry.size;
        entry.entryOrder = controlEntry.entryOrder;
        entry.isPasswordProtected = controlEntry.isPasswordProtected;
        start = controlEntry.startBlock;
        end = start;

        writeToBlock(fs, sb->block_size, entryStart + controlEntry.entryOrder, &entry);
    }
    else{
        writeToBlock(fs, sb->block_size, entryStart + sb->total_entries, &entry);
        sb->free_blocks--;
        sb->total_entries++;
    }
    // update directory size
    int directoryStart = sb->directory_start;
    int totalDirectories = sb->total_directories;

    Directory directory;
    int f = 0;
    while(f < totalDirectories){

        fseek(fs, directoryStart * sb->block_size, SEEK_SET);
        fread(&directory, sizeof(Directory), 1, fs);

        if(strncmp(directory.name, entry.parentDirectory, strlen(entry.parentDirectory)) == 0){
            directory.size += controlSize;
            directory.modification_time = time(NULL);
            
            writeToBlock(fs, sb->block_size, directoryStart + directory.dirOrder - 1, &directory);
            break;
        }

        directoryStart++;
        f++;
    }

    
    // add targetDirectory+>+data
    for(int j = 0; j < strlen(targetDirectory); j++){
        data[j] = targetDirectory[j];
    }
    data[strlen(targetDirectory)] = '>';

    
    for(int j = 0; j < size; j++){
        data[j + strlen(targetDirectory) + 1] = fgetc(source);
    }

    if(end == start){
        writeToBlock(fs, sb->block_size, start, data);
        sb->free_blocks--;
        sb->data_start++;
        fseek(fs, sb->fat_start * sb->block_size, SEEK_SET);
        int* fat = (int*)malloc(sb->total_blocks * sizeof(int));
        fread(fat, sizeof(int), sb->total_blocks, fs);
        fat[start] = 1;
        fseek(fs, sb->fat_start * sb->block_size, SEEK_SET);
        fwrite(fat, sizeof(int), sb->total_blocks, fs);
        free(fat);
    }
    else{
        for(int i = start; i <= end; i++){
            writeToBlock(fs, sb->block_size, i, data + (i - start) * sb->block_size);

            // update sb
            sb->free_blocks--;
            sb->data_start++;
            fseek(fs, sb->fat_start * sb->block_size, SEEK_SET);
            int* fat = (int*)malloc(sb->total_blocks * sizeof(int));
            fread(fat, sizeof(int), sb->total_blocks, fs);
            fat[i] = 1;
            fseek(fs, sb->fat_start * sb->block_size, SEEK_SET);
            fwrite(fat, sizeof(int), sb->total_blocks, fs);
            free(fat);
        }
    }    

    free(data);
    fclose(source);

    // update sb
    fseek(fs, 0, SEEK_SET);
    fwrite(sb, sizeof(SuperBlock), 1, fs);

}

void read(FILE *fs, SuperBlock *sb, const char* sourceDirectory, const char* targetDirectory, const char* password){
    // source directory will be in the file system
    // target directory will be in the host system
    // copy the source directory to the target directory

    // find EntryDirectory for sourceDirectory
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;

    DirectoryEntry entry;
    int i = -1;
    int f = 0;
    while(f < totalEntries){

        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);
        i = compareFilename(fs, sb, sourceDirectory, &entry);

        if(i == 1){
            if(entry.permissions[0] == 'R'){
                if(entry.isPasswordProtected == 1){
                    if(strcmp(password, entry.password) == 0){
                        break;
                    }
                    else{
                        printf("Wrong password\n");
                        return;
                    }
                }
                else{
                    break;
                }
            }else{
                printf("No permission to read\n");
                return;
            }
            
            
        }
        entryStart++;
        f++;
    }

    if(i == -1){
        printf("Source directory not found\n");
        return;
    }

    // get data from entry.startBlock+filename+> and write it to targetDirectory
    int initialBlock = entry.startBlock;
    int endBlock = initialBlock + entry.size;
    int dataSize = entry.dataSize;

    char *data = (char*)malloc(sb->block_size);
    if (data == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    memset(data, 0, sb->block_size);

    // get datas from initialBlock
    fseek(fs, initialBlock * sb->block_size, SEEK_SET);
    fread(data, sb->block_size, 1, fs);

    // get rid of filename
    int j = 0;
    while(data[j] != '>'){
        j++;
    }
    j++;

    // write data to targetDirectory
    FILE *target = fopen(targetDirectory, "wb");
    if(entry.permissions[1] == 'W' && entry.permissions[0] == 'R'){
        if(chmod(targetDirectory, S_IRUSR | S_IWUSR) == -1)
            printf("No permission to write\n");
    }else if(entry.permissions[0] == 'R'){
        if(chmod(targetDirectory, S_IRUSR) == -1)
            printf("No permission to read\n");
    }else if(entry.permissions[1] == 'W'){
        if(chmod(targetDirectory, S_IWUSR) == -1)
            printf("No permission to write\n");
    }
    
    
    if(!target){
        printf("Target directory not found\n");
        free(data);
        return;
    }
    int writtenSize = 0;
    for(int i = initialBlock; i <= endBlock; i++){
        fseek(fs, i * sb->block_size, SEEK_SET);
        fread(data, sb->block_size, 1, fs);

        if(i == initialBlock){
            // remove filename
            if(i == endBlock){
                fwrite(data + j, dataSize, 1, target);
            }
            else{
                fwrite(data + j, sb->block_size - j, 1, target);
            }
            //fwrite(data + j, sb->block_size - j, 1, target);
        }
        else if(i == endBlock){
            fwrite(data, dataSize-writtenSize+j, 1, target);
        }
        else{
            fwrite(data, sb->block_size, 1, target);
        }
        writtenSize += sb->block_size;
    }
    
    free(data);
    fclose(target);
}

void mKdir(FILE *fs, SuperBlock *sb, const char* targetDirectory){
    // control of parent directory
    if(controlOfParentDirectoryIsPresent(fs, sb, targetDirectory) == -1){
        printf("Parent directory not found\n");
        return;
    }

    // control of target directory is present
    if(controlOfDirectoryIsPresent(fs, sb, targetDirectory) == 1){
        printf("Directory already exists\n");
        return;
    }

    // there is space for new directory
    int directoryStart = sb->directory_start;
    int totalDirectories = sb->total_directories;
    
    if(directoryStart + totalDirectories > sb->data_start){
        printf("No space for new directory\n");
        return;
    }

    // find empty block
    Directory directory;
    directory.size = 1;
    directory.startBlock = directoryStart;
    directory.dirOrder = totalDirectories;
    directory.creation_time = time(NULL);

    strcpy(directory.name, targetDirectory);
    strcat(directory.name, ROOT_DEFAULT_PATHSTR);

    writeToBlock(fs, sb->block_size, directoryStart + totalDirectories, &directory);

    // update sb
    sb->total_directories++;
    sb->free_blocks--;
    
    fseek(fs, 0, SEEK_SET);
    fwrite(sb, sizeof(SuperBlock), 1, fs);

}

int dir(FILE *fs, SuperBlock *sb, const char* targetDirectory, int isPrint){
    // control of target directory is present
    if(controlOfDirectoryIsPresent(fs, sb, targetDirectory) == -1){
        printf("Directory not found\n");
        return -1;
    }

    Directory directory;
    int directoryStart = sb->directory_start;
    int totalDirectories = sb->total_directories;
    int i = -1;
    int f = 0;
    while(f < totalDirectories){

        fseek(fs, directoryStart * sb->block_size, SEEK_SET);
        fread(&directory, sizeof(Directory), 1, fs);

        if(strncmp(directory.name, targetDirectory, strlen(targetDirectory)) == 0){
            int isBaseDir = 0;
            for(int k = strlen(targetDirectory) + 1; k < strlen(directory.name); k++){
                if(directory.name[k] == ROOT_DEFAULT_PATH){
                    isBaseDir++;
                }
            }
            if(isPrint == 1 && isBaseDir == 1){
                printf("Directory Name: ");
                for(int k = 0; k < strlen(directory.name); k++){
                    printf("%c", directory.name[k]);
                }
                printf("\t\tSize: %d\n", directory.size);
                printf("\t\tCreation time: %s", ctime(&directory.creation_time));
                printf("\t\tModification time: %s\n", ctime(&directory.modification_time));
            }
            i = 1;
        }

        directoryStart++;
        f++;
    }
    
    // find EntryDirectory for targetDirectory
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;

    DirectoryEntry entry;
    i = -1;
    f = 0;
    int amount = 0;
    while(f < totalEntries){

        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);

        if(strcmp(entry.parentDirectory, targetDirectory) == 0){
            if(isPrint == 1){
                printf("File Name: ");
                printFileName(fs, sb, entry.startBlock);
                printf("\t\tSize: %d\n", entry.dataSize);
                printf("\t\tPermissions: %s\n", entry.permissions);
                printf("\t\tCreation time: %s", ctime(&entry.creation_time));
                printf("\t\tModification time: %s\n", ctime(&entry.modification_time));
            }
            ++amount;
        }
        
        if(i == 1){
            break;
        }
        entryStart++;
        f++;
    }

    return amount;
}

void rmdir(FILE *fs, SuperBlock *sb, const char* targetDirectory){
    // control of target directory is present
    if(controlOfDirectoryIsPresent(fs, sb, targetDirectory) == -1){
        printf("Directory not found\n");
        return;
    }
    
    int isHoldFiles = dir(fs, sb, targetDirectory, 0);
    if(isHoldFiles > 0){
        printf("Directory is not empty\n");
        return;
    }

    // find directory and delete it
    int directoryStart = sb->directory_start;
    int totalDirectories = sb->total_directories;

    Directory directory;
    int i = -1;
    int f = 0;
    while(f < totalDirectories){

        fseek(fs, directoryStart * sb->block_size, SEEK_SET);
        fread(&directory, sizeof(Directory), 1, fs);
        if(strlen(directory.name) - 1 == strlen(targetDirectory)){
            if(strncmp(directory.name, targetDirectory, strlen(targetDirectory)) == 0){
                i = 1;
                break;
            }
        }

        directoryStart++;
        f++;
    }
    if(i == -1){
        printf("Directory not found\n");
        return;
    }

    // delete directory
    directory.size = 0;
    directory.startBlock = 0;
    memset(directory.name, 0, 32);

    writeToBlock(fs, sb->block_size, directoryStart, &directory);
    // deleted files cannot be used...
    // get last directory and put it to the deleted directory
    Directory lastDirectory;
    fseek(fs, (directoryStart + totalDirectories - 1) * sb->block_size, SEEK_SET);
    fread(&lastDirectory, sizeof(Directory), 1, fs);
    writeToBlock(fs, sb->block_size, directoryStart, &lastDirectory);
    // update sb
    sb->total_directories--;
    sb->free_blocks--;

    fseek(fs, 0, SEEK_SET);
    fwrite(sb, sizeof(SuperBlock), 1, fs);
}

void dump2fs(SuperBlock *sb, FILE *fs) {
    // read all information from the file system and print it
    printf("-----------------------------------------------------------------\n");
    printf("----\t\tBlock size       : %d\n", sb->block_size);
    printf("----\t\tTotal blocks     : %d\n", sb->total_blocks);
    printf("----\t\tFree blocks      : %d\n", sb->free_blocks);
    printf("----\t\tFAT start        : %d\n", sb->fat_start);
    printf("----\t\tEntry start      : %d\n", sb->entry_start);
    printf("----\t\tTotal entries    : %d\n", sb->total_entries);
    printf("----\t\tDirectory start  : %d\n", sb->directory_start);
    printf("----\t\tTotal directories: %d\n", sb->total_directories);
    printf("----\t\tData start       : %d\n", sb->total_directories + sb->directory_start);
    printf("-----------------------------------------------------------------\n");
    // find EntryDirectory for targetDirectory
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;

    DirectoryEntry entry;
    int i = -1;
    int f = 0;
    int amount = 0;

    while(f < totalEntries){

        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);
        
        printf("Block: %d\nFullPathOfFile: ", entry.startBlock);
        printFileName(fs, sb, entry.startBlock);
        printf("\t\tSize: %d\n", entry.dataSize);
        printf("\t\tPermissions: %s\n", entry.permissions);
        printf("\t\tCreation time: %s", ctime(&entry.creation_time));
        printf("\t\tModification time: %s\n", ctime(&entry.modification_time));
        printf("\t\tParent directory: %s\n", entry.parentDirectory);
        if(entry.isPasswordProtected == 1){
            printf("\t\tPassword: %s\n", entry.password);
        }

        entryStart++;
        f++;
    }
}

void del(FILE *fs, SuperBlock *sb, const char *targetDirectory){
    // find entry and delete it
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;
    char *data = (char*)malloc(sb->block_size);
    if (data == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    
    DirectoryEntry entry;
    int i = -1;
    int f = 0;
    while(f < totalEntries){

        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);
        
        
        memset(data, 0, sb->block_size);
        // get datas from initialBlock
        fseek(fs, entry.startBlock * sb->block_size, SEEK_SET);
        fread(data, sb->block_size, 1, fs);

        // get rid of filename
        int j = 0;
        while(data[j] != '>'){
            j++;
        }
       
        if(strncmp(data, targetDirectory, j) == 0){
            i = 1;
            break;
        }

        entryStart++;
        f++;
    }

    free(data);
    if(i == -1){
        printf("Entry not found\n");
        return;
    }
    int entryOrder = entry.entryOrder;
    // delete directory
    entry.size = 0;
    entry.startBlock = 0;
    entry.dataSize = 0;
    entry.isPasswordProtected = 0;
    entry.creation_time = 0;
    entry.modification_time = 0;
    memset(entry.permissions, 0, 2);
    memset(entry.parentDirectory, 0, 32);
    memset(entry.password, 0, 32);
    
    writeToBlock(fs, sb->block_size, entryStart + entryOrder, &entry);
    // deleted files cannot be used...
    // control of it is the last entry

    if(entryStart + entryOrder == sb->entry_start + sb->total_entries){
        // update sb
        sb->total_entries--;
        sb->free_blocks--;
        
        fseek(fs, 0, SEEK_SET);
        fwrite(sb, sizeof(SuperBlock), 1, fs);
        return;
    }
    // get last entry and put it to the deleted entry
    DirectoryEntry lastEntry;
    fseek(fs, (entryStart + totalEntries - 1) * sb->block_size, SEEK_SET);
    fread(&lastEntry, sizeof(DirectoryEntry), 1, fs);
    lastEntry.entryOrder = entryOrder;

    writeToBlock(fs, sb->block_size, entryStart + entryOrder, &lastEntry);

    // update sb
    sb->total_entries--;
    sb->free_blocks--;
    
    fseek(fs, 0, SEEK_SET);
    fwrite(sb, sizeof(SuperBlock), 1, fs);
}

void cHmod(FILE *fs, SuperBlock *sb, const char *targetDirectory, char *permissions){
    int isPlus = 0;
    int isMinus = 0;
    int isR = 0;
    int isW = 0;
    if(strlen(permissions) != 3){
        printf("Invalid permission\n");
        return;
    }

    if(permissions[0] == '+')
        isPlus = 1;
    else if(permissions[0] == '-')
        isMinus = 1;
    else{
        printf("Invalid permission\n");
        return;
    }

    if(permissions[1] == 'r')
        isR = 1;
    if(permissions[1] == 'w')
        isW = 1;
    if(permissions[1] == 'r' && permissions[2] == 'w'){
        isR = 1;
        isW = 1;
    }

    if(permissions[1] == 'w')
        isW = 1;
    if(isW == 0 && isR == 0){
        printf("Invalid permission\n");
        return;
    }

    // find entry and change permissions
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;
    char *data = (char*)malloc(sb->block_size);
    if (data == NULL) {
        perror("Failed to allocate memory");
        return;
    }

    DirectoryEntry entry;
    int i = -1;
    int f = 0;
    while(f < totalEntries){

        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);

        memset(data, 0, sb->block_size);
        // get datas from initialBlock
        fseek(fs, entry.startBlock * sb->block_size, SEEK_SET);
        fread(data, sb->block_size, 1, fs);

        // get rid of filename
        int j = 0;
        while(data[j] != '>'){
            j++;
        }
       
        if(strncmp(data, targetDirectory, j) == 0){
            i = 1;
            break;
        }

        entryStart++;
        f++;
    }

    free(data);
    if(i == -1){
        printf("Entry not found\n");
        return;
    }
    permissions[0] = entry.permissions[0];
    permissions[1] = entry.permissions[1];
    // set permissions
    if(isPlus == 1){
        if(isR == 1){
            if(isW == 1){
                permissions[0] = 'R';
                permissions[1] = 'W';
            }else{
                permissions[0] = 'R';
            }
        }else if(isW == 1){
            permissions[1] = 'W';
        }
    }else if(isMinus == 1){
        if(isR == 1){
            if(isW == 1){
                permissions[0] = '-';
                permissions[1] = '-';
            }else{
                permissions[0] = '-';
            }
        }else if(isW == 1){
            permissions[1] = '-';
        }
    }

    // change permissions
    entry.permissions[0] = permissions[0];
    entry.permissions[1] = permissions[1];
    entry.modification_time = time(NULL);
    writeToBlock(fs, sb->block_size, entryStart, &entry);
}

void addPw(FILE *fs, SuperBlock *sb, const char *targetDirectory, const char *password){
    if(strlen(password) > 32){
        printf("Password is too long\n");
        return;
    }else if(strlen(password) < 3){
        printf("Password is too short\n");
        return;
    }

    // find entry and change permissions
    int entryStart = sb->entry_start;
    int totalEntries = sb->total_entries;
    char *data = (char*)malloc(sb->block_size);

    DirectoryEntry entry;
    int i = -1;
    int f = 0;
    while(f < totalEntries){

        fseek(fs, entryStart * sb->block_size, SEEK_SET);
        fread(&entry, sizeof(DirectoryEntry), 1, fs);
        
        if (data == NULL) {
            perror("Failed to allocate memory");
            return;
        }
        memset(data, 0, sb->block_size);
        // get datas from initialBlock
        fseek(fs, entry.startBlock * sb->block_size, SEEK_SET);
        fread(data, sb->block_size, 1, fs);

        // get rid of filename
        int j = 0;
        while(data[j] != '>'){
            j++;
        }
       

        if(strncmp(data, targetDirectory, j) == 0){
            i = 1;
            break;
        }

        entryStart++;
        f++;
    }

    free(data);
    if(i == -1){
        printf("Entry not found\n");
        return;
    }

    entry.isPasswordProtected = 1;
    // change permissions
    strcpy(entry.password, password);
    writeToBlock(fs, sb->block_size, entryStart, &entry);
}

int main(int argc, char* argv[]) {
    FILE* fs = fopen(argv[1], "rb+");
    if (!fs) {
        perror("Unable to open file");
        return 1;
    }

    char command[10];
    strcpy(command, argv[2]);

    SuperBlock sb;
    fread(&sb, sizeof(SuperBlock), 1, fs);

    if(strcmp(command, "write") == 0){
        if(argc != 5)
            printf("Usage of write: write <file_system> <command> <target_directory> <source_directory>\n");
        else
            write(fs, &sb, argv[4], argv[3]); 
    }
    else if(strcmp(command, "read") == 0){
        if(argc == 5)
            read(fs, &sb, argv[3], argv[4], "");
        else if(argc == 6)
            read(fs, &sb, argv[3], argv[4], argv[5]);
        else
            printf("Invalid number of arguments\n");
    }else if(strcmp(command, "mkdir") == 0){
        if(argc != 4)
            printf("Usage of mkdir: mkdir <file_system> <command> <directory_name>\n");
        else
            mKdir(fs, &sb, argv[3]);
    }else if(strcmp(command, "dir") == 0){
        if(argc != 4)
            printf("Invalid number of arguments\n");
        else
            dir(fs, &sb, argv[3], 1);
    }else if(strcmp(command, "rmdir") == 0){
        if(argc != 4)
            printf("Invalid number of arguments\n");
        else
            rmdir(fs, &sb, argv[3]);
    }else if(strcmp(command, "dumpe2fs") == 0){
        if(argc != 3)
            printf("Invalid number of arguments\n");
        else
            dump2fs(&sb, fs);
    }else if(strcmp(command, "del") == 0){
        if(argc != 4)
            printf("Invalid number of arguments\n");
        else
            del(fs, &sb, argv[3]);
    }else if(strcmp(command, "chmod") == 0){
        if(argc != 5)
            printf("Invalid number of arguments\n");
        else
            cHmod(fs, &sb, argv[3], argv[4]);
    }else if(strcmp(command, "addpw") == 0){
        if(argc != 5)
            printf("Invalid number of arguments\n");
        else
            addPw(fs, &sb, argv[3], argv[4]);
    }
    else{
        printf("Invalid command\n");
    }

    fclose(fs);
    return 0;
}