#include <fstream>
#include <stdexcept>
#include "DayofYearSet.cpp"
using namespace DSet;

void testIT();

int main(){

    try{
        testIT();

    }catch(DayofYearSet::DayofYear::OutofDayException e){

        cout << e.what();
    }catch(std::invalid_argument e){

        cout << e.what(); // stoi
    }catch(DayofYearSet::DayofYear::OutofMonthException e){

        cout << e.what();
    }

    return 0;
}

void testIT(){
        // set1
        
        DayofYearSet::DayofYear firstJan("1/1");
        DayofYearSet::DayofYear thirdFeb("3/2");
        DayofYearSet::DayofYear tFourthMar("14/03");
        DayofYearSet::DayofYear tSixthApr("16/4");
        DayofYearSet::DayofYear sFirstMay("21/05");

        // set2
        DayofYearSet::DayofYear sFifthJun("25/06");
        DayofYearSet::DayofYear tFifthJuly("15/07");
        DayofYearSet::DayofYear tSixthJuly("16/07");
        DayofYearSet::DayofYear tSixthJulyyDublicate("16/07");
        DayofYearSet::DayofYear secondSept("2/09");
        DayofYearSet::DayofYear thirdSept("3/9");
        DayofYearSet::DayofYear firstOct("1/10");


        // additional dayofyear
	    decltype(firstOct) firstNov("1/10");
        //DayofYearSet::DayofYear firstNov("1/10");
        DayofYearSet::DayofYear secondDec("31/12");

        // wrong typed dayofyear object
        //DayofYearSet::DayofYear wtdO("3I/12");

        // outofdayexception objects
        //7DayofYearSet::DayofYear ood1("30/2");
        //DayofYearSet::DayofYear ood2("-5/10");

        // outofmonthexception objects
        //DayofYearSet::DayofYear oom1("30/22");
        //DayofYearSet::DayofYear oom2("5/-5");

        // result set object
        DayofYearSet result;    
     
        // operation sets
        DayofYearSet set1({firstJan, thirdFeb, tFourthMar,tSixthApr, sFirstMay});
        DayofYearSet equalSet1({thirdFeb, tFourthMar, firstJan ,tSixthApr, tFourthMar, sFirstMay}); // it also inculeded duplicates

        DayofYearSet set2({sFifthJun, tFifthJuly,tSixthJuly,tSixthJulyyDublicate, secondSept, thirdFeb, firstOct});
        ofstream res("results.txt");
        // testing << operator overloading
        cout << "Testing operator << overloading " << endl << endl; res << "Testing operator << overloading " << endl << endl;

        cout << "(TEST1)Obj 1 : " << endl; res << "(TEST1)Obj 1 : " << endl;
        cout << set1; res << set1;
        cout << endl << "(TEST2)Obj 2 : " << endl; res << endl << "(TEST2)Obj 2 : " << endl;
        cout << set2 << endl; res << set2 << endl;

        // testing == != operator overloadings
        cout << "Testing operator == and != overloading " << endl << endl; res << "Testing operator == and != overloading " << endl << endl;

        cout << "(TEST1)set1 ==  eqaulSet1 they are different sets but same objects also includes duplicates too" << endl; res << "(TEST1)set1 ==  eqaulSet1 they are different sets but same objects also includes duplicates too" << endl;
        if(set1 == equalSet1){
            cout << "They are equal!" << endl << endl; res << "They are equal!" << endl << endl;}
        else{
            cout << "They are not equal!" << endl; res << "They are not equal!" << endl;cout << "They are not equal!" << endl;}

        cout << "(TEST2)set1 !=  eqaulSet1 they are different" << endl; res << "(TEST2)set1 !=  eqaulSet1 they are different" << endl;
        if(set1 != equalSet1)   
            {cout << "They are not equal!" << endl << endl; res << "They are not equal!" << endl << endl;}
        else
            {cout << "They are equal!" << endl; res << "They are equal!" << endl;}

        // testing remove functions 
        cout << "Testing remove functions... " << endl << endl; res << "Testing remove functions... " << endl << endl;
        cout << "(3 TEST)Before remove func... (Also testing removing non member dayofyear)" << endl << set1 << endl; res << "(3 TEST)Before remove func... (Also testing removing non member dayofyear)" << endl << set1 << endl; 
        cout << "Removing : " << endl; res << "Removing : " << endl;
        firstJan.print();
        sFifthJun.print();
        set1.remove(firstJan);
        set1.remove(firstJan);
        set1.remove(sFifthJun);
        cout << endl << "After remove func..." << endl << set1; res << endl << "After remove func..." << endl << set1;
 
        // testing size function

        cout << "(2 TEST)Testing size functions... " << endl << endl; res << "(2 TEST)Testing size functions... " << endl << endl;
        cout << "Set1 size : " << set1.size() << endl << endl; res << "Set1 size : " << set1.size() << endl << endl;
        cout << "Set2 size : " << set2.size() << endl << endl; res << "Set2 size : " << set2.size() << endl << endl;

        // Testing operator+ ( set + dayofyear)
        cout << "Testing operartor+ overloading..." << endl << endl; res << "Testing operartor+ overloading..." << endl << endl;
        cout << "Before adding an dayofyear element... (Also try adding 3 same dayofyear)" << endl; res << "Before adding an dayofyear element... (Also try adding 3 same dayofyear)" << endl;
        cout << set1; res << set1;
        cout << endl << "(4 TEST)Adding ... 15/06 twice.."  << endl; res << endl << "(4 TEST)Adding ... 15/06 twice.."  << endl;

        sFifthJun.print(); tFifthJuly.print(); tSixthJuly.print();

        set1 = set1 + sFifthJun;
        set1 = set1 + sFifthJun;
        set1 = set1 + tFifthJuly;
        set1 = set1 + tSixthJuly;
        cout << "After adding..." << endl << endl; res << "After adding..." << endl << endl;
        cout << set1 << endl; res << set1 << endl;

        // Testing operator+ ( set + set)
        cout << "(2 TEST)Testing operartor+ overloading..." << endl << endl; res << "(2 TEST)Testing operartor+ overloading..." << endl << endl;
        cout << "Before additional op: " << endl; res << "Before additional op: " << endl;

        cout << "(1. TEST)set1 : "<< endl <<set1; res << "(1. TEST)set1 : "<< endl <<set1;
        cout << "(1. TEST)set2 : "<< endl <<set2; res << "(1. TEST)set2 : "<< endl <<set2;
        cout << endl << "(1. TEST)Adding..."  << endl; res << endl << "(1. TEST)Adding..."  << endl;
 
        result = set1 + set2;

        cout << "(1. TEST)After adding..." << endl << endl; res << "(1. TEST)After adding..." << endl << endl;
        
        cout << result; res << result;

        cout << "Before additional op: " << endl; res << "res additional op: " << endl;

        cout << "(2. TEST)set1 : "<< endl <<result; res << "(2. TEST)set1 : "<< endl <<result;
        cout << "(2. TEST)set2 : "<< endl <<set2;  res << "(2. TEST)set2 : "<< endl <<set2; 
        cout << endl << "(2. TEST)Adding..."  << endl; res << endl << "(2. TEST)Adding..."  << endl;

        result = result + set2;

        cout << "(2. TEST)After adding..." << endl << endl; res << "(2. TEST)After adding..." << endl << endl;

        cout << result;

        // Testing operator- ( set - dayofyear)
        cout << "Testing operartor- overloading..." << endl << endl; res << "Testing operartor- overloading..." << endl << endl;
        cout << "Before Substraction an dayofyear element... (Also try adding 2 same dayofyear)" << endl; res << "Before Substraction an dayofyear element... (Also try adding 2 same dayofyear)" << endl;
        cout << set1;  res << set1;
        cout << endl << "(4 TEST)Substraction ... 15/06 twice.."  << endl; res << endl << "(4 TEST)Substraction ... 15/06 twice.."  << endl;

        sFifthJun.print(); tFifthJuly.print(); tSixthJuly.print();

        set1 = set1 - sFifthJun;
        set1 = set1 - sFifthJun;
        set1 = set1 - tFifthJuly;
        set1 = set1 - tSixthJuly;
        cout << "After Substraction..." << endl << endl; res << "After Substraction..." << endl << endl;
        cout << set1 << endl;  res << set1 << endl; 

        // Testing operator+ ( set + set)
        cout << "Testing operartor- overloading..." << endl << endl; res << "Testing operartor- overloading..." << endl << endl;
        cout << "Before Substraction op: " << endl; res << "Before Substraction op: " << endl;

        cout << "set1 : "<< endl <<set1;  res << "set1 : "<< endl <<set1;
        cout << "set2 : "<< endl <<set2;  res << "set2 : "<< endl <<set2;
        cout << endl << "Substraction..."  << endl; res << endl << "Substraction..."  << endl;

        result = set1 - set2;

        cout << "After Substraction... Difference of set..." << endl << endl; res << "After Substraction... Difference of set..." << endl << endl;
        cout << result; res << result;

        // testing operator! 
        cout << "Testing operator! overloading..." << endl << endl; res << "Testing operator! overloading..." << endl << endl;
        cout << "Set2: " << endl << set2; res << "Set2: " << endl << set2;
        cout << "Complement of set : " << endl << endl; res << "Complement of set : " << endl << endl;
        cout << !set2; res << !set2;

        //testing operator[] 
        cout << endl <<"(2TEST)Testing []operator with set1 gets 2 and 1 indexes.." << endl; res << endl <<"(2TEST)Testing []operator with set1 gets 2 and 1 indexes.." << endl;
        cout << "Set1 : " << endl; res << "Set1 : " << endl;
        cout << set1; res << set1;
        
        cout << "Getting 2. idx " << endl;      res << "Getting 2. idx " << endl;
        cout << set1[2]; res << set1[2];
        cout << "Getting 1. idx " << endl;      res << "Getting 1. idx " << endl;
        cout << set1[1]; res << set1[1];
        res.close();
} 
