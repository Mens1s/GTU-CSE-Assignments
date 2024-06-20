#include <stdio.h>
#include <string.h>
/*
    function name : reverse
    it is revereses one array after that it control
    reversed array with original array
    if there is a mistake it gives not polindrome !
    return none;
*/
void reverse(char str[], char reversed[], int i){
    int a = strlen(str) - 1 - i;
    reversed[i] = str[a];
    if(strlen(reversed) == i) {

        return;
    }
    else reverse(str,reversed, i + 1 );
}
/*
    function name : merge
    it is merges two array element
    and
    compare them each other 
    return none
*/
void merge(int arr[], int p, int q, int r){
    int n1 = q - p +1;
    int n2 = r -q;

    int L[n1], M[n2];

    for(int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    
    for(int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    int i, j ,k;
    i=0;
    j=0;
    k=p;

    while(i < n1 && j < n2){
        if(L[i] <= M[j]){
            arr[k] = L[i];
            i++;
        }else{
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while(j < n2){
        arr[k] = M[j];
        j++;
        k++;
    }
}
/*
    function name : mergeSort
    it is controls and merges array with recursively
    return none
*/
void mergeSort(int array[], int l, int e)
{
	if (l >= e)
		return; 

	int mid = l + (e - l) / 2;
	mergeSort(array, l, mid);
	mergeSort(array, mid + 1, e);
	merge(array, l, mid, e);
}

/*
    function name : main
    it does main function and calls each them and gives to user

    return none
*/
int main(){

    printf("First Task : \n");

    char arr[10] = "159";
    char reveresed[100] = {0};

    printf("Enter string : ");
    gets(arr);
    reverse(arr, reveresed, 0);

    
    printf("Original string : %s\n",arr);
    printf("Reversed string : %s\n",reveresed);

    if (strcmp(arr, reveresed)) printf("It is not polindrome\n");
    else printf("It is polindrome\n");
    

    printf("Second Task : \n");
    int array[6] = {6,5,12,10,9,1};

    for(int a = 0; a < 6 ; a++){
        printf("Enter %d. element: ",a+1);
        scanf("%d",&array[a]);
    }

    printf("\nUnordered arr : \n");
    for(int a = 0; a < 6; a++) printf("%d ",array[a]);

    mergeSort(array,0,5);
    printf("\nOrdered arr : \n");

    for(int a = 0; a < 6; a++) printf("%d ",array[a]);
    printf("\n");

    return 0;
}