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
    
    string _userInp = "a";
    int height = 0;
    int length = 0;

    while (!isNumber(_userInp, 'n')) {
        cout << "Please enter height : " ;
        cin >> _userInp; 
    }
    height =  stoi(_userInp);

    _userInp = "a";
    while (!isNumber(_userInp, 'n')) {
        cout << "Please enter length : " ;
        cin >> _userInp; 
    } 
    length =  stoi(_userInp);

    cout << "Do you want to play at :" << endl << "1-) 1D Array Board" << endl << "2-) Vector Board" << endl <<"3-) Template Board(deque)\n";
    _userInp = "a";
    
    while (!isNumber(_userInp,'n') ) {cout << "Choice : " ;cin >> _userInp;}
    
    if(stoi(_userInp) == 1) { // Dedconstructırsız calısıyor
        TetrisArray1D::TetrisArray1D game(height, length);
        startGame<TetrisArray1D::TetrisArray1D>(game);
    }

    else if(stoi(_userInp) == 2) {
        TetrisVector::TetrisVector game(height, length);
        startGame<TetrisVector::TetrisVector>(game);
    }else{
        TetrisAdapter<deque<char>> game(height, length);
        startGame< TetrisAdapter< deque<char> > >(game);
        // TetrisAdapter<vector<char>> game(height, length);
        // startGame< TetrisAdapter< vector<char> > >(game);
    }
    return 0;
}


void lenControl(string & userInp){
    while(userInp.length() != 1){
        cout << "Enter only 1 character : ";
        cin >> userInp;
    }
}

template <typename T>
void startGame(T & game){

    string _userInp = "a";

    cout << "Do you want to play last game automaticaly?" << endl << "1-) Yes" << endl << "O(ther)-) No\n Choice : ";
    _userInp = "a";

    while (!isNumber(_userInp,'n')) cin >> _userInp; 
    
    if(stoi(_userInp) == 1){
        game.playPrev();
        cout << "Do you want to continue?" << endl << "1-) Yes" << endl << "2-) No\nChoice : ";
         _userInp = "a";
        while (!isNumber(_userInp,'n'))  cin >> _userInp; 
        
        if(stoi(_userInp) == 2)  exit(1);  
    }else{
        fstream oFile;
        oFile.open("output.txt", ios::out);
        oFile.close();
    }

    while(_userInp != "Q"){
        cout << "Enter Tetromino type (I, O, T, J, L, S, Z). R for random Tetromino, Q for quit." << endl;
        cin >> _userInp;

        // input length control
        lenControl(_userInp);
        
        // input value control
        while (!(_userInp == "I" || _userInp == "O" || _userInp == "T" || _userInp == "J" || _userInp == "L" || _userInp == "S" || _userInp == "Z" || _userInp == "R" || _userInp == "Q")) {
            cout << "Enter only I, O, T, J , L, S, Z  : ";
            cin >> _userInp;
            lenControl(_userInp);
        }
        if(_userInp != "Q"){
            if (_userInp == "R"){
                int a = rand() % 8;
                if (a == 1) _userInp = "I";else if (a == 2) _userInp = "O";else if (a == 3) _userInp = "T";else if (a == 4) _userInp = "J";else if (a == 5) _userInp = "L";else if (a == 6) _userInp = "S";
                else _userInp = "Z";
            }
            tetromino _userInput = static_cast<tetromino>(_userInp[0]);
            Tetromino temp(_userInput);
            game.showAtMid(temp);

            _userInp = "a";
            
            while (!isNumber(_userInp,'n')) {
                cout << "Please enter rotation count : " ;
                cin >> _userInp; 
            } 
            int rotCount = stoi(_userInp);
            for(int rot = 0; rot < stoi(_userInp); rot++) {temp.rotate(true);game.showAtMid(temp);}
            
            // middeki durumu göster
            _userInp = "a";
            while (!isNumber(_userInp,'p')) {
                cout << "Please enter move count (right is positive) : " ;
                cin >> _userInp; 
            } 
            int right = stoi(_userInp);
            
            game.add(temp, right, rotCount);
        }
    }
}

// this function count (first string and other parameter comman characters) and control size equality s.size()
bool isNumber(const string& s, const char type)
{
    string _userInp = "a";

    if(type == 'n') return( strspn( s.c_str(), "0123456789" ) == s.size() );
    return( strspn( s.c_str(), "-0123456789" ) == s.size() );
}
