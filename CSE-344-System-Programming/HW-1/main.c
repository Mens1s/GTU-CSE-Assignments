#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_NAME_LENGTH 100
#define MAX_FILE_NAME 100
#define MAX_LINE_LENGTH 250
#define MAX_STUDENT 25000
#define LOG_FILE_NAME "log.txt"

typedef struct {
    char name[MAX_NAME_LENGTH];
    char grade[3];
} Student;

void createFile(char * fileName, char *argv[], int argc); // file created in sepereted process
void addStudentGrade(char * fullName, char * grade, char * fileName, char *argv[], int argc); // add informations seperate process / add to end of the file
char * searchStudentByName(char * fullName, char * fileName, char *argv[], int argc); // search student and print/return his name and grade to termial
void sortFileByParameter(char * fileName,char * parameter, char * order, int isDefaultOrder, int isDefaultParameter, char *argv[], int argc); // sort file by parameter which can be name or grade and print to terminal default parameter is name -- insertion sort used
void showAll(char * fileName, char *argv[], int argc); // print all entries of file
void listGrades(char * fileName, char *argv[], int argc); // print first 5 entries in the file
void listSome(int numberOfEntries, int pageNumber, char * fileName, char *argv[], int argc); // print number of entries at page number in the file
void logActions(char * action, char *argv[], int argc); // logs all actions to file as named log.txt
void manGtuStudentGrades(); // shows available functions ""
int appendStringToFile(char * fileName, char * data, char *argv[], int argc); // appends data using syscall
int getDatasFromFileByParameters(char * fileName, int startEntry, int endEntry, Student * students, char *argv[], int argc); // reads data using syscall
void asTerminal(int argc, char *argv[]);
void argvCleaner(char *argv[], int argc);

int main(int argc, char *argv[]) {
    printf("If you want to quit press Q\n");
    char command[100];
    char *args[100];
    int i = 0;

    while(1){
        printf(">");
        fflush(stdout);
        ssize_t bytes_read;
       
        bytes_read = read(STDIN_FILENO, command, sizeof(command));

        if (bytes_read == -1) {
            logActions("Error reading from stdin", argv, argc);
            perror("Error reading from stdin");
            return 1;
        }else 
            command[bytes_read] = '\0';
        
       if(strcmp(command, "Q\n") == 0){
            printf("Exiting...\n");
            break;
        }

        //arg0 "arg1" "arg2" "arg3" "arg4"
        int isCommandFind = 0;
        char temp[100];
        int charLen = 0;
        int isQuote = 0;
        for(int idx = 0; idx < strlen(command); idx++){
            if(isCommandFind == 0 && (command[idx] == ' ' || command[idx] == '\n')){
                temp[charLen] = '\0';
                args[i] = malloc((charLen+1) * sizeof(char));
                strcpy(args[i++], temp);
                charLen = 0;
                isCommandFind = 1;
                // temp clear
                for(int j = 0; j < 100; j++)
                    temp[j] = '\0';
                continue;
            }
            if(command[idx] == '"'){
                if(isQuote == 0)
                    isQuote = 1;
                else{
                    temp[charLen] = '\0';
                    args[i] = malloc((charLen+1) * sizeof(char));

                    strcpy(args[i++], temp);
                    charLen = 0;
                    isQuote = 0;
                    // temp clear
                    for(int j = 0; j < 100; j++)
                        temp[j] = '\0';        
                }
                continue;
            }
            if(command[idx] == '\0')
                break;
            if(command[idx] == ' ' && isQuote != 1)
                continue;
            temp[charLen++] = command[idx];
        }

        args[i-1][strlen(args[i-1])] = '\0';
        
        asTerminal(i-1, args);
        i = 0;
    }
    return 0;
}

