#include "tetris.cpp"
#include "string.h"
#include <cstdlib>

class DriverControl{
    private:
        tetromino _userInput;
        string _userInp = "OLTTTISOTSJITTJ";
        int _height;
        int _len;

    public:
        DriverControl();
        void testTetrominoClass();
        void startGame();                   // starts game
};


int main(){
    DriverControl game;

    game.testTetrominoClass();

    game.startGame();
    
    return 0;
}

DriverControl::DriverControl(){};

void DriverControl::testTetrominoClass(){
    cout << "TEST of Tetromino Class..." << endl;
    string testTetrominos = "IOTJLSZ";
    
    for(int a = 0 ; a  < testTetrominos.size(); a++){
        tetromino testTetromino = static_cast<tetromino>(testTetrominos[a]);

        Tetromino temp(testTetromino);

        cout << "Test of Print Function for " << testTetrominos[a] << " tetromino.." << endl;

        cout << "ROTATING 3 TIMES..." << endl;
        temp.print();

        for(int b = 0 ; b < 3; b++){
            temp.rotate(true);
            temp.print();
        }
       
        cout << "Test of getErasedShape function..." << endl;
        cout << "This function erases spacenin in the vector.." << endl;

        vector<vector<char>> tempShape =  temp.getShape();
        cout << "Before erase, size of Height : " << tempShape.size() << " size of Length : " << tempShape[0].size() << endl;

        tempShape =  temp.getErasedShape();
        cout << "After erase, size of Height : " << tempShape.size() << " size of Length : " << tempShape[0].size() << endl;
        
        this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}   


void DriverControl::startGame()
{
    cout << "TEST of Tetris Class by playing the Game autonomusly.. " << endl;

    cout << "TEST HEIGHT : 8" << endl;
    cout << "TEST LEN : 8" << endl;
    cout << "We test this tetrominos one by on " << _userInp << endl;
    for(int a = 0 ; a  < _userInp.size(); a++) cout << _userInp[a] << ' ';
    cout << endl;

    _height = 8;
    _len = 8;
    this_thread::sleep_for(std::chrono::milliseconds(1500));

    Tetris game(_height, _len);

    for(int a = 0 ; a  < _userInp.size(); a++){
        _userInput = static_cast<tetromino>(_userInp[a]);

        Tetromino temp(_userInput);

        game.add(temp);
    }
    
}
