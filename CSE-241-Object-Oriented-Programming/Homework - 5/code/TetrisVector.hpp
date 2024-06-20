#ifndef _TETRISVECTOR_H_
#define _TETRISVECTOR_H_

#include "AbstractTetris.cpp"
using namespace AbstractTetris;

namespace TetrisVector{
class TetrisVector : public AbstractTetris::AbstractTetris{
    private:
        vector<vector<char>> _gameArea;

        void const draw(); // prints game area
        char getElement(int h, int l){ return _gameArea[h][l] ;};
        
    public:
        TetrisVector(int height, int len); // constructor of game class
        TetrisVector(TetrisVector & copy);
        ~TetrisVector(){}; // constructor of game class

        void add(Tetromino & instantTetromino, int right, int rotCount); // add tetromino to game area
        void animate(vector<vector<char>> newShape);
        void playPrev();
        
        // operator overloading
        const TetrisVector operator +(const vector<vector<char>>  &userShape);
        const TetrisVector operator +=(const vector<vector<char>>  &userShape);
        const TetrisVector operator =(TetrisVector & copy);
    
};
#endif /* _TETRISVECTOR_H_ */
}
