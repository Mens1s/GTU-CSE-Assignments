#include <stdio.h>
/* Showing elements in arr */
void display(int arr[3][3]){

    for(int a = 0; a < 3; a++){
        for(int b = 0;b < 3; b++){
            printf("%d\t",arr[a][b]);
        }printf("\n");
    }
}
/* getting elements from user */
void get_elements(int arr[], int n){
    printf("1D Array : \n");
    for(int i = 0; i < n; i++){
        
        int temp;
        scanf("%d",&temp);
        arr[i] = temp;
    }
}
/* getting elements from user for 3 by 3 arrays */
void get_elementsByThree(int matrix_one[3][3],int matrix_two[3][3]){

    printf("First Matrix :\n");
    /* Getting first matrix from user */
    for(int a = 0; a < 3; a++){
        for(int b = 0;b < 3; b++){
            int temp;
            scanf("%d",&temp);
            matrix_one[a][b] = temp;
        }
    }
    /* Getting second matrix from user */
    printf("Second Matrix :\n");
    for(int a = 0; a < 3; a++){
        for(int b = 0;b < 3; b++){
            int temp;
            scanf("%d",&temp);
            matrix_two[a][b] = temp;
        }
    }


}
/* Solutiın of part1 */
void part1(){
    int temp_arr[] = {-1, 1, -1};
    int arr[10];
    int new_arr[10];
    int counter = 0;
    get_elements(arr,10); /* Getting elemnts from user*/

    for(int i = 0; i < 8 ;i++){
        new_arr[counter] = arr[i]*temp_arr[0] + arr[i+1]*temp_arr[1] + arr[i+2]*temp_arr[2];
        counter++;
    }
    /* Printing new array */
    printf("Convolutional results : \n");
    for(int i = 0; i < 8;i++){
        printf("%d\t",new_arr[i]);
    }
}
/* Solution of part2 */
void part2(int matrix_one[3][3],int matrix_two[3][3]){


//1 * 5 +6 * 4 +7 * 7
    int resultArr[3][3];
    for(int a = 0; a < 3; a++){
        for(int b = 0;b < 3; b++){
            int temp_1 = 0;
            for(int c = 0; c < 3; c++){
                /* Matrix multiplaction */
                temp_1 += matrix_one[a][c] * matrix_two[c][b];
            }
            resultArr[a][b] = temp_1;
        }
    }
    printf("Multiplcation of matrix : \n\n");
    /* Displayin results */
    display(resultArr);
}

/* Solution of part3 */

void part3(int matrix_one[3][3],int matrix_two[3][3]){
    
    int temp_1 = 0;
    int temp_2 = 0;
 
    for(int a = 0; a < 3; a++){
        for(int b = 0;b < 3; b++){
            if(a == b) {
                /* Calculating of Tracing of matrix */
                temp_1 += matrix_one[a][b];
                temp_2 += matrix_two[a][b];    
            }
        }
    }
     /* Printing results */
    printf("\nTrace of first matrix\n");
    display(matrix_one);
    printf("\nTrace of matrix is : %d\n",temp_1);

    printf("\nTrace of secodnd matrix is \n");
    display(matrix_two);
    printf("\nTrace of matrix is : %d\n",temp_2);
    
}
void part4(int matrix_one[3][3],int matrix_two[3][3]){


    int temp_1[3][3];
    int temp_2[3][3];
 
    for(int a = 0; a < 3; a++){
        for(int b = 0;b < 3; b++){
            
            temp_1[b][a] = matrix_one[a][b];
            temp_2[b][a] = matrix_two[a][b];    
            
        }
    }

    /* Printing results */
    printf("\nTransporse of matrix is : \n");
    display(temp_1);
    
    printf("\nTransporse of matrix is : \n");
    display(temp_2);
}

/* -1 1 -1 */
int main(){
    int choice;
    int control = 0;
    int matrix_one[3][3];
    int matrix_two[3][3];
    do{
        printf("\nChoose the matrix operation\n");
        printf("-----------------------\n");
        printf("1. Convolutional Filter for 1D array\n");
        printf("2. Multiplication \n");
        printf("3. Trace\n");
        printf("4. Transpose\n");
        printf("5. Exit\n");

        scanf("%d",&choice);

        if(choice == 1) part1();
        else if(choice >= 2 && choice<=4){
            if(control == 0){
                get_elementsByThree(matrix_one, matrix_two);
                control = 1;
            }

            if(choice == 2) part2( matrix_one, matrix_two);
            else if(choice == 3) part3( matrix_one, matrix_two);
            else if(choice == 4) part4( matrix_one, matrix_two);
        }
        
        else if(choice == 5) printf("Exitting..\n");
        else printf("Please enter right type num!..\n");
    }while(choice != 5);
    

    return 0;
}