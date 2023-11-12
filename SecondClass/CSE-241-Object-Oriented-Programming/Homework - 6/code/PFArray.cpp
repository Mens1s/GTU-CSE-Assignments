#include "PFArray.hpp"

using namespace it;
using namespace arr;

// Constructors for PFArray 
template <typename T>
PFArray<T>::PFArray() : data_(nullptr), size_(0) {}

template <typename T>
PFArray<T>::PFArray(PFArray&& o) : data_( move(o.data_) ), size_(o.size_){
    o.size_ = 0;
}

// Functions declaration

// Function name : empty        return type: bool       aim : checking PFArray is empty or not
template <typename T>
bool PFArray<T>::empty() const { return size_ == 0; }

// Function name : push_back        return type: void   aim : adding element to data_/array
template <typename T>
void PFArray<T>::push_back(const T& addData){
    shared_ptr<T> temp( new T[size_+1] ); // creating new shared_ptr with size_+1 to hold all prev datas and new data

    for(size_t i = 0; i < size_; ++i) temp.get()[i] = data_.get()[i]; // assign old element to new data

    temp.get()[size_] = addData;

    data_.swap(temp);   // temp data has to be PFArray new data_
    ++size_;            // size++
}

// Function name : size        return type: size_t      aim : return number of elements
template <typename T>
size_t PFArray<T>::size() const{ return size_; }

// Function name : clear        return type: void       aim : clears PFArray
template <typename T>
void PFArray<T>::clear(){ 
    data_ = nullptr;
    size_ = 0;
}

// Function name : erase        return type: void       aim : erase iterator point from array
template <typename T>
void PFArray<T>::erase(Iterator<T> iterator){
    shared_ptr<T> temp( new T[size_-1] ); // creating new shared_ptr with size_+1 to hold all prev datas and new data
    int isFinded = 0;
    int idx = 0;

    for (auto it = begin(); it != end(); ++it, ++idx) {
        if(it == iterator)  isFinded = -1;
        else temp.get()[idx + isFinded] = data_.get()[idx];
    }
    --size_;
    data_.swap(temp);
};

// Function name : erase        return type: size_t       aim : erase 'val', which is defined as a parameter, from data_
template <typename T>
size_t PFArray<T>::erase(const T & val){
    size_t result = 0;
    shared_ptr<T> temp( new T[size_] );

    int idx = 0;
    int count = 0;

    for (auto it = begin(); it != end(); ++it, ++idx) {
        if(*it == val)  {
            temp.get()[idx] = data_.get()[idx];
            ++result;
            ++count;
        }
    }

    shared_ptr<T> erased( new T[size_-count] );

    int isFinded = 0;
    idx = 0;
    for (auto it = begin(); it != end(); ++it, ++idx) {
        if(*it == val)  --isFinded;
        else erased.get()[idx + isFinded] = data_.get()[idx];
    }

    for(int i = 0; i < count;i++) --size_;
    data_.swap(erased);

    return result;
}    

// Function name : erase        return type: void       aim : erase iterator point from array
template <typename T>
void PFArray<T>::erase(Iterator<T> first, Iterator<T> last){

    if( first > last ){
        throw IteratorInf(); // thrown !!!!

    }else{
        bool isBtwn = false;
        int startIndex = -1;
        int endIndex = -1;
        int idx = 0;
        int count = 0;

        for (auto it = begin(); it != end(); ++it, ++idx){
            if(it == first){
                isBtwn = true;
                startIndex = idx;

                if(it == last)
                    endIndex = idx;
            }
            else{
                if(it == last){
                    isBtwn = false;
                    endIndex = idx;
                }
                else if( !isBtwn )
                    ++count;
            }
        }
        
        if( (endIndex == -1 || startIndex == -1)) 
            throw IteratorCannotFinded();  // we can not throw exception only return this is more useful logic in here but I want to use exception!;
        
        if(startIndex == endIndex) count = size_ - 1;

        shared_ptr<T> erased( new T[count] );
        int isFinded = 0;
        idx = 0;
        
        for (auto it = begin(); it != end(); ++it, ++idx) {
            if(idx >= startIndex && idx <= endIndex)  --isFinded;
            else erased.get()[idx + isFinded] = data_.get()[idx];
        }

        int deletedSize = size_-count;
        for(int i = 0; i < deletedSize; i++) --size_;

        data_.swap(erased);
    }
    
}
// Opeartor overloading
template <typename T>
PFArray<T>& PFArray<T>::operator=(PFArray&& o) {
    if (this != &o) {
        data_   = move(o.data_);
        size_   = o.size_;
        o.size_ = 0;
    }
    return *this;
}

// Iterator functions 

// Function name : sort [overloaded]        return type: void       aim : sorts begin to end
template <typename T>
void sort(Iterator<T> begin, Iterator<T> end){   
    auto tempEnd = end;
    --tempEnd;
    for(auto b = begin; b != end; ++b){
        for(auto c = begin; c != tempEnd; ++c){
            auto tempC = c;
            if ( *c > *(++tempC) ){
                auto tempVal = *c;
                *c = *tempC;
                *tempC = tempVal;
            }
        }
    }
        
}

template <typename T>
int find(Iterator<T> begin, Iterator<T> end, T val){  
    int idx = 0;

    for(auto it = begin; it != end; ++it, ++idx){
        if(val == *(it)) return idx;
    }
    return -1;
}
// Function name : begin        return type: Iterator       aim : returns begin of shared_ptr/data_
template <typename T>
Iterator<T> PFArray<T>::begin() { return Iterator<T>(data_.get()); }

// Function name : end          return type: Iterator       aim : returns end of shared_ptr/data_
template <typename T>
Iterator<T> PFArray<T>::end() { return Iterator<T>(data_.get() + size_); } 

// Function name : cbegin        return type: Iterator  const     aim : returns cbegin of shared_ptr/data_
template <typename T>
Iterator<T> PFArray<T>::cbegin() const { return Iterator<T>(data_.get()); };

// Function name : cend          return type: Iterator  const     aim : returns cend of shared_ptr/data_
template <typename T>
Iterator<T> PFArray<T>::cend() const { return Iterator<T>(data_.get() + size_);}
