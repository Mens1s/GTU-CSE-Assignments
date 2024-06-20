#include "game.h"

// Game class constructor || function setUserTetrominos sets tetrominos too
Game::Game(int count, Tetromino * tetrominos){
    setTCount(count);
    setOrder(0);

    setUserTetrominos(tetrominos);

    startGame();
}
// Program tries to find best rotation of tetromino to fit game area
int Game::findBestRotation(Tetromino & newTetromino){
    int left = 0;
    int maxTrigger = 0;
    int now = 0;
    // checking last tetromino has empty space
    bool isDownEmpty = (this->gameArea.getShape()[3][this->gameArea.getShape()[3].size() - 1] == ' ');
    bool isDownEmptyTwice = (this->gameArea.getShape()[3][this->gameArea.getShape()[3].size() - 2] == ' ');

    bool isUpEmpty = (this->gameArea.getShape()[2][this->gameArea.getShape()[2].size() - 1] == ' ');
    // tries to find best rotation with rotate instant tetromino
    for(int rotateCount = 0 ; rotateCount < 4; rotateCount++){
        vector<vector<char>> shape = newTetromino.getShape();

        if(isDownEmpty && isUpEmpty){
            left = 1;
            if(shape[3][0] != ' ' && shape[2][0] != ' ' && shape[1][0] == ' '){
                now = 3;
                break;
            }
            else if(shape[3][0] != ' ' && shape[2][0] == ' ')
                now = 1;
        }
        else if(isDownEmpty){
            if(isDownEmptyTwice){
                if(shape[3][0] != ' ' && shape[2][0] == ' ' && shape[3][1] != ' ' && shape[2][1] == ' ' ){
                    left = 2;
                    now = 2;
                    break;
                }
            }
            if(shape[3][0] != ' ' && shape[2][0] == ' '){
                left = 1;
                now = 2;
                break;
            }
            else if(shape[3][0] != ' ' && shape[2][0] != ' '){
                now = 1;
                if (rotateCount == 3)
                    break;
            }
            else if(shape[3][0] == ' ' && shape[3][1] != ' ')
                now = 1;
            
        }
        else if(isUpEmpty){
            
            if(shape[3][0] == ' ' && shape[3][1] != ' '){
                left = 1;
                now = 2;
                break;
            }
            else
                now = 1;
            
        }
        else{
            if(shape[3][0] != ' ' && shape[2][0] != ' '){
                now = 2;
                break;
            }
            else
                now = 1;
        }
        
        newTetromino.rotate(true);
    }
    // returing can it move to left and if it how many step is it
    return left;
}
// Instant best fit tetromino added the game area 
void Game::addTetro(){
    int orderIDX = getOrder();
    int leftMove = findBestRotation(this->userTetrominos[orderIDX]);

    expendGameArea();
    
    for(int i = 0 ; i < 4; i++ )
        for(int j = 0; j < 4 ; j++)
            this->gameArea.setShape((this->userTetrominos[orderIDX].getShape()),i,this->gameArea.getShape()[0].size()+j-4-leftMove,j);
   

    this->gameArea.eraseRight();

    setOrder(orderIDX+1);
}
// startGame starts game and put first element using best element depends on counting of touching left side
void Game::startGame(){
    // First element add checking of touch count of left side
    for(int a = 0; a < 4; a++ ){
        vector<vector<char>> shape = this->userTetrominos->getShape();
        if (shape[3][0] != ' ' && shape[2][0] != ' ')
            break;
        this->userTetrominos->rotate(false);
    }
    // checking z ... Z is a special case
    if(this->userTetrominos[0].getShape()[2][0] == 'Z')
        this->userTetrominos->rotate(true);
    // printing first user element to gameArea
    for(int i = 0 ; i < 4; i++ )
        for(int j = 0; j < 4 ; j++)
            this->gameArea.setShape((this->userTetrominos[0].getShape()),i,j,j);
 
    // erase right bcs of we don't need any right element in game area
    this->gameArea.eraseRight();
    setOrder(getOrder()+1);

    while (getOrder() < getTCount())
        addTetro();
  
}
// function calls expand game area with using tetromino class we can use inheritence too but it is not allowed yet!  
void Game::expendGameArea(){
    this->gameArea.expendGameArea();
}  
// This function prints game area
void Game::printGameArea(){
    for(auto ts : (this->gameArea.getShape())){
        for(auto t : ts){          
            cout << t;
        }
        cout << endl;
    }
}
     