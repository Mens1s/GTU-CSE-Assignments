#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Book{
    char title[50];
    char author[50];
    char subject[50];
    int year;
};

struct Books{
    struct Book book;
};


int main(){
    int choice = 0;
    int x = 2;
    int c = 0;
    
    struct Books *arr = (struct Books *) malloc( 100 * sizeof(int) );
/* This loop works until user not enter 3 */
    while(choice != 3){
        printf("MENU\n");
        printf("\t\t1. Add New Book\n");
        printf("\t\t2. List Books\n");
        printf("\t\t3. Exit\n");

        printf("\n\nChoose = ");
        int d = scanf("%d",&choice);
        
        if(choice == 1){
            char title[50] = {0}, author[50] = {0}, subject[50] = {0};
            int year;
            char t;
            struct Books *temp = (struct Books *) calloc( x , sizeof(int) );
            //arr = (struct Books *) calloc(x , sizeof(int));
            for(int a  = 0; a < c; a++){
                    strcpy(temp[a].book.title, arr[a].book.title);
                    strcpy(temp[a].book.author, arr[a].book.author);
                    strcpy(temp[a].book.subject, arr[a].book.subject);
                    
                    temp[a].book.year = arr[a].book.year ;                            
                
            }
            arr = (struct Books *) calloc( x , sizeof(int) );
            arr = temp;
            x++;

            printf("Please enter book title = ");

            fflush(stdin);
            scanf(" %[^\n]%*c",title);

            printf("Please enter book author = ");
            fflush(stdin);

            scanf(" %[^\n]%*c",author);

            printf("Please enter book subject = ");
            fflush(stdin);

            scanf(" %[^\n]%*c",subject);

            printf("Please enter book year = ");
            scanf("%d",&year);

            strcpy(arr[c].book.title ,title);
            strcpy(arr[c].book.author ,author);
            strcpy(arr[c].book.subject ,subject);
            arr[c].book.year = year;

            c++;

        }/* This loop works until user not enter 6 */
        else if(choice == 2){
            int s_c = 0;
            while(s_c != 6){

                printf("**************\n");
                printf("1. Get by  Title\n");
                printf("2. Get by  Author\n");
                printf("3. Get by  Subject\n");
                printf("4. Sorted List by year (DESC)\n");
                printf("5. List All Books\n");
                printf("6. Exit SUBMENU\n");
                printf("**************\n");
                scanf("%d",&s_c);


                if(s_c == 1){/* This loop finds title and print it */
                    char t[50] = {0};
                    int flag = 0;

                    printf("Enter a books of title = ");
                    fflush(stdin);
                    scanf(" %[^\n]%*c",t);

                    for(int a = 0; a < c ; a++){
                        if(strcmp(t, arr[a].book.title) == 0){
                            flag = 1;
                            printf("**************\n");
                            printf("BOOK FOUND\n");
                            printf("Title => %s\n",arr[a].book.title);
                            printf("Author => %s\n",arr[a].book.author);
                            printf("Subject => %s\n",arr[a].book.subject);
                            printf("Year => %d\n",arr[a].book.year);
                        }
                    }
                    if(flag == 0) printf("Book not found!\n");
                }/* This loop finds author and print it */
                else if(s_c == 2){
                    char t[50]= {0};
                    int flag = 0;

                    printf("Enter a books author = ");
                    fflush(stdin);
                    scanf(" %[^\n]%*c",t);

                    for(int a = 0; a < c ; a++){
                        if(strcmp(t, arr[a].book.author)  == 0){
                            flag = 1;
                            printf("**************\n");
                            printf("BOOK FOUND\n");
                            printf("Title => %s\n",arr[a].book.title);
                            printf("Author => %s\n",arr[a].book.author);
                            printf("Subject => %s\n",arr[a].book.subject);
                            printf("Year => %d\n",arr[a].book.year);
                            printf("**************\n");
                        }
                    }
                    if(flag == 0) printf("Book not found!\n");
                }/* This loop finds subjects and print it */
                else if(s_c == 3){
                    char t[50] = {0};
                    int flag = 0;

                    printf("Enter a books subject = ");
                    fflush(stdin);
                    scanf(" %[^\n]%*c",t);

                    for(int a = 0; a < c; a++){
                        if(strcmp(t, arr[a].book.subject) == 0){
                            flag = 1;
                            printf("**************\n");
                            printf("BOOK FOUND\n");
                            printf("Title => %s\n",arr[a].book.title);
                            printf("Author => %s\n",arr[a].book.author);
                            printf("Subject => %s\n",arr[a].book.subject);
                            printf("Year => %d\n",arr[a].book.year);
                            printf("**************\n");
                        }
                    }
                    if(flag == 0) printf("Book not found!\n");
                }/* This loop sorts array */
                else if(s_c == 4){
                    char t[50] = {0};
                    int flag = 0;
                    struct Books *temp = (struct Books *) calloc(1 , sizeof(int));

                    
                    
                    for(int a  = 0; a < c; a++){
                        for(int b = 0; b < c-1; b++){
                            if(arr[b].book.year > arr[b+1].book.year){

                                //temp
                                strcpy(temp[0].book.title,arr[b].book.title);
                                strcpy(temp[0].book.author,arr[b].book.author);
                                strcpy(temp[0].book.subject,arr[b].book.subject);
                                temp[0].book.year = arr[b].book.year;
                                //end of temp
                                //
                                strcpy(arr[b].book.title,arr[b+1].book.title);
                                strcpy(arr[b].book.author,arr[b+1].book.author);
                                strcpy(arr[b].book.subject,arr[b+1].book.subject);
                                arr[b].book.year = arr[b+1].book.year;
                                //
                                strcpy(arr[b+1].book.title,temp[0].book.title);
                                strcpy(arr[b+1].book.author,temp[0].book.author);
                                strcpy(arr[b+1].book.subject,temp[0].book.subject);
                                arr[b+1].book.year = temp[0].book.year;

                            }
                        }
                    }
                    
                    for(int a = 0; a < c; a++){
                        printf("**************\n");
                        printf("Title => %s\n",arr[a].book.title);
                        printf("Author => %s\n",arr[a].book.author);
                        printf("Subject => %s\n",arr[a].book.subject);
                        printf("Year => %d\n",arr[a].book.year);
                        
                    }
                }
                else if(s_c == 5){
                    for(int a = 0; a < c; a++){
                            printf("**************\n");
                            printf("%d. BOOK\n",a+1);
                            printf("Title => %s\n",arr[a].book.title);
                            printf("Author => %s\n",arr[a].book.author);
                            printf("Subject => %s\n",arr[a].book.subject);
                            printf("Year => %d\n",arr[a].book.year);
                            printf("**************\n");
                    }
                }
                else if(s_c == 6);
                else{
                    printf("Try Again!\n");
                }
            }
            
        }
        else if(choice == 3);
        else{
            printf("TRY AGAIN\n");
        }
    /*
        printf("Title => %s\n",arr[c-1].book.title);
        printf("Author => %s\n",arr[c-1].book.author);
        printf("Subject => %s\n",arr[c-1].book.subject);
        printf("Year => %d\n",arr[c-1].book.year);
        printf("%d \n", c-1);
    */
    }

    return 0;
}
