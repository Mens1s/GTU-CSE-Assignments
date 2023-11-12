#include "tetromino.hpp"
#include <chrono>
#include <thread>

Tetromino::Tetromino(){
    befMid = 0;
    shape = new MVector[4];
    for (int a = 0; a < 4; a++)
        for( int b  = 0 ; b < 4; b++)
            shape[a].push_back(' ');
    _height = 4;
    _len = 4;
}

// Constructor
// filling MVector with tetrominos
Tetromino::Tetromino(tetromino t){
    befMid = 0;

    shape = new MVector[4];
    for (int a = 0; a < 4; a++)
        for( int b  = 0 ; b < 4; b++)
            shape[a].push_back(' ');

    if( t == tetromino::I){
        for( int b  = 0 ; b < 4; b++)
            shape[3].edit(b,'I');
    }
    else if( t == tetromino::O){
        shape[2].edit(0,'O');
        shape[2].edit(1,'O');
        shape[3].edit(0,'O');
        shape[3].edit(1,'O');
    }
    else if( t == tetromino::T){
        shape[2].edit(0,'T');
        shape[2].edit(1,'T');
        shape[2].edit(2,'T');
        shape[3].edit(1,'T');
    }
    else if( t == tetromino::J){
        shape[1].edit(1,'J');
        shape[2].edit(1,'J');
        shape[3].edit(0,'J');
        shape[3].edit(1,'J');
    }
    else if( t == tetromino::L){
        shape[1].edit(0,'L');
        shape[2].edit(0,'L');
        shape[3].edit(0,'L');
        shape[3].edit(1,'L');
    }
    else if( t == tetromino::S){
        shape[2].edit(1,'S');
        shape[2].edit(2,'S');
        shape[3].edit(0,'S');
        shape[3].edit(1,'S');
    }
    else{
        shape[2].edit(0,'Z');
        shape[2].edit(1,'Z');
        shape[3].edit(1,'Z');
        shape[3].edit(2,'Z');
    }
    _height = 4;
    _len = 4;
}

// expending game area dynamically
void Tetromino::expendGameArea(int height, int len){
    _height = height;
    _len = len;

    shape = new MVector[height];
    for (int a = 0; a < height; a++)
        for( int b  = 0 ; b < len; b++)
            shape[a].push_back(' ');
}

// printing tetrominos
void Tetromino::print(){
    string stuff(_len+2, '*');
    cout<< "\033[0;34m" <<stuff<< "\033[0m" << endl;  
    for(int i = 0; i< _height; i++){
        cout << "\033[0;34m" <<"*"<< "\033[0m";
        for(int a = 0; a < _len; a++){
            cout << (shape[i].get(a));
        }cout << "\033[0;34m" <<"*"<< "\033[0m"<<endl;
    }
    cout<< "\033[0;34m" <<stuff<< "\033[0m" << endl;  
}

// rotatate func
void Tetromino::rotate(int rotLeft){
    if(rotLeft == 1)        
        rotateLeft();
    else
        for(int i =  0; i < 3 ; i++)
            rotateLeft();
}

// rotatate func
void Tetromino::rotateLeft(){
    for(int i = 0; i < 2; i++) {
        for(int j = i; j < 3 - i; j++) {
            char temp = Tetromino::shape[i][j];
            Tetromino::shape[i][j] = Tetromino::shape[3-j][i];
            Tetromino::shape[3-j][i] = Tetromino::shape[3-i][3-j];
            Tetromino::shape[3-i][3-j] = Tetromino::shape[j][3-i];
            Tetromino::shape[j][3-i] = temp;
        }
    }
}

// animate function
// if rightMove equals -1 this means that that is a print only animate dont change anything
// otherwise animate function and add this to game_area
void Tetromino::animate(Tetromino & userTetromino, int rightMove){
    if(rightMove == -1){
        check = 0;
        printPart(userTetromino,0,0);
    }
    else{
        int count = 1;
        check = 1;
        printPart(userTetromino, 0, 0);
        
        while(printPart(userTetromino, count, rightMove)) count++;
        
        *this += userTetromino;
        print();
    }
}   

