#include <iostream>
#include "tetromino.h"
#include <chrono>
#include <thread>


using namespace std;

// default constructor decleration
Tetromino::Tetromino(){
    Tetromino::shape ={{' '}};
}
// Constructor of class decleration 
Tetromino::Tetromino(tetromino t){ // shape creating for users choices

    if( t == tetromino::I)
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{' ',' ',' ',' '},{'I','I','I','I'}};
    else if( t == tetromino::O)
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{'O','O',' ',' '},{'O','O',' ',' '}};
    else if( t == tetromino::T)
        Tetromino::shape = {{' ',' ',' ',' '},{' ',' ',' ',' '},{'T','T','T',' '},{' ','T',' ',' '}};
    else if( t == tetromino::J)
        Tetromino::shape = {{' ',' ',' ',' '},{' ','J',' ',' '},{' ','J',' ',' '},{'J','J',' ',' '}};
    else if( t == tetromino::L)
        Tetromino::shape = {{' ',' ',' ',' '},{'L',' ',' ',' '},{'L',' ',' ',' '},{'L','L',' ',' '}};
    else if( t == tetromino::S)
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
void Tetromino::expendGameArea(int height, int len){
    _height = height;
    _length = len;
    for(int i = 0 ; i < height-1 ; i++)
        this->shape.push_back({' '});

    for(int i = 0; i < height; i++)
        for(int j = 0 ; j < len-1; j++)
            this->shape[i].push_back(' ');

}  
// sets tetromino shape instantly THIS FUNCTION ONLY USE FOR GAME AREA ADDING OPERATION const and & ekle
void Tetromino::setShape(vector<vector<char>> newShape, int i , int j, int k){
    if(! (newShape[i][k] == ' ' && this->shape[i][j] != ' ')){
        this->shape[i][j] = newShape[i][k];
    }
    
}

void Tetromino::setShape(vector<vector<char>> newShape, int posX , int posY,int x, int y){
    // cout << "shape y" << posY << " shape X" << posX << " newShape y" << y << "  newShape x" << x << endl;
    if(! (newShape[y][x] == ' ' && this->shape[posY][posX] != ' ')){
        this->shape[posY][posX] = newShape[y][x];
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

int Tetromino::getLength(){
    int y = -1;
    for(int a = 0; a < this->shape.size(); a++){
        for(int b = 0; b < this->shape[0].size(); b++){
            if (this->shape[a][b] != ' '){
                if (b > y){
                    y = b;
                }
            }
        }
    }
    return y+1;
}

int Tetromino::getHeight(){

    for(int a = 0; a < this->shape.size(); a++){
        for(int b = 0; b < this->shape[0].size(); b++){
            if (this->shape[a][b] != ' '){
                return 4-a;
            }
        }
    }
    return 0;
}

vector<vector<char>> Tetromino::getErasedShape(){
    // erase right
    vector<vector<char>> instantShape = shape;

    bool flag = true;
    // loop finds if there is a space right of the tetromino

    for(int j = 0; j < 4; j++)
        if(instantShape[j][instantShape[0].size()-1] != ' ')
            flag = false;
    
    // if space found we erase space after and after again
    while(flag){

        for(int i = 0; i < Tetromino::shape.size(); i++){ 
            instantShape[i].pop_back() ;

        }
               
        for(int j = 0; j < 4; j++)
            if(instantShape[j][instantShape[0].size()-1] != ' ')
                flag = false;
    }

    flag = true;
    // erasetop
    for(int j = 0; j < 4 && flag; j++)
        if (instantShape[0][j] != ' ')
            flag = false;

    // if space found we erase space after and after again
    while (flag){

        // instantShape.pop_front();
        instantShape.erase(instantShape.begin());

        flag = true;

        for(int j = 0; j < instantShape[0].size(); j++)
            if (instantShape[0][j] != ' ')
                flag = false;   
    } 
  

    return instantShape;
}

void Tetromino::animate(vector<vector<char>> newShape, int h , int l,int length, int height){
    string stuff(_length+2, '*');
    
    for(int go = 0 ; go <= h; go++){
        cout<<stuff<<endl; // _length+2
        for(int y = 0; y < _height ; y++ ){
            cout<<"*";
            for(int x = 0 ; x < _length ; x++){
                if(y >= go && y < go + height){
                    if(x >= l && x < l+length){
                        char c = newShape[y-go][x-l];
                        if(c == ' ')
                            cout << shape[y][x];
                        
                        else
                            cout << c;
                    }
                    else{
                        cout << shape[y][x];
                    }
                }
                else{
                    cout << shape[y][x];
                }
            }cout<< "*" << endl;
        }
        cout<<stuff<<endl; // _length+2
        this_thread::sleep_for(std::chrono::milliseconds(500));
        cout << "\033[2J" << endl;
        
    }
}
