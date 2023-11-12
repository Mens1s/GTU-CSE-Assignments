#include "tetris.h"
#include <map>
// Game class constructor || function setUserTetrominos sets tetrominos too
Tetris::Tetris(int height, int len){
    _maxH = height;
    _maxL = len;
    expendGameArea(_maxH, _maxL);
}

// Program tries to find best rotation of tetromino to fit game area
vector<int> Tetris::fit(Tetromino & instantTetromino){
    vector<vector<char>> gameArea = _gameArea.getShape();
    map<int, vector<int>> scoreMap; 
    int maxScore = 0;

    // amacim 3 rotate yukardan asagi indirme ve ilk temasta puan hesaplama!
    for(int rotateCount = 0 ; rotateCount < 3 ; rotateCount++) {

        int length = instantTetromino.getLength();
        int height = instantTetromino.getHeight();
        int score = 0;
        
        vector<vector<char>> userArea =  instantTetromino.getErasedShape();
 
        for(int l = 0; l <= _maxL-length; l++){
            bool yTrigger = true;
            
            for(int h = 0; h <= _maxH-height && yTrigger; h++){
                
                score = 0;

                if(isTrigger(userArea,  h,  l,  length, height)){
                    // cout << "ilk score : "<< score << endl;
                    if(h == _maxH-height){ // en alttaki satirlar +2 point
                        for(int x = 0; x < length ; x++){
                            if(userArea[userArea.size()-1][x] != ' ')
                                score += 2;
                        }
                    }
                    else{ // alttan temaslar 1 puan
                        for(int x = l; x < l + length && x < _maxL && x > 1; x++){
                            if(gameArea[h+height][x] != ' ' && userArea[userArea.size()-1][x-l]){
                                score++;
                            }
                        }
                    }
              
                    for(int y = _maxH-height; y < _maxH  && y-_maxH+height>=0 ; y++){ // soldan temas +1 point
                        if(gameArea[y][l-1] != ' ' && userArea[y-_maxH+height][0] != ' '){
                            score++;    
                        }
                    }

                    for(int y = _maxH-height; y < _maxH  && y-_maxH+height < userArea.size(); y++){ // sagdan temas +1 point
                        if(gameArea[y][l+length] != ' ' && userArea[y-_maxH+height][length-1] != ' '){
                            score++;
                        }
                    }
                    score += (2*h+height);
        

                    if(score>=maxScore){  
                        maxScore = score;
                        _erasedInstant = userArea;
                    }

                    scoreMap[score] = {h, l, rotateCount, length, height};  
                }else yTrigger = false;
            }
        }

        instantTetromino.rotate(true);
    }

    if(maxScore == 0){
        cout << "We cannot add this item to tetris.. Game has been finished.. " << endl;
        return {-1 , -1, -1, -1, -1};
    }
    return scoreMap[maxScore];
}

// Instant best fit tetromino added the game area 
void Tetris::add(Tetromino & instantTetromino){
    _pos = fit(instantTetromino); // h, l, rotCount,  length, height

    if(_pos[0] == -1 && _pos[1] == -1) {
        exit(1);
    }

    vector<vector<char>> userShape = _erasedInstant; // to know what is erasedInstant 

    this_thread::sleep_for(std::chrono::milliseconds(500));
    cout << "\033[2J" << endl;

    _gameArea.animate(userShape,_pos[0],_pos[1],_pos[3],_pos[4]);

    //*this = *this + userShape;
    *this += userShape;
    draw();
}

const Tetris Tetris::operator +(const vector<vector<char>>  &userShape){

    int y = _pos[0]; 
    int a = 0;
    for(; y < _pos[0]+_pos[4] ; y++ ){
        int x = _pos[1];
        for(; x < _pos[1] + _pos[3] ; x++)
            _gameArea.setShape(userShape, x, y, x-_pos[1], a);
        
        a++;
    }
    return *this;
}

const Tetris Tetris::operator +=(const vector<vector<char>>  &userShape){
    return *this + userShape;
}

// function calls expand game area with using tetromino class we can use inheritence too but it is not allowed yet!  
void Tetris::expendGameArea(int height, int len){
    _length = len; 
    _height = height;
    this->_gameArea.expendGameArea(height, len);
}  

// This function prints game area
void const Tetris::draw(){
    string stuff(_length+2, '*');
    cout<<stuff<<endl; 
    for(auto ts : (this->_gameArea.getShape())){
        cout << "*";
        for(auto t : ts)      
            cout << t;
        cout <<"*"<< endl;
    }
    cout<<stuff<<endl; // _length+2
}
     
bool const Tetris::isTrigger(vector<vector<char>> & userArea, int h, int l, int length, int height){
    vector<vector<char>> gameArea = _gameArea.getShape();

    for(int y = h; y < h+height; y++)
        for(int x = l; x < l+length; x++)
            if(gameArea[y][x] != ' ' && userArea[y-h][x-l] != ' ')
                return false;
        
    return true;
}
