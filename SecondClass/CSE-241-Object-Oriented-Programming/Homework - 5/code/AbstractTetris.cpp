#include "AbstractTetris.hpp"
namespace AbstractTetris{
vector<vector<string>> AbstractTetris::readFromFile(){
    ifstream iFile("output.txt");
    string lineText;

    string numofmove = "";
    string added_piece = "";
    string right_move_count = "";
    string rot_count = "";
    int idx = 0;

    vector<vector<string>> results;
    bool flag = false;

    while (getline (iFile, lineText)) {
        
        idx = lineText.find("=") + 1; 

        if(lineText[0] == '1')
            for(;idx < lineText.length();idx++)
                numofmove += lineText[idx];

        else if(lineText[0] == '2')
            for(;idx < lineText.length(); idx++)
                added_piece += lineText[idx];

        else if(lineText[0] == '3')
            for(;idx < lineText.length();idx++)
                right_move_count += lineText[idx];

        else if(lineText[0] == '4'){
            for(;idx < lineText.length();idx++)
                rot_count += lineText[idx];
            flag = true;
        }
        
        if(flag){
            vector<string> temp;
            temp.push_back(added_piece);
            temp.push_back(right_move_count);
            temp.push_back(rot_count);
            results.push_back(temp);
            flag = false;
            rot_count = "";right_move_count = "";added_piece = "";numofmove = "";
        }
    }   
	iFile.close();
    return results;
}

// writes to file every Detail
// NUM_OF_MOV = INT
// ADDED_PIECE = TETROMINO (I,O,T, ETC)
// TETROMINO_PIECE
// RIGHT_MOVE_COUNT = INT
// ROT_COUNT = INT
void AbstractTetris::writeToFile(Tetromino t, int rightMoveCount, int rotCount){
    ofstream oFile;
    oFile.open("output.txt", ios_base::app);
  
    oFile << "1- NUM_OF_MOV  = " << numberOfMoves() << endl;
    oFile << "2- ADDED_PIECE = " << t.getType() << endl;
    oFile << "3- RIGHT_MOVE_COUNT = " << rightMoveCount << endl;
    oFile << "4- ROT_COUNT = " << rotCount << endl << endl;
	
    oFile.close();
};
string AbstractTetris::lastMove() {
    string result="";

    fstream iFile;
    iFile.open("lastMove.txt",ios::in);
    
    while(iFile) {getline(iFile, result); return result;}
    
    iFile.close();

    if(result == "")
        throw NoLastMove();
    
    return result;
}

void AbstractTetris::lastMoveWrite(int right, int rotCount, char type){
    ofstream oFile;
    oFile.open("lastMove.txt");
    
    oFile << "1- TYPE of TETROMINO  = " << type ;
    oFile << "\t2- RIGHT STEP COUNT = " << right ;
    oFile << "\t3- ROTATION_COUNT = " << rotCount << endl;

    oFile.close();
};

// function calls expand game area with using tetromino class we can use inheritence too but it is not allowed yet!  
void AbstractTetris::expendGameArea(int height, int len){
    _length = len; 
    _height = height;
    this->_game.expendGameArea(height, len);
}  

// Program tries to find best rotation of tetromino to fit game area
vector<int> AbstractTetris::canAdd(Tetromino & instantTetromino, int right){

    map<int, vector<int>> scoreMap; 
    for(int rotateCount = 0; rotateCount < _midPoint[0]; rotateCount++) instantTetromino.rotate(true);
    
    int length = instantTetromino.getLength();
    int height = instantTetromino.getHeight();

    vector<vector<char>> userArea =  instantTetromino.getErasedShape();
    int goDown = 0;
    bool firstMove = false;
 
    while(isTrigger(userArea, goDown, _midPoint[1]+right, length, height) ) {
        goDown++;
        firstMove = true;
        if(goDown >= _height-height)  break; 
    }
    

    if((goDown > _height-height )) goDown--; 
    // int l = right; // mid 0 kabul alıncak asıl kullanıcıdan alınacak yer burası
    scoreMap[0] = {goDown, _midPoint[1]+right,  length, height};
 
    if(!firstMove){    
        cout << "We cannot add this item to tetris.. Game has been finished.. " << endl;
        return {-1 , -1, -1, -1};
    }

    _erasedInstant = userArea;

    return scoreMap[0];
}

void AbstractTetris::showAtMid(Tetromino & instantTetromino){

    int mid = _length/2;
    bool flag = false;

    int length = 0;
    int height = 0;
    int rotCount = 0;

    for(; rotCount < 3 && !flag; rotCount++){
  
        vector<vector<char>> userArea =  instantTetromino.getErasedShape();
   
        length = instantTetromino.getLength();
        height = instantTetromino.getHeight();
        _erasedInstant = userArea;

        mid = _length/2 - length/2;
        
        while(!isTrigger(userArea, 0, mid, length, height) && mid >= 0) mid--;
  

        if(mid < 0){        
            mid = _length/2;
            while(!isTrigger(userArea, 0, mid, length, height) && mid < _length-length) mid++;
            if(mid == _length-length) flag = false;
            else flag = true;
        }else flag = true;
        if(!flag) instantTetromino.rotate(true);
    }

    if(!flag) {
        cout << "mide ekleyemedi"<<endl;
        exit(1);
        }
    _pos = {0, mid, length, height};
    isMid = 1;
    animate(_erasedInstant);
    _midPoint = {rotCount-1, mid}; // rotCount mid

}

// isTrigger function helps to find isTriggered or not 
bool const AbstractTetris::isTrigger(vector<vector<char>> & userArea, int h, int l, int length, int height){

    for(int y = h+1; y <= h+height; y++){
        for(int x = l; x < l+length; x++){
            if(getElement(y, x) != ' ' && userArea[y-h-1][x-l] != ' '){
                return false;}
        }
    }
    return true;
}
}
