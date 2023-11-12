#include <iostream>
#include <stdexcept>
#include <memory>
#include <algorithm>

using namespace std;

class IteratorInf : public exception{
    public:
        string what(){
            return "First iterator after than last iterator. Run time error!\n";
        };
};

class IteratorCannotFinded : public exception{
    public:
        string what(){
            return "Iterator cannot finded in PFArray!. No element erased.\n";
        };
};

namespace it{
template <typename T>
class Iterator{
    public: 
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = random_access_iterator_tag;
        
        Iterator(T* p) : ptr_(p) {}

        // Overloading *(star) operator
        T& operator*() { return *ptr_; }

        // Overloading -> operator
        T* operator->() { return ptr_; }

        // Overloading post increment operator
        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        // Overloading post decrement operator
        Iterator& operator--() {
            --ptr_;
            return *this;
        }

        // Overloading == operator
        bool operator==(const Iterator& other) {
            return ptr_ == other.ptr_;
        }

        // Overloading != operator
        bool operator!=(const Iterator& other) {
            return ptr_ != other.ptr_;
        }

        Iterator& operator+(int a) {
            for(int i = 0; i<a;i++) ++ptr_;
            return *this;
        };

        bool operator>(Iterator other) {
            return ptr_ > other.ptr_;
        };

        bool operator<(Iterator other) {
            return ptr_ < other.ptr_;
        };
        // for range based loops

    private:
        T* ptr_;
};
}

namespace arr{
template <typename T>
class PFArray{

    private:
        shared_ptr<T> data_;   // shared_ptr data variables
        size_t size_;                // size_t is an unsigned integral data type => geegforgeegs:d
        // size_t == sizeof(T) 
        // int 4 4
        // char 1 1 
    public:
        // Constructor definations
        PFArray();
        PFArray(PFArray&& o);

        // Destructor 
        ~PFArray() { clear(); };

        // Functions
        bool empty() const;

        void push_back(const T& addData);

        size_t size() const;

        void clear();

        void erase(it::Iterator<T> iterator);
        size_t erase(const T & val);    
        void erase(it::Iterator<T> first, it::Iterator<T> last);
        // Iterator functions
        it::Iterator<T> begin();
        it::Iterator<T> end();
        it::Iterator<T> cbegin() const;
        it::Iterator<T> cend() const;

        // operator overloading
        PFArray<T>& operator=(PFArray&& other);
};
}