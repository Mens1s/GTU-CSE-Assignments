#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "tetromino.cpp"

using std::vector;

class Tetris{
    private:
        int _maxH;
        int _maxL;
        int _height;
        int _length;
        vector<int> _pos;
        

        vector<vector<char>> _erasedInstant;
        vector<int> fit(Tetromino  & newTetromino); // try to find best tetromino to fit game area, returing can it move to left and if it how many step is it

        Tetromino _gameArea; // contains all tetrominos 
        Tetromino getGameArea() { return _gameArea;}; // returns Game Area

        void expendGameArea(int height, int len); // expands game area via using tetromino expand game area function
        void const draw(); // prints game area

        bool const isTrigger(vector<vector<char>> & userArea, int posX, int maxPosX, int posY, int maxPosY);
    
    public:
        Tetris(int height, int len); // constructor of game class
        void add(Tetromino & instantTetromino); // add tetromino to game area

        // operator overloading
        const Tetris operator +(const vector<vector<char>>  &userShape);
        const Tetris operator +=(const vector<vector<char>>  &userShape);

};
#endif /* _TETRIS_H_ */
