#include "stdio.h"
#include "stdlib.h"

char a = 'O';
char * grid;
int * oldBombLocations;
int oldBombLocationsSize;
int * newBombLocations;
int row;
int column;
int step;

void getSizes(){
    printf("Enter the number of rows: ");
    scanf("%d", &row);
    printf("Enter the number of columns: ");
    scanf("%d", &column);
    printf("Enter the number of step: ");
    scanf("%d", &step);
    printf("The grid size is %d x %d\n", row, column); 
}

void fillGrid(){
    grid = (char *)malloc(row * column * sizeof(char));

    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("Enter the value for grid[%d][%d]: ", i, j);
            scanf(" %c", &grid[i * column + j]);
        }
    }
}

void printGrid(){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("%c ", grid[i * column + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void getBombOlderLocation(){
    int i, j;
    // FREE OLDBOMBLOCATIONS
    oldBombLocationsSize = 0;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            if(grid[i * column + j] == a){
                oldBombLocationsSize++;
            }
        }
    }
    oldBombLocations = (int *)malloc(oldBombLocationsSize * sizeof(int));
    oldBombLocationsSize = 0;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            if(grid[i * column + j] == a){
                oldBombLocations[oldBombLocationsSize] = i * column + j;
                oldBombLocationsSize++;
            }
        }
    }
}

void fillGridWithBombs(){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            grid[i * column + j] = a;
        }
    }
}

void executeOlderBombs(){
    // i want to execute older bombs only
    int i, j;
    for (i = 0; i < oldBombLocationsSize; i++) {
        int bombLocation = oldBombLocations[i];
        int bombRow = bombLocation / column;
        int bombCol = bombLocation % column;
        
        // Explode the bomb
        grid[bombLocation] = '.'; // Empty space
        
        int newRow = bombRow + 0;
        int newCol = bombCol + -1;
        if (newRow >= 0 && newRow < row && newCol >= 0 && newCol < column) {
            grid[newRow * column + newCol] = '.'; // Empty space
        }

        newRow = bombRow + 0;
        newCol = bombCol + 1;
        if (newRow >= 0 && newRow < row && newCol >= 0 && newCol < column) {
            grid[newRow * column + newCol] = '.'; // Empty space
        }

        newRow = bombRow + -1;
        newCol = bombCol + 0;
        if (newRow >= 0 && newRow < row && newCol >= 0 && newCol < column) {
            grid[newRow * column + newCol] = '.'; // Empty space
        }

        newRow = bombRow + 1;
        newCol = bombCol + 0;
        if (newRow >= 0 && newRow < row && newCol >= 0 && newCol < column) {
            grid[newRow * column + newCol] = '.'; // Empty space
        }
    }
}

void BomberMan(){
 
    step--;
    step %= 4;
    step += 4;


    while(step != 0){
       
        getBombOlderLocation();
        
        // Bomberman places bomb second second
        fillGridWithBombs();
        step -= 1;
        if(step == 0){
            break;
        }
        executeOlderBombs();

        step -= 1;
    }
    
}

int main()
{
    getSizes();
    fillGrid();
    printGrid();
    
    if(step != 1) {
        step = step % 4;
        BomberMan();
    }
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("%c", grid[i * column + j]);
        }
        printf("\n");
    }
    return 0;
}


