#ifndef _TETRISADAPTER_H_
#define _TETRISADAPTER_H_

#include "TetrisArray1D.cpp"


template <typename T>
class TetrisAdapter : public AbstractTetris::AbstractTetris{

    private:
        T _gameArea;

        char getElement(int h, int l){ return _gameArea.at( (h*_height) + l ) ;};
        void const draw(); 
    
    public:
        TetrisAdapter(int height, int len); 
        TetrisAdapter(){}; 
        
        void add(Tetromino & instantTetromino, int right, int rotCount); // add tetromino to game area
        void animate(vector<vector<char>> newShape);
        void playPrev();
        
        // operator overloading
        const TetrisAdapter operator +(const vector<vector<char>>  &userShape);
        const TetrisAdapter operator +=(const vector<vector<char>>  &userShape);
        
};


#endif /* _TETRISADAPTER_H_ */
