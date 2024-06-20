#include "game.cpp"
#include "string.h"

void lenControl(string & userInp);
bool is_number(const string& s);

int main(){
    tetromino userInput;
    string userInp; 
    int tetroCount;
    vector<Tetromino> tetrominos = {};

    cout << "How many tetrominos?" << endl;
    cin >> userInp;
    
    while (!is_number(userInp)) {
        cout << "Please enter number : " << endl;
        cin >> userInp; 
    }
    // str to int 
    tetroCount =  stoi(userInp);

    for(int a = 0 ; a < tetroCount; a++){
        cout << a+1 << ". : ";
        cin >> userInp;

        // input length control
        lenControl(userInp);
        
        // input value control
        while (!(userInp == "I" || userInp == "O" || userInp == "T" || userInp == "J" || userInp == "L" || userInp == "S" || userInp == "Z")) {
            cout << "Enter only I, O, T, J , L, S, Z  : ";
            cin >> userInp;
            lenControl(userInp);
        }
        userInput = static_cast<tetromino>(userInp[0]);
        Tetromino temp(userInput);
        tetrominos.push_back(temp);
    }
    
    cout << "Your tetrominos are : " << endl;
    for(auto tetro : tetrominos)
        tetro.print(); cout << endl;
    
    cout << "Horizontally best-fit tetrominos" << endl;
    
    Game g(tetroCount , &tetrominos[0]);
    g.printGameArea();
    
    return 0;
}

void lenControl(string & userInp){
    while(userInp.length() != 1){
        cout << "Enter only 1 character : ";
        cin >> userInp;
    }
}
// control of string is number or not
// this function count (first string and other parameter comman characters) and control size equality s.size()
bool is_number(const string& s)
{
    return( strspn( s.c_str(), "0123456789" ) == s.size() );
}