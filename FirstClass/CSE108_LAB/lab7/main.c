#include <stdio.h>
#include <string.h>
#define MAX_LEN 100
/*
    Funtion : alphabeticSort
    
    it does 
        sort words in a array

    maxLen = 100

    it uses pointer to save or return array
    return NONE

*/
void alphabeticSort(char words[][MAX_LEN], int n){

    int idx = 0;

    /* Sorting */
    for(int a = 0 ; a < n; a++){
        for(int b = 0 ; b < n-1 ; b++){
            int status = strcmp(words[b], words[b+1]);
            if (status > 0){
                char temp[100];
                strcpy(temp , words[b]);
                strcpy(words[b], words[b+1]);
                strcpy(words[b+1],temp);
            }
        }
    }
    
}
/*
    Funtion : generateTagParser
    
    it does 
            add specific characters to specific lenght of array

    maxLen = 100
    
    it uses pointer to save or return array
    return NONE

*/
void generateTagParser(char temp[], char arr[]){

    int a;
    /* if arr lenght less than 5 */
    if(strlen(arr)<5){
        temp[0] = '<';
        temp[1] = '<';
        for(a = 2; a < strlen(arr)+2; a++) temp[a] = arr[a-2];
        temp[a] = '>';
        temp[a+1] = '>';
        
        
    }
    /* if arr lenght bigger than 5 less than 10 */

    else if(strlen(arr)<10){
        temp[0] = '*';
        for(a = 1; a < strlen(arr)+1; a++) temp[a] = arr[a-1];
        temp[a] = '*';
    }
    /* if arr lenght more than 10 */

    else{
        temp[0] = '/';
        temp[1] = '+';
        for(a = 2; a < strlen(arr)+2; a++) temp[a] = arr[a-2];
        temp[a] = '+';
        temp[a+1] = '/';
    }

}

int main(){
    char words[100][MAX_LEN] = {0};
    int n;
    int choice=0;
    do{
        printf("1- PART1 \n2- PART2\n3- EXİT\nChoice : ");

        scanf("%d",&choice);
        if(choice == 1){
            printf("Enter the count of how many strings you will enter : ");
            scanf("%d",&n);
            /* Taking inputs */
            printf("Please enter strings : \n");

            for(int a = 0 ; a < n ; a++){
                scanf("%s",words[a]);
            }

            printf("Given array is : \n");

            for(int a = 0 ; a < n ; a++){
                printf("%d: %s\n",a,words[a]);
            }
            printf("\n\nSorted array is : \n");
            /* Doing part1 */
            alphabeticSort(words,n);
            for(int a = 0 ; a < n; a++){
                printf("%d: %s\n",a,words[a]);
            }
            printf("\n");
        }
        else if(choice == 2){
            char temp[MAX_LEN] = {0}, arr[MAX_LEN] = {0};

            printf("Enter a string : ");
            scanf("%s",arr);
            
            generateTagParser(temp,arr);
            /* Printing results */
            
            strcpy(arr,temp);
       
            printf("\n%s",arr);

            printf("\n");
        }
        else if(choice!=3) printf("Wrong Value!");
    }while(choice != 3);
    
    /*Question one solution*/
    
    /* Doing part2 */
    
    return 0;
}