#ifndef _TETROMINO_H_
#define _TETROMINO_H_

#include <vector>
#include <string>
using std::vector;

enum class tetromino : char {
    I = 'I', O = 'O', T = 'T', J = 'J', L = 'L', S = 'S', Z = 'Z'
};

class Tetromino{
    private:
        vector<vector<char>> shape;
        void rotateLeft();
        int _height;
        int _length;

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
        void animate(vector<vector<char>> newShape, int posX , int posY,int x, int y);
        
        int getHeight();
        int getLength();

        bool canFit(Tetromino T, vector<vector<int>> position) const; // This function find otherTetromino[3][0] is better or not
};

#endif /* _TETROMINO_H_ */