void asTerminal(int argc, char *argv[]){

    char *command = argv[0];
    char *arg1 = (argc >= 1) ? argv[1] : NULL;
    char *arg2 = (argc >= 2) ? argv[2] : NULL; // Optional third argument
    char *arg3 = (argc >= 3) ? argv[3] : NULL; // Optional fourth argument
    char *arg4 = (argc >= 4) ? argv[4] : NULL; // Optional fourth argument

    if (strcmp(command, "gtuStudentGrades") == 0) {
        if(argc == 0){
            manGtuStudentGrades();
        }else if(argc == 1){
            createFile(arg1, argv, argc);
        }else{
            printf("Usage: gtuStudentGrades <filename>\n");
        }
    } else if (strcmp(command, "addStudentGrade") == 0) {
        if (argc != 3) {
            printf("Usage: addStudentGrade <name> <grade> <filename>\n");
        } else {
            addStudentGrade(arg1, arg2, arg3, argv, argc);
        }
    } else if (strcmp(command, "searchStudent") == 0) {
        if (argc != 2) {
            printf("Usage: searchStudent <name> <filename>\n");
        } else {
            char *result = searchStudentByName(arg1, arg2, argv, argc);
            if (result != NULL) {
                printf("%s\n", result);
                free(result);
            } else {
                printf("Student Not Found.\n");
            }
        }
    } else if (strcmp(command, "showAll") == 0) {
        if (argc != 1) {
            printf("Usage: showAll <filename>\n");
        } else {
            showAll(arg1, argv, argc);
        }
    } else if (strcmp(command, "listGrades") == 0) {
        if (argc != 1) {
            printf("Usage: listGrades <filename>\n");
        } else {
            listGrades(arg1, argv, argc);
        }
    } else if (strcmp(command, "listSome") == 0) {
        if (argc != 3) {
            printf("Usage: listSome <num_entries> <page_number> <filename>\n");
        } else {
            // control for integer
            int numberOfEntries = atoi(arg1);
            int pageNumber = atoi(arg2);
            listSome(numberOfEntries, pageNumber, arg3, argv, argc);
        }
    } else if (strcmp(command, "sortAll") == 0) {
        if (!(argc == 3 || argc == 2 || argc == 1)) {
            printf("Usage: sort <filename> <parameter> <parameter:optional>\n");
        } else {
            sortFileByParameter(arg1, arg2, arg3, argc <= 2, argc == 1, argv, argc);
        }
    } else {
        printf("Invalid command. Use 'gtuStudentGrades' for usage information.\n");
    }
    // free all
    argvCleaner(argv, argc);
}

void createFile(char * fileName, char *argv[], int argc){
    pid_t pid = fork();

    if(pid == 0){
        // fd = open(fileName, O_RDONLY);
        // use syscall to create file
        int fd = open(fileName, O_CREAT | O_WRONLY, 0644);
        if(fd == -1){
            logActions("FAILURE! File cannot created!", argv, argc);
            perror("open");
            argvCleaner(argv, argc);
            exit(EXIT_FAILURE);
        }else{
            close(fd);
            logActions("File created succesfully!", argv, argc);
        }
        argvCleaner(argv, argc);
        exit(1);
    }else if(pid > 0) {
        wait(NULL);
    }else{
        logActions("Proccesses cannot created!", argv, argc);
    }
}

void addStudentGrade(char * fullName, char * grade, char * fileName, char *argv[], int argc){
    pid_t pid = fork();
    if(pid == 0){
        char data[strlen(fullName)+strlen(grade)];
        strcpy(data, fullName);
        strcat(data, ", ");
        strcat(data, grade);
        strcat(data, "\n");

        int i = appendStringToFile(fileName, data, argv, argc);

        if(i == 0)
            logActions("Student and grade added to file succesfully!", argv, argc);
        else
            logActions("FAILURE! Student and grade cannot added to file!", argv, argc);

        argvCleaner(argv, argc);
        exit(1);
    }else if(pid > 0){
        wait(NULL);
    }else{
        logActions("Proccesses cannot created!", argv, argc);
    }
}

