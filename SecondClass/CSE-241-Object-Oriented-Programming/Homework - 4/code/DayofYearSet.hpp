#include <iostream>
#include <list>

using namespace std;

namespace DSet{

    class DayofYearSet{
        public:
            class DayofYear{   
                private:
                    int _day;
                    int _month;
                    int _hash;

                    void calcHash();
                    bool checkInp();
                public:
                    class OutofDayException : public exception{
                        public:
                            string what(){
                                return "You have entered wrong day enter properly! \n";
                            };
                    };
                    class OutofMonthException : public exception{
                        public:
                            string what(){
                                return "You have entered wrong month enter properly! \n";
                            };
                    };
                    
                    DayofYear();
                    DayofYear(string data, string type = "eu");
                    DayofYear(int day, int month);
                    DayofYear(int day);

                    int getDay() const{ return _day;};
                    int getMonth() const{ return _month;};
                    int getHash() const{return _hash;};
                    
                    bool operator==(const DayofYear & oData) const;
                    friend ostream & operator<<(ostream & os,const DSet::DayofYearSet::DayofYear & data );
                    
                    void print(){
                        cout << "day : " << _day << "  month : " << _month << endl;
                    }
            };
            
            DayofYearSet(const initializer_list<DayofYear> data);
            DayofYearSet(DayofYearSet & copy);
    
            DayofYearSet();
            ~DayofYearSet(){ delete[] _set;}

            friend ostream & operator <<(ostream & os,const DSet::DayofYearSet & data );
            ostream & print(ostream & os) const;

            DayofYearSet operator=(const DayofYearSet & oData);
            bool operator==(const DayofYearSet & oData) const;
            bool operator!=(const DayofYearSet & oData) const { return *this == oData;};

            int getCap() const {return _cap;};
            int size() const {return _size;};
            void addData(const DayofYear data);

            void remove(const DayofYear & data);

            DayofYearSet operator+(const DayofYear & data); // adds an element
            DayofYearSet operator+(const DayofYearSet & data); // returns the union sets

            DayofYearSet operator-(const DayofYear & data); // removes an element
            DayofYearSet operator-(const DayofYearSet & data); // returns the difference set

            DayofYearSet operator^(const DayofYearSet & data) const; //  returns the intersection set.
            DayofYearSet operator!()const; // returns complements of set
            DayofYear operator[](int idx); // return the element at given position

        private:
            DayofYear * _set;
            int _size;
            int _cap;
    };

    ostream & operator<<(ostream & os,const DSet::DayofYearSet & data ){

        for(int idx = 0; idx < data._size; idx++)
            os << "day : " << data._set[idx].getDay() << " Month : " << data._set[idx].getMonth() << endl;

        return os;
    }

    ostream & operator<<(ostream & os,const DSet::DayofYearSet::DayofYear & data ){

        os << "day : " << data.getDay() << " Month : " << data.getMonth() << endl;

        return os;
    }
}


