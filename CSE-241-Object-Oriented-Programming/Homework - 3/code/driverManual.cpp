#include "tetris.cpp"
#include <cstdlib>

class DriverControl{
    private:
        tetromino _userInput;
        string _userInp = "initialValue";
        int _height;
        int _len;

    public:
        DriverControl();
        void lenControl(string& userInp);   // control of userInp size is one or not and take character as a len is not one
        bool isNumber(const string& s);     // control of string is number or not
        void getSizes();                    // get sizes from user
        void startGame();                   // starts game
};


int main(){
    DriverControl game;

    game.getSizes();

    game.startGame();
    
    return 0;
}

DriverControl::DriverControl(){};

void DriverControl::lenControl(string & userInp){
    while(userInp.length() != 1){
        cout << "Enter only 1 character : ";
        cin >> userInp;
    }
}

// this function count (first string and other parameter comman characters) and control size equality s.size()
bool DriverControl::isNumber(const string& s)
{
    return( strspn( s.c_str(), "0123456789" ) == s.size() );
}

void DriverControl::getSizes(){

    while (!isNumber(_userInp)) {
        cout << "Please enter height : " ;
        cin >> _userInp; 
    }
    _height =  stoi(_userInp);

    _userInp = "a";
    while (!isNumber(_userInp)) {
        cout << "Please enter length : " ;
        cin >> _userInp; 
    } 
    _len =  stoi(_userInp);
}

void DriverControl::startGame(){
    Tetris game(_height, _len);
    
    while(_userInp != "Q"){
        game.setMid(0);
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
            _userInput = static_cast<tetromino>(_userInp[0]);
            Tetromino temp(_userInput);

            //

            int direction = 0;
            int counter = 0;
            int step;

            game.add(temp, -1);

            _userInp = "a";
            while (!isNumber(_userInp)) {
                cout << "Which Turn Direction ? \n1-)\tRight\nO-)\tLeft\nQ-)\tQuit\n\t\tChoice : ";
                cin >> _userInp; 
            } 
            direction =  stoi(_userInp);

            _userInp = "a";
            while (!isNumber(_userInp)) {
                cout << "\nEnter rotation count : ";
                cin >> _userInp; 
            } 
            counter =  stoi(_userInp);
        
            for(int a = 0 ; a < counter ; a++){
                temp.rotate(direction);
                game.add(temp, -1);
                // printPart(userTetromino, 0, 0);
            }

            game += temp;
            // game.add(temp, step);
        }
    }
}
