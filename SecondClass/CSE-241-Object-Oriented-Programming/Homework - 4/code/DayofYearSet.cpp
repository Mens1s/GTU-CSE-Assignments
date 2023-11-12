#include "DayofYearSet.hpp"

namespace DSet{

    // empty constructor of DayofYear
    DayofYearSet::DayofYear::DayofYear(){
        this->_day = 1;
        this->_month = 1;
        calcHash();
    }

    // string data constructor of DayofYear
    DayofYearSet::DayofYear::DayofYear(string data, string type){ // 15/12 or 15.12 (day/month) default europe
        if(type == "us"){ // month/day
            string tempDay = "";
            string tempMonth = "";
            bool day = true;
            for(int idx = 0 ; idx < data.size() ; idx++){
                if(data[idx] == '.' || data[idx] == '/')
                    day = false;
                else if(day)
                    tempMonth += data[idx];
                else
                    tempDay += data[idx];
            }  
            _day = stoi(tempDay);
            _month = stoi(tempMonth); 
        }else{ // day/month
            string tempDay = "";
            string tempMonth = "";
            bool day = true;
            for(int idx = 0 ; idx < data.size() ; idx++){
                if(data[idx] == '.' || data[idx] == '/')
                    day = false;
                else if(day)
                    tempDay += data[idx];
                else
                    tempMonth += data[idx];
            }  
            _day = stoi(tempDay);
            _month = stoi(tempMonth); 
        }
        calcHash();
        checkInp();
    }

    // day and month data constructor of DayofYear
    DayofYearSet::DayofYear::DayofYear(int day, int month){
        this->_day = day;
        this->_month = month;
        calcHash();
        checkInp();
    }

    // day data constructor of DayofYear it takes day and returns to day and month
    DayofYearSet::DayofYear::DayofYear(int day){
        if(day <= 0){
            throw OutofDayException();
        }else if(day <= 31){
            this->_day = day;
            this->_month = 1;
        }else if(day <= 60){
            this->_day = day-31;
            this->_month = 2;
        }else if(day <= 91){
            this->_day = day-60;
            this->_month = 3;
        }else if(day <= 121){
            this->_day = day-91;
            this->_month = 4;
        }else if(day <= 152){
            this->_day = day-121;
            this->_month = 5;
        }else if(day <= 182){
            this->_day = day-152;
            this->_month = 6;
        }else if(day <= 213){
            this->_day = day-182;
            this->_month = 7;
        }else if(day <= 244){
            this->_day = day-213;
            this->_month = 8;
        }else if(day <= 274){
            this->_day = day-244;
            this->_month = 9;
        }else if(day <= 305){
            this->_day = day-274;
            this->_month = 10;
        }else if(day <= 335){
            this->_day = day-305;
            this->_month = 11;
        }else if(day <= 366){
            this->_day = day-335;
            this->_month = 12;
        }else throw OutofDayException();
        calcHash();
        checkInp();
    }
    
    // compares hashCodes || hashCodes are unique!
    bool DayofYearSet::DayofYear::operator==(const DayofYear & oData) const{
        return this->getHash() >= oData.getHash();
    }

    // calculates hashes of day and month 
    void DayofYearSet::DayofYear::calcHash(){
        _hash = 100 * _month + _day;
    }

    // checkes input is correct or not
    bool DayofYearSet::DayofYear::checkInp(){
        if  (_day > 31 || _day < 1) 
            throw OutofDayException();
        else if (_month > 12 || _month < 1)
            throw OutofMonthException();
        else if( _month == 2 && _day >= 30)
            throw OutofDayException();
        
        else if ( _day  == 31 && (_month == 4 || _month == 6 || _month == 9 || _month == 11 ))
            throw OutofDayException();
        
        return true;
    }

    // initializer list constructor of DayOfYearSet
    DayofYearSet::DayofYearSet(const initializer_list<DayofYear> data){
        this->_cap = 1;
        this->_size = 0;

        decltype(this->_cap) tempSize = data.size();

        for(;_cap <= tempSize;) _cap *= 2;

        _set = new DayofYear[_cap];

        for(auto d : data)
            this->addData(d);
    
    }

    // copy constructor of DayOfYearSet    BIG THREE !1/3
    DayofYearSet::DayofYearSet(DayofYearSet & copy){

        this->_cap = copy.getCap();
        this->_size = 0;

        _set = new DayofYear[_cap];

        for(int idx = 0; idx < copy._size; idx++)
            addData(copy._set[idx]);
    }

    // empty constructor of DayofYearSet
    DayofYearSet::DayofYearSet(){ 
        _size = 0; 
        _cap = 2; 
        _set = new DayofYear[_cap];
    } 

