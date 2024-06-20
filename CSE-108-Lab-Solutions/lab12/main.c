#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int * array;
    int currentsize;
} dynamic_array;
/*
    function name : resize array
    it resizez array by increasing one
    return array2
*/
int *resize_array(int *array, int currentsize){

    int * t = (int *) calloc( currentsize + 1 , sizeof(dynamic_array) );

    for(int a = 0; a < currentsize +1;a++){
        t[a] = array[a];
    }   

    return t;
}
/*
    function name : print_array
    it prints array
    return none
*/
void print_array(dynamic_array arr){
    printf("Data in Array\n****************************\n");
    for(int a = 0; a < arr.currentsize ;a++){
        printf("%d\n",arr.array[a]);
    }
}
/*
    function name : remove_data
    it does remove data in dynamic array
    return dynamic_array
*/
dynamic_array remove_data(dynamic_array arr, int number){
    int l = arr.currentsize;
    int c = 0;

    int *t = (int *) calloc( l - 1, sizeof(int) ); 

    for(int a = 0; a < l - 1; a++){
        if(c == 1 || arr.array[a] == number){
            t[a] = arr.array[a+1];
            c = 1;
        }
        else 
            t[a] = arr.array[a];
    }

    arr.array = (int *) calloc( l - 1, sizeof(int) ); 
    
    for(int a = 0; a < l - 1; a++){
        arr.array[a] = t[a];
    }

    return arr;
}
/*
    function name : dynamic_array_read_from_file
    it does read txt and fill by numbers in the txt file by incereasing arr in one each element
    return dynamic_array
*/

dynamic_array dynamic_array_read_from_file(char *filename, dynamic_array arr){

    FILE *f = fopen(filename, "r");
    int temp;
    int c = 1;
    int test = 0;

    arr.array = (int *) calloc( 1 , sizeof(dynamic_array) );
    arr.array[0] = 1;

    printf("Data in Source file\n****************************\n");

    while(fscanf(f,"%d",&temp) == 1){
        printf("%d\n",temp);
        arr.array = resize_array(arr.array, c);
        c++;
        //scanf("%d",&test);
    }

    fclose(f);
    
    c = 0;
    int t = 1;
    f = fopen(filename, "r");

    while(fscanf(f,"%d",&temp) == 1){
        arr.array[c] = temp;
        (c++);
    }
    arr.currentsize = c;

    fclose(f);

    return arr;
}

int main(){

    dynamic_array arr;


    int choice = 2;

    while(choice != 5){
        printf("1- Read File\n");
        printf("2- Remove Data\n");
        printf("3- Print Array\n");
        printf("5- Exit\n");
        scanf("%d",&choice);
        if(choice == 1){    
            arr = dynamic_array_read_from_file("arr.txt",arr);
        }
        else if(choice == 2){
            int temp;

            printf("Enter you wanted to remove number : ");
            scanf("%d",&temp);

            arr = remove_data(arr,temp);
            arr.currentsize--;
            printf("Array After Deletion\n******************\n");
            print_array(arr);
        }

        else if(choice == 3){
            print_array(arr);
        }

    }
    return 0;   
}