char * searchStudentByName(char * fullName, char * fileName, char *argv[], int argc){
    char * result = NULL;
    char * resultChild = NULL;
    int pipefd[2];

    // pipe for communication between parent and child
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid == 0){
        close(pipefd[0]); // parent communication(r/w) close
        int fd;
        char line[MAX_LINE_LENGTH];
        char tempLine[MAX_LINE_LENGTH];
        ssize_t byteData;
        int lineNumber = 0;
        int lineStartIndex = 0;
        int index = 0;
        int wordLen = 0;
        
        fd = open(fileName, O_RDONLY);

        if (fd == -1){
            logActions("File cannot openned!", argv, argc);
            perror("open");
            argvCleaner(argv, argc);
            exit(EXIT_FAILURE);
        }

        while ((byteData = read(fd, line, 1)) > 0) {
            if (line[0] == '\n') {
                tempLine[lineStartIndex++] = '\n'; 
                
                char * nameField = strtok(tempLine, ",");

                if(strcmp(nameField, fullName) == 0){
                    char * gradeField = strtok(NULL, ",");
                    resultChild = malloc((strlen(nameField) + strlen(gradeField) + 1) * sizeof(char));

                    if (resultChild == NULL) {
                        perror("malloc");
                        argvCleaner(argv, argc);
                        free(resultChild);
                        exit(EXIT_FAILURE);
                    }

                    // resultchild += (nameField + gradeField)
                    strcpy(resultChild, nameField);
                    strcat(resultChild, gradeField);
                    resultChild[strlen(resultChild)-1] = '\0';
                    // write result to pipe
                    write(pipefd[1], resultChild, strlen(resultChild) + 1);
                    close(pipefd[1]);
                    free(resultChild);
                    memset(line, 0, sizeof(line));
                    close(fd);
                    exit(EXIT_SUCCESS);
                }
                lineNumber++;
                lineStartIndex = 0; 
                wordLen = 0;
                // clear temp line
                for(int i = 0; i < MAX_LINE_LENGTH; i++)
                    tempLine[i] = '\0';
            } else {
                if (lineStartIndex < MAX_LINE_LENGTH - 1) {
                    tempLine[lineStartIndex++] = line[0];
                } else {
                    tempLine[lineStartIndex] = '\0';
                    printf("%s (Line truncated)\n", tempLine);

                    while ((byteData = read(fd, line, 1)) > 0 && line[0] != '\n');
                    lineStartIndex = 0; 
                }
            }
            

            wordLen++;
            memset(line, 0, sizeof(line));
        }
        if(byteData == -1){
            logActions("Read operations cannot done!", argv, argc);
            perror("read");
            exit(EXIT_FAILURE);
        }

        char * errorNotFound = "Student Not Found.\n";
        write(pipefd[1], errorNotFound, strlen(errorNotFound) + 1);
        close(pipefd[1]);
        close(fd);
        argvCleaner(argv, argc);
        exit(EXIT_SUCCESS);
    }
    else if(pid > 0){
        close(pipefd[1]); // child communication(r/w) close

        resultChild = malloc(MAX_LINE_LENGTH * sizeof(char));
        if (resultChild == NULL) {
            perror("malloc");
            logActions("Malloc operations cannot done!", argv, argc);
            exit(EXIT_FAILURE);
        }

        read(pipefd[0], resultChild, MAX_LINE_LENGTH); // get result from child
       
        int status; // wait to child process end
        waitpid(pid, &status, 0);
       
        if(WIFEXITED(status)){ // exit successful except open or malloc failure
            if(WEXITSTATUS(status) == EXIT_SUCCESS){
                result = strdup(resultChild);   
                logActions("Search operations done succesfully!", argv, argc);        
            }else if(WEXITSTATUS(status) == EXIT_FAILURE){
                logActions("Search operations cannot done!", argv, argc);
            }
        }
        close(pipefd[0]); // close pipe comm.
        free(resultChild); // Free memory allocated for resultChild
    }
    else{
        perror("fork");
        logActions("Proccesses cannot created!", argv, argc);
        exit(EXIT_FAILURE);
    }
    
    return result;
}