    // asignment operator overloading BIG THREE 2/3 deconstructor was initialized at hpp 3/3
    DayofYearSet DayofYearSet::operator=(const DayofYearSet & oData) {
        _cap = oData.getCap();
        _size = 0;
        delete[] _set;
        _set = new DayofYear[_cap];

        for(int idx = 0; idx < oData.size(); idx++)
            addData(oData._set[idx]);
        
        return *this;
    }   

    // compare overloading
    bool DayofYearSet::operator==(const DayofYearSet & oData) const{
        if( this->size() != oData.size() ) return false;
        
        int len = this->size();
      

        bool same = true;
        for(int c = 0; c < len && same; c++){
            same = false;
            int searchingElement = this->_set[c].getHash();

            for(int idx = 0; idx < len ; idx++)
                if(searchingElement == oData._set[idx].getHash())
                    same = true;
        }

        return same;
    }

    // addData function to add DayofYear data to set
    void DayofYearSet::addData(const DayofYear data){
        if(this->_size + 1 == this->_cap){
            this->_cap *= 2;
            DayofYear * temp = new DayofYear[_cap];

            for(int idx = 0; idx < this->_size; idx++)
                temp[idx] = _set[idx];

            delete [] _set;
            this->_set = temp;
        }

        bool isIn = false;

        // binary search can be added!
        for(int idx = 0; idx < this->_size; idx++)
            // if(_set[idx].getDay() == data.getDay() && _set[idx].getMonth() == data.getMonth()) // hash control?
            if(_set[idx].getHash() == data.getHash()) // hash control?
                isIn = true;

        if(!isIn)
            this->_set[(this->_size)++] = data;
        
    }

    // remove function to remove Dayofyear data from set
    void DayofYearSet::remove(const DayofYear & data) {

        DayofYear * temp = new DayofYear[this->_size+1];

        int isFinded = 0;

        for(int idx = 0; idx <= _size; idx++){

            if(this->_set[idx].getHash() == data.getHash())
                isFinded = -1;
            else
                temp[idx+isFinded] = _set[idx];
        }

        if(isFinded != 0) _size --;
        
        delete[] _set;
        this->_set = temp;
     
    }
    
    // operator overloadings
    DayofYearSet DayofYearSet::operator+(const DayofYear & data){ // adds an element
        DayofYearSet d({data});

        for(int idx = 0; idx < this->_size; idx++)  d.addData(this->_set[idx]);

        
        return d;
    }

    DayofYearSet DayofYearSet::operator+(const DayofYearSet & data){ // returns the union sets

        DayofYearSet d;

        for(int a = 0; a < _size; a++)
            d.addData(_set[a]);
        for(int a = 0; a < data._size; a++)
            d.addData(data._set[a]);
        

        return d;
    }

    DayofYearSet DayofYearSet::operator-(const DayofYear & data){ // returns the difference set
        DayofYearSet d;

        for(int idx = 0; idx < this->_size; idx++)  if(_set[idx].getHash() != data.getHash()) d.addData(this->_set[idx]);

        
        return d;
    }

    DayofYearSet DayofYearSet::operator-(const DayofYearSet & data){ // returns the difference set
        DayofYear * days = new DayofYear[_size];
        int count = 0;

        for(int idx = 0 ; idx < _size ; idx++){
            bool flag = false;
            for(int idx2 = 0 ; idx2 < data._size; idx2++)
                if(_set[idx].getHash() == data._set[idx2].getHash())
                    flag = true;
            if(!flag)
                days[count++] = _set[idx];
        }

        DayofYearSet d;

        for(int a = 0; a < count; a++)
            d.addData(days[a]);
        
        

        return d;

    }

    DayofYearSet DayofYearSet::operator^(const DayofYearSet & data)const { //  returns the intersection set.
        DayofYear * days = new DayofYear[_size];
        int count = 0;

        for(int idx = 0 ; idx < _size ; idx++){
            bool flag = false;
            for(int idx2 = 0 ; idx2 < data._size; idx2++)
                if(_set[idx].getHash() == data._set[idx2].getHash())
                    flag = true;
            if(flag)
                days[count++] = _set[idx];
        }

        DayofYearSet d;

        for(int a = 0; a < count; a++)
            d.addData(days[a]);
        
        

        return d;

    }

    DayofYearSet DayofYearSet::operator!()const{ // returns complements of set
        DayofYear * days = new DayofYear[365]; // -_size
        int count = 0;

        for(int idx = 1 ; idx <= 366 ; idx++){
            bool flag = false;
            DayofYear temp(idx);
            for(int idx2 = 0 ; idx2 < _size; idx2++)
                if(_set[idx2].getHash() == temp.getHash())
                    flag = true;
            if(!flag)
                days[count++] = temp;
        }

        DayofYearSet d;

        for(int a = 0; a < count; a++)
            d.addData(days[a]);
        
        
   
        return d;
    }
    
    DayofYearSet::DayofYear DayofYearSet::operator[](int idx){
        if (idx >= _size || idx < 0)
            throw DayofYear::OutofDayException();
        
        return _set[idx];
    }

};
