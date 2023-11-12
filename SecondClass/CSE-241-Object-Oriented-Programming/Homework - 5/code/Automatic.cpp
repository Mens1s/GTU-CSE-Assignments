#include "TetrisAdapter.cpp"
#include "string.h"
#include <cstdlib>
#include <queue>

using namespace TetrisArray1D;
using namespace TetrisVector;

bool isNumber(const string& s, const char type);
void lenControl(string & userInp);

template <typename T>
void startGame(T & game);

int main(){


    TetrisArray1D::TetrisArray1D game1(10, 10);
    // testing getting last move information when it is empty
    try{
        cout << game1.lastMove();
    }catch(AbstractTetris::AbstractTetris::NoLastMove e){
        cout << e.what() << endl;
    }

    // testing playing prev game with 1D array
    cout << "Playing previous game (1D Game Board): \n";
    game1.playPrev();
    cout << "Adding O (3 RIGHT MOVE), J (0 RIGHT MOVE), S (-3 RIGHT MOVE), Z (2 RIGHT MOVE)\n";

    Tetromino temp(static_cast<tetromino>('O'));
    game1.showAtMid(temp);
    game1.add(temp, 3, 0);

    Tetromino temp1(static_cast<tetromino>('J'));
    game1.showAtMid(temp1);
    game1.add(temp1, 0, 0);

    Tetromino temp2(static_cast<tetromino>('S'));
    game1.showAtMid(temp);
    game1.add(temp, -2, 0);
    
    Tetromino temp3(static_cast<tetromino>('Z'));


    // testing playing with vector
    cout << "Transform to VECTOR GAME BOARD" << endl;
    TetrisVector::TetrisVector game2(10, 10);
    game2.showAtMid(temp);
    game2.add(temp, -4, 0);

    game2.showAtMid(temp1);
    game2.add(temp1, -1, 0);

    game2.showAtMid(temp2);
    game2.add(temp2, 3, 0);

    game2.showAtMid(temp3);
    game2.add(temp3, -2, 0);
	
    // CLEANING THE OUTPUT FILE TO WHEN USER TESTS LAST GAME IN MANUAL IT CAN SEE THE RESULT
    fstream oFile;
    oFile.open("output.txt", ios::out);
    oFile.close();
    cout << "Transform to TEMPLATE GAME(deque) BOARD" << endl;

    TetrisAdapter<deque<char>> game3(10, 10);
    game3.showAtMid(temp);
    game3.add(temp, -4, 0);

    game3.showAtMid(temp1);
    game3.add(temp1, -1, 0);

    game3.showAtMid(temp2);
    game3.add(temp2, 3, 0);

    game3.showAtMid(temp3);
    game3.add(temp3, -2, 0);
    
    game3.showAtMid(temp2);
    game3.add(temp2, 1, 0);

    game3.showAtMid(temp1);
    game3.add(temp1, 3, 0);

    try{
        cout << game1.lastMove();
    }catch(AbstractTetris::AbstractTetris::NoLastMove::exception e){
        cout << e.what() << endl;
    }
    
    return 0;
}