// prints particallry if it is not triggered with game area
bool Tetromino::printPart(Tetromino & userTetromino, int step, int rightMove){

    rightMove += befMid;

    int * arr = userTetromino.getErasedShapeLoc();
    int left = arr[0], right = arr[1], up = arr[2], down = arr[3];

    char ** userShape = userTetromino.getShape();

    while(left + rightMove >= _len){
        cout << "Big number entered for this game area size! DEFETEAD! ";
        exit(0);
    }
    while(rightMove < 0){
        cout << "Small number entered for this game area size! DEFETEAD! ";
        exit(0);
    }
    // control len
    if(right - left > _len) return false;
    if(down - up > _height) return false;
    if(step >= _height-(down-up)) return false;

    // checking isTrigger to gameArea
    if(!isTrigger(userTetromino,  step,  rightMove)){
        if(step==0) {
            if( check == 1 ){
                cout << "Tetromino blow upp! DEFETEAD! ";
                exit(0);
            }
            else{
                bool f = true;

                while(!isTrigger(userTetromino,  step,  ++rightMove))
                    f = false;   

                if(f){
                    exit(0);
                    return false;
                }
            
            }   
        }   
        // bestPosY = step;
        else return false;
    }
    // printing instant tetromino
    string stuff(_len+2, '*');
    
    int goUp = up - step;
    int goLeft = left-rightMove;
    
    if(check == 0){
        int mid = (_len / 2) - ((right-left)/2);
        goLeft = -mid;
        if(goLeft >= _len + (right-left)) goLeft = 0;
        befMid = mid;
    }

    cout<< "\033[0;34m" <<stuff<< "\033[0m" << endl; 

    bool flag = true;
    
    for(int a = goUp; a < _height+goUp && flag ; a++){
        cout << "\033[0;34m" <<"*"<< "\033[0m";
        for(int b = goLeft ; b < _len+goLeft ; b++){
            if(b >= left && b<=right && a >= up && a<=down){
                
                if(userShape[a][b] != ' ') {
                    if(shape[a-goUp][b-goLeft] != ' '){
                        bestPosY = a-goUp;
                        flag = false;
                    }
                    else{
                        bestPosY = a-goUp;
                        cout << userShape[a][b];
                    }
                }    
                else
                    cout << shape[a-goUp][b-goLeft];
            }
            else
                cout << shape[a-goUp][b-goLeft];
                
        }
        cout << "\033[0;34m" <<"*"<< "\033[0m" << endl;
    }
    bestPosX = rightMove;

    if(flag == false) bestPosY--;
    else bestPosY -= (down-up);

    cout<< "\033[0;34m" <<stuff<< "\033[0m" << endl; 

    
    this_thread::sleep_for(std::chrono::milliseconds(50));
    if(check != 0)cout << "\033[2J" << endl;
    
    delete [] arr;

   
    if(userShape != nullptr){
        for(int a = 0; a < userTetromino.getHei() ;a++)
            delete [] userShape[a];
        delete [] userShape;
    }

    return true;
}

// controlling userTetromino comes with gameArea and returning false or true
bool Tetromino::isTrigger(Tetromino & userTetromino, int step, int rightMove){

    
    int * arr = userTetromino.getErasedShapeLoc();
    int left = arr[0], right = arr[1], up = arr[2], down = arr[3];
    char ** userShape = userTetromino.getShape();

    int goUp = up - step;

    int goLeft = left-rightMove;

    rightMove += befMid;
    for(int a = goUp; a < _height+goUp ; a++)
        for(int b = goLeft ; b < _len+goLeft ; b++)
            if(b >= left && b<=right && a >= up && a<=down)
                if(userShape[a][b] != ' ') 
                    if(shape[a-goUp][b-goLeft] != ' '){
                        delete [] arr;
                        if(userShape != nullptr){
                            for(int a = 0; a < userTetromino.getHei() ;a++)
                                delete [] userShape[a];
                            delete [] userShape;
                        }
                        return false;
                    }
                        
    delete [] arr;
    if(userShape != nullptr){
        for(int a = 0; a < userTetromino.getHei() ;a++)
            delete [] userShape[a];
        delete [] userShape;
    }
    return true;
}

// get erased shape and returnign start, end points
int * Tetromino::getErasedShapeLoc(){
    newShape = new char*[4];

    for (int a = 0; a < 4; a++)
        newShape[a] = new char[_len];

    for(int a = 0; a < 4; a++)
        for(int b = 0; b < _len; b++)
            newShape[a][b] = shape[a].get(b);

    int left = 5, right = 0, up = 5 ,down = 0; 

    for(int a = 0; a < 4; a++){
        for(int b = 0; b < _len; b++){
            if(newShape[a][b] != ' '){
                if(left > b) left = b;
                if(right < b) right = b;
                if(up > a) up = a;
                if(down < a) down = a;
            }
        }      
    }

    int * arr = new int[4];
    arr[0] =  left;
    arr[1] =  right;
    arr[2] =  up;
    arr[3] =  down;
    return arr;
}

// adds tetrominos
Tetromino Tetromino::operator +=(Tetromino &userShape){
    char ** userSh = userShape.getShape();
    int * arr = userShape.getErasedShapeLoc(); // that is not const...

    int left = arr[0], right = arr[1], up = arr[2], down = arr[3];

    for(int a = bestPosY; a <= bestPosY+(down-up); a++){
        for(int b = bestPosX; b <= bestPosX+(right-left); b++){
            if(shape[a][b] == ' ')
                shape[a][b] = userSh[a-bestPosY+up][b-bestPosX+left];
        }
    }
    delete [] arr;

    return *this;
}
