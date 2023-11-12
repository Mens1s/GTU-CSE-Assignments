#include "TetrisAdapter.hpp"
#include <map>


// Game typename constructor || function setUserTetrominos sets tetrominos too
template <typename T>
TetrisAdapter<T>::TetrisAdapter(int height, int len){
    _height = height;
    _length = len;
    expendGameArea(_height, _length);

    vector<vector<char>> temp = _game.getShape(); // it returnes a vector 

    for(int h = 0; h < _height; h++)
        for(int l = 0; l < _length; l++)
            _gameArea.push_back(temp[h][l]);


}

// Instant best fit tetromino added the game area 
template <typename T>
void TetrisAdapter<T>::add(Tetromino & instantTetromino, int right, int rotCount){


    _pos = canAdd(instantTetromino, right); // h, l, rotCount,  length, height

    if(_pos[0] == -1 && _pos[1] == -1) exit(1);

    vector<vector<char>> userShape = _erasedInstant; // to know what is erasedInstant 

    this_thread::sleep_for(std::chrono::milliseconds(50));
    cout << "\033[2J" << endl;

    animate(userShape);
    numOfMov_++;
    //*this = *this + userShape;
    *this += userShape;
    writeToFile(instantTetromino, right, rotCount);
    lastMoveWrite(right, rotCount, instantTetromino.getType());

    draw();
}

// adds vector to gameArea
template <typename T>
const TetrisAdapter<T> TetrisAdapter<T>::operator +(const vector<vector<char>>  &userShape){
  
    int y = _pos[0]; 
    int a = 0;
    for(; y < _pos[0]+_pos[3] ; y++ ){
        int x = _pos[1];
        for(; x < _pos[1] + _pos[2] ; x++)
            _game.setShape(userShape, x, y, x-_pos[1], a);
        
        a++;
    }
    vector<vector<char>> temp = _game.getShape(); // it returnes a vector 

    // while (!_gameArea.empty()) _gameArea.pop();
    _gameArea.clear();

    for(int h = 0; h < _height; h++)
        for(int l = 0; l < _length; l++)
            _gameArea.push_back(temp[h][l]);
    return *this;
}

// basic overloading
template <typename T>
const TetrisAdapter<T> TetrisAdapter<T>::operator +=(const vector<vector<char>>  &userShape){
    return *this + userShape;
}

// This function prints game area
template <typename T>
void const TetrisAdapter<T>::draw(){
    string stuff(_length+2, '*');
    cout<<stuff<<endl; 
    for(int h = 0; h < _height; h++){
        cout << "*";
        for(int l = 0; l < _length; l++)      
            cout << _gameArea.at( (h*_height) + l );
        cout <<"*"<< endl;
    }
    cout<<stuff<<endl; // _length+2
}

// reads from file
template <typename T>
void TetrisAdapter<T>::playPrev(){

    vector<vector<string>> results = readFromFile();
    fstream oFile;
    oFile.open("output.txt", ios::out);
    oFile.close();// Deleting file to prevent movement dublicate

    for(vector<string> result : results)
    {
        tetromino _userInput = static_cast<tetromino>(result[0][1]);
        Tetromino temp(_userInput);
        for(int rot = 0; rot < stoi(result[2]); rot ++) temp.rotate(true);


        showAtMid(temp);

        add(temp, stoi(result[1]), stoi(result[2])); 
    }
    
};  

template <typename T>
void TetrisAdapter<T>::animate(vector<vector<char>> newShape){
   
    int h =  _pos[0]; int l = _pos[1]; int length =_pos[2]; int height = _pos[3];
    string stuff(_length+2, '*');
 
    for(int go = 0 ; go <= h; go++){
        cout<<stuff<<endl; // _length+2
        for(int y = 0; y < _height ; y++ ){
            cout<<"*";
            for(int x = 0 ; x < _length ; x++){
                if(y >= go && y < go + height){
                    if(x >= l && x < l+length){
                        char c = newShape[y-go][x-l];
                        if(c == ' ')
                            cout << _gameArea.at((y*_height) + x);
                        
                        else
                            cout << c;
                    }
                    else{
                        cout << _gameArea.at((y*_height) + x);
                    }
                }
                else{
                    cout << _gameArea.at((y*_height) + x);
                }
            }cout<< "*" << endl;
        }
        cout<<stuff<<endl; // _length+2
        this_thread::sleep_for(std::chrono::milliseconds(50));
        if(isMid == 0 ) cout << "\033[2J" << endl;
	isMid = 0;
    }
}

