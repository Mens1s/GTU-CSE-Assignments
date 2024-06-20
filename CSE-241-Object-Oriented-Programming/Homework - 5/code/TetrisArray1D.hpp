#ifndef _TetrisArray1D_H_
#define _TetrisArray1D_H_

#include "TetrisVector.cpp"
namespace TetrisArray1D{
class TetrisArray1D : public AbstractTetris::AbstractTetris{
    private:
        char * _gameArea;

        void const draw(); 
        char getElement(int h, int l){ return _gameArea[(h*_height) + l] ;};
        
        char * vectorToArray1D(vector<vector<char>> vectorGameArea, int height, int len);
    public:
        TetrisArray1D(int height, int len); 
        TetrisArray1D(TetrisArray1D & copy);
        ~TetrisArray1D(){};

        void add(Tetromino & instantTetromino, int right, int rotCount);
        void animate(vector<vector<char>> newShape);
        void playPrev();
        
        // operator overloading
        const TetrisArray1D operator +(const vector<vector<char>>  &userShape);
        const TetrisArray1D operator +=(const vector<vector<char>>  &userShape);
        const TetrisArray1D operator =(TetrisArray1D & copy);

};

#endif /* _TETRISVECTOR_H_ */
}
