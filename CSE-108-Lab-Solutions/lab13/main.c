#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node{
    char bookname[30];
    char author[30];
    int year;
    struct node *next;
}; 
/*
    function name : insert_node
    it function insert nodes to linked lists 
    return node
*/
struct node * insert_node(struct node * head, char *bookname, char *author, int year){
        head->next = (struct node*) malloc(sizeof(struct node));
        strcpy(head->bookname,bookname);
        strcpy(head->author,author);
        head->year = year;
        
        return head->next;
}

/*
    function name : readFile
    it function read all info in file
    return node
*/
struct node * read_file(){
    struct node * head = (struct node *) malloc(sizeof(struct node));

    struct node * oHead;
    struct node * orHead;
    struct node *tHead;

    tHead = head;
    orHead = head;

    oHead = head;

    FILE *file = fopen("source.txt","r");
    char bookname[30],author[30];
    int year;

    for(int a = 0; a < 5 ; a++){
        fscanf(file,"%s", bookname);
        fscanf(file,"%s", author);
        fscanf(file,"%d", &year);
        head = insert_node(head,bookname,author,year);
    }
        
    for(int a = 0; a < 5; a++){
        orHead = tHead;
        for(int b = a ; b < 4; b++){
            if (orHead->year < orHead->next->year){
                /* yıl */
                int temp = orHead->year;
                orHead->year= orHead->next->year;
                orHead->next->year = temp;
                /* kitap ismi */
                char tempname[30] = {0};
                strcpy(tempname,orHead->bookname);
                strcpy(orHead->bookname,orHead->next->bookname);
                strcpy(orHead->next->bookname,tempname);
                /* yazar ismi */

                strcpy(tempname,orHead->author);
                strcpy(orHead->author,orHead->next->author);
                strcpy(orHead->next->author,tempname);

            }
            orHead = orHead->next;
        }
    }

    return oHead;
}
/*
    function name : printNodes
    it function print all nodes in the linked list
    return none
*/
void print_nodes(struct node * head){
    while(head){
        printf("%15s %15s %10d\n",head->bookname,head->author,head->year);
        head = head->next;
    }
}
/*
    function name : deletenode
    it function delete node which is defined or given by user
    return node
*/

struct node * delete_node(struct node * head, char * bookname){
    struct node * ohead;
    int control = 0;
    ohead = head;
    if(strcmp(head->bookname, bookname) == 0){
            head = head->next;
            return head;
    }
    while(head){
        if(!(head->next)){
            if(control == 0) printf("none\n");
            return ohead;
        }
        if(strcmp(head->next->bookname, bookname) == 0){
            head->next = head->next->next;
            return ohead;
            control = 1;
        }
        head = head->next;
    }
    return ohead;
}

int main(){
    
    struct node * head = (struct node *) malloc(sizeof(struct node)*1);
    

    head = read_file();

    print_nodes(head);

    char name[] = "MrsDalloway";
    printf("Which book do you want to delete ? ");
    scanf("%s",name);
    

    head = delete_node(head,name);

    print_nodes(head);

    return 0;
}