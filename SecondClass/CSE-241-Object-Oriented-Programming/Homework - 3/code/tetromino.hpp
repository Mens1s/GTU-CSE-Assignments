#ifndef _TETROMINO_HPP_
#define _TETROMINO_HPP_

#include <iostream>
using namespace std;

enum class tetromino : char {
    I = 'I', O = 'O', T = 'T', J = 'J', L = 'L', S = 'S', Z = 'Z'
};

// MVector is a class of represent vector of char
class MVector{
    private:
        char * array;
        int cap;
        int current;
        
    public:
        MVector(){
            array = new char[1];
            cap = 1;
            current = 0;
        };  

        MVector(MVector& p1){
            cap = 1;
            current = 0;
            for(int a = 0; a < p1.cap; a++)
                push(p1[a]);
        };

        void push(char element){
            if(current == cap){
                char* temp = new char[2*cap];

                for(int a = 0 ; a < cap; a++)
                    temp[a] = array[a]; 
                
                delete[] array;
                cap *= 2;
                array = temp;
            }
            array[current] = element;
            current++;
        };

        void push_back(char data){
            push(data);
        }

        char get(int idx){
            if(idx < current)
                return array[idx];
            return '-';
        };

        void pop() {current--;}; 

        int size() {return current;};

        int getCapacity() {return cap;};

        void edit(int idx, char data){
            if(idx <= cap && idx >= 0)
                array[idx] = data;
        }

        char &operator[] (int a){
            return array[a];
        }

        ~ MVector(){
            delete [] array;
        };
};


class Tetromino{
    private:
        MVector * shape;
        char ** newShape = nullptr;

        void rotateLeft();
        int _height;
        int _len;

        int bestPosY;
        int bestPosX;

        int befMid;
        int check = 1;
    public:
        Tetromino();
        Tetromino(tetromino t);
        
        int getLen(){return _len;};
        int getHei(){return _height;};
        void setMid(int mid){befMid = mid;};
        void expendGameArea(int height, int len);
        int * getErasedShapeLoc();
        char ** getShape()const {return newShape;};

        void rotate(int rotLeft);

        void animate(Tetromino & userTetromino, int step);
        bool printPart(Tetromino & userTetromino, int step, int left);
        void print();

        bool isTrigger(Tetromino & userTetromino, int step, int rightMove);
        Tetromino operator +=(Tetromino &userShape);

        ~ Tetromino(){
            // delete [] shape;
            int rows = sizeof(newShape)/sizeof(newShape[0]);
            if(newShape != nullptr){
                for(int a = 0; a < rows ;a++)
                    delete [] newShape[a];
                delete [] newShape;
            }
        } 

};

#endif /* _TETROMINO_HPP_ */
