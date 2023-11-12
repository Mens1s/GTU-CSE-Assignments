#include "tetromino.cpp"
#include <fstream>

namespace AbstractTetris{
class AbstractTetris{
    protected:
        int _height;
        int _length;
        int numOfMov_;
	int isMid = 0;
        Tetromino _game; // contains all tetrominos 
        Tetromino getGameArea() { return _game;}; // returns Game Area

        vector<int> _pos;
        vector<int> _midPoint;
        vector<vector<char>> _erasedInstant;

        void expendGameArea(int height, int len); // expands game area via using tetromino expand game area function

        virtual char getElement(int h, int l) = 0;
        virtual bool const isTrigger(vector<vector<char>> & userArea, int posX, int maxPosX, int posY, int maxPosY);

        
    public:
        class NoLastMove : public exception{
            public:
                string what(){
                    return "You don't have any last move! \n";
                };
        };
        AbstractTetris(){ numOfMov_=0;};
        int numberOfMoves() { return numOfMov_; };

        vector<int> canAdd(Tetromino  & newTetromino, int right); // try to find best tetromino to fit game area, returing can it move to left and if it how many step is it
        vector<vector<string>> readFromFile();
        void lastMoveWrite(int right, int rotCount, char type);
        string lastMove();

        void showAtMid(Tetromino & instantTetromino);
        void writeToFile(Tetromino t, int rightMoveCount, int rotCount);
    
        virtual void const draw() = 0;
        virtual void animate(vector<vector<char>> userShape) = 0;
        // virtual AbstractTetris & operator+=(const Tetromino element) = 0;
        // virtual ~AbstractTetris(){};
    
};

}