void sortFileByParameter(char * fileName,char * parameter, char * order, int isDefaultOrder, int isDefaultParameter, char *argv[], int argc){
    pid_t pid = fork();

    if(pid == 0){
        Student * students = (Student *)malloc(MAX_STUDENT * sizeof(Student));

        int studentCount = getDatasFromFileByParameters(fileName, 0, -1, students, argv, argc);

        if(isDefaultParameter == 1 || strcmp(parameter, "name") == 0){
            for(int i = 1; i < studentCount; i++){
                Student temp = students[i];
                int j = i - 1;
                while(j >= 0 && strcasecmp(students[j].name, temp.name) > 0){
                    students[j+1] = students[j];
                    j--;
                }
                students[j+1] = temp;
            }
        }else if(strcmp(parameter, "grade") == 0){
            for(int i = 1; i < studentCount; i++){
                Student temp = students[i];
                int j = i - 1;
                while(j >= 0 && strcasecmp(students[j].grade, temp.grade) > 0){
                    students[j+1] = students[j];
                    j--;
                }
                students[j+1] = temp;
            }
        }else{
            argvCleaner(argv, argc);
            free(students);
            logActions("Invalid parameter for sort!", argv, argc);
            exit(EXIT_FAILURE);
        }

        if(isDefaultOrder == 1)
            order = "asc";

        if(strcmp(order, "desc") == 0){
            for(int i = 0; i < studentCount/2; i++){
                Student temp = students[i];
                students[i] = students[studentCount - i - 1];
                students[studentCount - i - 1] = temp;
            }
        }else if(strcmp(order, "asc") != 0){
            argvCleaner(argv, argc);
            free(students);
            logActions("Invalid order for sort!", argv, argc);
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < studentCount; i++)
            printf("%s, %s\n", students[i].name, students[i].grade);

        free(students);
        logActions("Sort operations done succesfully!", argv, argc);
        argvCleaner(argv, argc);
        exit(EXIT_SUCCESS);
    }
    else if(pid > 0) {
        wait(NULL);
    }else{
        logActions("Proccesses cannot created!", argv, argc);
    }
}

void showAll(char * fileName, char *argv[], int argc){
    pid_t pid = fork();

    if(pid == 0){
        Student * students = (Student *)malloc(MAX_STUDENT * sizeof(Student));

        int studentCount = getDatasFromFileByParameters(fileName, 0, -1, students, argv, argc);

        for(int i = 0; i < studentCount; i++)
            printf("%s, %s\n", students[i].name, students[i].grade);

        free(students);

        logActions("All informations from file showed succesfully!", argv, argc);
        argvCleaner(argv, argc);
        exit(EXIT_SUCCESS);
    }else if(pid > 0) {
        wait(NULL);
    }else{
        logActions("FAILURE! All informations from file cannot showed!", argv, argc);
        logActions("Proccesses cannot created!", argv, argc);
    }
}

void listGrades(char * fileName, char *argv[], int argc){
    pid_t pid = fork();

    if(pid == 0){
        Student * students = (Student *)malloc(MAX_STUDENT * sizeof(Student));

        int studentCount = getDatasFromFileByParameters(fileName, 0, 5, students, argv, argc);

        for(int i = 0; i < studentCount; i++)
            printf("%s, %s\n", students[i].name, students[i].grade);

        free(students);

        logActions("Only first 5 entries from the file showed succesfully!", argv, argc);
        argvCleaner(argv, argc);
        
        exit(EXIT_SUCCESS);
    }else if(pid > 0) {
        wait(NULL);
    }else{
        logActions("FAILURE! irst 5 entries from the file cannot showed!", argv, argc);
        logActions("Proccesses cannot created!", argv, argc);
    }
}

void listSome(int numberOfEntries, int pageNumber, char * fileName, char *argv[], int argc){
    pid_t pid = fork();

    if(pid == 0){
        int startEntry = numberOfEntries * (pageNumber-1);
        int endEntry = numberOfEntries * pageNumber;

        Student * students = (Student *)malloc(MAX_STUDENT * sizeof(Student));

        int studentCount = getDatasFromFileByParameters(fileName, startEntry, endEntry, students, argv, argc);

        if(studentCount <= 0){
            argvCleaner(argv, argc);
            free(students);
            logActions("FAILURE! Entries between <user given range> from the file cannot showed!", argv, argc);
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < studentCount; i++)
            printf("%s, %s\n", students[i].name, students[i].grade);
        
        free(students);
        argvCleaner(argv, argc);
        logActions("Entries between <user given range> from the file showed succesfully!", argv, argc);
        exit(EXIT_SUCCESS);
    }else if(pid > 0) {
        wait(NULL);
    }else{
        logActions("FAILURE! First 5 entries from the file cannot showed!", argv, argc);
        logActions("Proccesses cannot created!", argv, argc);
    }
}

