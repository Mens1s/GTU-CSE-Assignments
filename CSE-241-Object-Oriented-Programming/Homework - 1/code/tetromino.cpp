#include <iostream>
#include "tetromino.h"

using namespace std;

// default constructor decleration
Tetromino::Tetromino(){
    Tetromino::shape ={{' ',' ',' ',' '},{' ',' ',' ',' '},{' ',' ',' ',' '},{' ',' ',' ',' '}};
}
// Constructor of class decleration 
Tetromino::Tetromino(tetromino t){ // shape creating for users choices
    if( static_cast<char>(t) == 'I')
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{' ',' ',' ',' '},{'I','I','I','I'}};
    else if( static_cast<char>(t) == 'O')
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{'O','O',' ',' '},{'O','O',' ',' '}};
    else if( static_cast<char>(t) == 'T')
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{'T','T','T',' '},{' ','T',' ',' '}};
    else if( static_cast<char>(t) == 'J')
        Tetromino::shape = {{' ',' ',' ',' '},{' ','J',' ',' '},{' ','J',' ',' '},{'J','J',' ',' '}};
    else if( static_cast<char>(t) == 'L')
        Tetromino::shape = {{' ',' ',' ',' '},{'L',' ',' ',' '},{'L',' ',' ',' '},{'L','L',' ',' '}};
    else if( static_cast<char>(t) == 'S')
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{' ','S','S',' '},{'S','S',' ',' '}};
    else
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{'Z','Z',' ',' '},{' ','Z','Z',' '}};
    
}
// Tetromino function prints in here. The algorithm is basic if there is a char print the char otherwise print space
void Tetromino::print() const{
    for(auto ts : Tetromino::shape){
        for(auto t : ts){
            cout << t;
        }
        cout << endl;
    }
}
// rotate Function rotate shape 90 degree or 270 degree depends on rotLeft boolean variable
void Tetromino::rotate(bool rotLeft){
    if(rotLeft)        
        rotateLeft();
    else
        for(int i =  0; i < 3 ; i++)
            rotateLeft();

    bool flag = true;
    // loop finds if there is a space under the tetromino
    for(int j = 0; j < 4 && flag; j++)
        if (Tetromino::shape[3][j] != ' ')
            flag = false;
    // if space found we erase space after and after again
    while (flag){

        for(int i = 3; i > 0; i--)
            for(int j = 0; j < 4; j++)
                Tetromino::shape[i][j] = Tetromino::shape[i-1][j];

        Tetromino::shape[0] = {' ',' ',' ',' '};
        
        flag = true;

        for(int j = 0; j < 4; j++)
            if (Tetromino::shape[3][j] != ' ')
                flag = false;   
    }   
    flag = true;
    // loop finds if there is a space left of the tetromino
    for(int j = 0; j < 4; j++)
        if(Tetromino::shape[j][0] != ' ')
            flag = false;
    
    // if space found we erase space after and after again
    while(flag){
        for(int i = 0; i < 4; i++)
            for(int j = 0 ; j < 3; j++)
                Tetromino::shape[i][j] = Tetromino::shape[i][j+1];
        
        for(int i = 0; i < 4 ;i++)
            Tetromino::shape[i][3] = ' ';
        
        for(int j = 0; j < 4; j++)
            if(Tetromino::shape[j][0] != ' ')
                flag = false;
    }

}
// Function ratates tetromino 90 degrees on each calles
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
// canFit function returnes this shape fittes or not my aim is only check newTetromino[3][0] is have a char or not
bool Tetromino::canFit(Tetromino newTetromino, vector<vector<int>> position) const{
    if((newTetromino.getShape())[3][0] != ' ')
        return true;
    return false;
}
// This funtion expands tetromino vector <GAME AREA> 4*4 space
void Tetromino::expendGameArea(){
    for(int i = 0; i < 4; i++)
        for(int j = 0 ; j < 4; j++)
            this->shape[i].push_back(' ');
}  
// sets tetromino shape instantly THIS FUNCTION ONLY USE FOR GAME AREA ADDING OPERATION
void Tetromino::setShape(vector<vector<char>> newShape, int i , int j, int k){
    if(! (newShape[i][k] == ' ' && this->shape[i][j] != ' ')){
        this->shape[i][j] = newShape[i][k];
    }
    
}
// This function delates right columns if there is no element
void Tetromino::eraseRight(){
    bool flag = true;
    // loop finds if there is a space right of the tetromino

    for(int j = 0; j < 4; j++)
        if(Tetromino::shape[j][Tetromino::shape[0].size()-1] != ' ')
            flag = false;
    
    // if space found we erase space after and after again
    while(flag){

        for(int i = 0; i < Tetromino::shape.size(); i++){ 
            Tetromino::shape[i].pop_back() ;

        }
               
        for(int j = 0; j < 4; j++)
            if(Tetromino::shape[j][Tetromino::shape[1].size()-1] != ' ')
                flag = false;
    }

}