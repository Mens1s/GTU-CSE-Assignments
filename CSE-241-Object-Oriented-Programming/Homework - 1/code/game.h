#include "tetromino.cpp"

using std::vector;

class Game{
    private:
        Tetromino gameArea; // contains all tetrominos 
        Tetromino * userTetrominos; // takes all user tetrominos
        vector<Tetromino> userTetromino; // instant user tetromino
        int findBestRotation(Tetromino  & newTetromino); // try to find best tetromino to fit game area, returing can it move to left and if it how many step is it
        void addTetro(); // add tetromino to game area
        int countTetrominos; // number of tetrominos
        int order; // instant tetromino order

        // inline functions
        void setUserTetrominos(Tetromino * uT) { this->userTetrominos =  uT; }; // sets user tetrominos to our private variable

    public:
        Game(int count, Tetromino tetrominos[]); // constructor of game class
        
        // inline functions
        Tetromino getGameArea() { return gameArea;}; // returns Game Area
        void setTCount(int count) { this->countTetrominos = count; }; // edit tetrominos
        int getTCount() const { return countTetrominos; }; // returns number of tetrominos
        void setOrder(int newOrder){this->order = newOrder;}; // sets instant game area
        int getOrder(){return this->order;}; // get instant tetromino order
    
        void startGame(); // starts game
        void expendGameArea(); // expands game area via using tetromino expand game area function
        void printGameArea(); // prints game area
        
};
