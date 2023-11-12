#include "tetris.hpp"
#include "string.h"

Tetris::Tetris(int height, int len){
    _gameArea.expendGameArea(height, len);
    _height = height;
    _len = len;
}

void Tetris::add(Tetromino & userTetromino, int step){
    string s = "initialValue";

    while ( step > 0 && !(strspn( s.c_str(), "-0123456789" ) == s.size())) {
        cout << "\n\t\tEnter step count :  ";
        cin >> s; 
    } 

    if(step >= 0) step =  stoi(s);
    else if(step <= -2){// automatic part
                        // -10dan baslar
                        // OOOIISSSZ
        step += 10;
        int st[] = {0,2,4,6,7,3,1,0,4};

        if(step == 3 || step == 4) 
            userTetromino.rotate(1);
        
        step = st[step];
    }
    
    _gameArea.animate(userTetromino, step);

}

// This function prints game area
void const Tetris::draw(){
    string stuff(_len+2, '*');
    cout<< "\033[0;34m" <<stuff<< "\033[0m" << endl; 
    _gameArea.print();
    cout<< "\033[0;34m" <<stuff<< "\033[0m" << endl;  // _length+2
}
const Tetris Tetris::operator +(Tetromino &userShape){
    add(userShape, 1);
    return *this;
}
const Tetris Tetris::operator +=(Tetromino &userShape){
    
    add(userShape, 1);
    return *this;
}

