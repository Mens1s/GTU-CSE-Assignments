#include <vector>
#include <string>
using std::vector;

enum class tetromino{
    I, O, T, J, L, S, Z
};

class Tetromino{
    private:
        vector<vector<char>> shape;
        void rotateLeft();
        
    public:
        Tetromino(); // constructor function
        Tetromino(tetromino t); // constructor function
        void print() const; // That is print function of Tetromino
        bool canFit(Tetromino T, vector<vector<int>> position) const; // This function find otherTetromino[3][0] is better or not
        void rotate(bool rotLeft); // rotates left 90 degrees
        vector<vector<char>> getShape() {return Tetromino::shape;}; // returns instant tetromino shape 
        void setShape(vector<vector<char>> newShape, int i , int j, int k);  // sets tetromino shape instantly THIS FUNCTION ONLY USE FOR GAME AREA ADDING OPERATION
        void expendGameArea(); // This funtion expands tetromino vector <GAME AREA> 4*4 space
        void eraseRight(); // This function delates right columns if there is no elemnt
};