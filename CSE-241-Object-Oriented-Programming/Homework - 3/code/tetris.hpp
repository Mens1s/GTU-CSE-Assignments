#ifndef _TETRIS_HPP_
#define _TETRIS_HPP_
#include "tetromino.cpp"

class Tetris{
    private:
        int _height;
        int _len;

        Tetromino _gameArea;

        void const draw();

        bool isTrigger();

    public:
        Tetris(int height, int len);

        void add(Tetromino & userTetromino, int step);
        void setMid(int m){_gameArea.setMid(m);};

        const Tetris operator +(Tetromino &userShape);
        const Tetris operator +=(Tetromino &userShape);
};

#endif /* _TETRIS_HPP_ */
