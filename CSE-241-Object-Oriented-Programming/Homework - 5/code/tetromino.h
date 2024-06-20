#ifndef _TETROMINO_H_
#define _TETROMINO_H_

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <map>
using namespace std;

enum class tetromino : char {
    I = 'I', O = 'O', T = 'T', J = 'J', L = 'L', S = 'S', Z = 'Z'
};

class Tetromino{
    private:
        vector<vector<char>> shape;
        void rotateLeft();
        int _height;
        int _length;
        char _type;

    public:
        Tetromino(); // constructor function
        Tetromino(tetromino t); // constructor function

        vector<vector<char>> getShape() {return Tetromino::shape;}; // returns instant tetromino shape 
        vector<vector<char>> getErasedShape(); // returns instant erased  tetromino shape 
        
        void print() const; // That is print function of Tetromino
        void rotate(bool rotLeft); // rotates left 90 degrees
        void setShape(vector<vector<char>> newShape, int i , int j, int k);  // sets tetromino shape instantly THIS FUNCTION ONLY USE FOR GAME AREA ADDING OPERATION
        void setShape(vector<vector<char>> newShape, int posx , int posY,int i, int k);  // sets tetromino shape instantly THIS FUNCTION ONLY USE FOR GAME AREA ADDING OPERATION
        void expendGameArea(int height, int len); // This funtion expands tetromino vector <GAME AREA> 4*4 space
        void eraseRight(); // This function delates right columns if there is no elemnt
        
        int getHeight();
        int getLength();

        char getType(){return _type;};

};

#endif /* _TETROMINO_H_ */