void logActions(char * action, char *argv[], int argc){
    pid_t pid = fork();

    if(pid == 0){
        // open file to append action using open
        int fd = open(LOG_FILE_NAME, O_WRONLY | O_APPEND);
        if(fd == -1){
            int fdd = open(LOG_FILE_NAME, O_CREAT | O_WRONLY, 0644);
            if(fdd == -1){
                perror("open");
                exit(EXIT_FAILURE);
            }
            write(fdd, action, strlen(action));
            write(fdd, "\n", 1);
            close(fdd);
            argvCleaner(argv, argc);
            exit(EXIT_FAILURE);
        }else{
            write(fd, action, strlen(action));
            write(fd, "\n", 1);
            close(fd);
            argvCleaner(argv, argc);
            exit(EXIT_SUCCESS);
        }
        argvCleaner(argv, argc);
        exit(EXIT_SUCCESS);
    }else if(pid > 0) {
        wait(NULL);
    }else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void manGtuStudentGrades() {
    printf("gtuStudentGrades [COMMAND] [ARGUMENTS]\n\n");
    printf("Available commands:\n");
    printf("  <filename>\t - Creates a new file to store student grades.\n");
    printf("  addStudentGrade <name> <grade> <filename>\n");
    printf("\t\t - Adds a new student's name and grade to the specified file.\n");
    printf("  searchStudent <name> <filename>\n");
    printf("\t\t - Searches for a student's grade by name in the specified file.\n");
    printf("  sortAll <filename> <field:Optional> <order:Optional>\n");
    printf("\t\t - Parameter can be name or grade. Default is name. Default is asc.\n");
    printf("\t\t - Sorts all student grades stored in the specified file.\n");
    printf("  showAll <filename>\n");
    printf("\t\t - Displays all student grades stored in the specified file.\n");
    printf("  listGrades <filename>\n");
    printf("\t\t - Displays the first 5 student grades stored in the specified file.\n");
    printf("  listSome <num_entries> <page_number> <filename>\n");
    printf("\t\t - Displays a specific range of student grades from the specified file.\n");
    printf("\t\t num_entries: Number of entries per page.\n");
    printf("\t\t page_number: Page number to display.\n");
    printf("  gtuStudentGrades\t\t - Displays this help message.\n");
}
// this functions works on fork so there is no need to fork again
int appendStringToFile(char * fileName, char * data, char *argv[], int argc){
    int fd;

    fd = open(fileName, O_WRONLY | O_APPEND);

    if (fd == -1){
        logActions("File cannot openned!", argv, argc);
        perror("open");
        return -1;
    }

    ssize_t writeToFile = write(fd, data, strlen(data));

    if(writeToFile == -1){
        logActions("Write operations cannot done!", argv, argc);
        perror("write");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

// this functions works on fork so there is no need to fork again
int getDatasFromFileByParameters(char * fileName, int startEntry, int endEntry, Student * students, char *argv[], int argc){
    int fd;
    char buffer[MAX_LINE_LENGTH];
    char tempLine[MAX_LINE_LENGTH];
    ssize_t byteData;
    int lineNumber = 0;
    int lineStartIndex = 0;
    int index = 0;
    int wordLen = 0;
    int studentCount = 0;

    fd = open(fileName, O_RDONLY);

    if (fd == -1){
        logActions("File cannot openned!", argv, argc);
        perror("open");
        return -1;
    }

    while ((byteData = read(fd, buffer, 1)) > 0) {
        if (buffer[0] == '\n') {
            if ((lineNumber >= startEntry && lineNumber < endEntry) || endEntry == -1) {
                tempLine[lineStartIndex] = '\0'; 
                
                sscanf(tempLine, "%[^,], %[^\n]", students[studentCount].name, students[studentCount].grade);
                studentCount++;
            }
            lineNumber++;
            lineStartIndex = 0; 
            wordLen = 0;
        } else {
            if (lineStartIndex < MAX_LINE_LENGTH - 1) {
                tempLine[lineStartIndex++] = buffer[0];
            } else {
                buffer[lineStartIndex] = '\0';
                printf("%s (Line truncated)\n", buffer);

                while ((byteData = read(fd, buffer, 1)) > 0 && buffer[0] != '\n');
                lineStartIndex = 0; 
            }
        }
        wordLen++;
        memset(buffer, 0, sizeof(buffer));
    }

    if(byteData == -1){
        logActions("Read operations cannot done!", argv, argc);
        perror("read");
    }
    close(fd);
    return studentCount;
}

void argvCleaner(char *argv[], int argc){
    for(int i = 0; i <= argc; i++)
        free(argv[i]);
}