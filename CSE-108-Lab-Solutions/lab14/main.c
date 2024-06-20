#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMOFITEMS 2
#define FILENAME "LAB11_data.bin"


typedef struct {
	int id;
	char name[100];
	int price;

}item_t;

typedef union{
	char info[100];
}contactInfo_t;

typedef struct {
	int id;
	char email[100];
	char phone[100];
	
}itemWithCont_t;
/* getting elements */
item_t * items;

item_t *getItems(item_t *items, int size){
	printf("ID => ");
	scanf("%d",&items[size].id);

	char temp[100];
	printf("Name => ");
	scanf("%s",temp);
	strcpy(items[size].name,temp);

	printf("PRICE => ");
	scanf("%d",&items[size].price);

	printf("\n\n\n");
}
/* printing elements */
void printItem(item_t item){	
		printf("ID : %d \n", item.id); // prints a series of bytes
		printf("Name : %s\n", item.name);
		printf("Price : %d\n", item.price);
}

int writeItems(char filename[], item_t *items, int size){
/* write items to binary file 0*/
	
	FILE *file;

	file = fopen(FILENAME,"wb");  // w for write, b for binary

	for(int a = 0; a < NUMOFITEMS; a++){
		fwrite(items,sizeof(items),1,file);
		items++;
	}
	fclose(file);
}	
/*
    void : readanitem
	file added form asd
*/
item_t readAnItem(FILE *file){
	item_t * temp = (item_t *) malloc(sizeof(item_t));

	FILE *ptr;

	ptr = fopen(FILENAME,"rb");  // r for read, b for binary

	fread(temp,sizeof(temp),1,ptr); // read temp bytes to our buffer


	for(int i = 0; i<NUMOFITEMS; i++){
		printf("ID : %d \n", temp->id); // prints a series of bytes
		printf("Name : %s \n", temp->name);
		printf("Price : %d \n\n\n", temp->price);
		fread(temp,sizeof(temp),1,ptr); // read temp bytes to our buffer
	}	
	fclose(ptr);
}
/*
    void : contactInfoAdder
	added contact info 
*/
void contactInfoAdder(item_t * items, int size, itemWithCont_t contacts[]){
	for(int a = 0; a < NUMOFITEMS; a++){
		printf("Enter the phone for %s => ",items[a].name);
		scanf("%s",contacts[a].phone);

		printf("Enter a mail for %s => ",items[a].name);
		scanf("%s",contacts[a].email);
	}	
}
/*
    void : printItemWithContactInfo
	printing
*/
void printItemWithContactInfo(itemWithCont_t itemsWithCont[], item_t * items){
	for(int a = 0; a < NUMOFITEMS; a++){
		printf("ID : %d\n",items[a].id);
		printf("Name : %s\n",items[a].name);
		printf("Price : %d\n",items[a].price);
		printf("Phone number : %s\n",itemsWithCont[a].phone);
		printf("Mail : %s\n\n",itemsWithCont[a].email);
	}
}

int main(){

	FILE *file = fopen(FILENAME,"");

	items = (item_t *) malloc(sizeof(item_t) * NUMOFITEMS);

	int currentElement = 0;

	while(currentElement < NUMOFITEMS){
		getItems(items, currentElement);
		currentElement++;
	}
	printf("DATA Succesfull addded \n\n\n");
	for(int a = 0; a < NUMOFITEMS; a++){
		printItem(items[a]);
	}

	printf("DATA Succesfull printed \n\n\n");

	writeItems(FILENAME,items,NUMOFITEMS);
	printf("Succesfully wrote data to file\n\n\n");

	printf("Reading from file:\n");

	readAnItem(file);

	itemWithCont_t contacts[NUMOFITEMS];
	contactInfoAdder(items, NUMOFITEMS, contacts);

	printf("Printing with contact iformation...\n");
	printItemWithContactInfo(contacts, items);
	
	return 0;
}
