#include "PFArray.cpp"


int main() {
    PFArray<int> arr;

    // Test push_back function
    arr.push_back(0);
    arr.push_back(4);
    arr.push_back(23);
    arr.push_back(435);
    arr.push_back(1312);
    arr.push_back(4);
    arr.push_back(4356);
    arr.push_back(31);
    arr.push_back(454);
    arr.push_back(412);
    arr.push_back(554);
    arr.push_back(51);
    arr.push_back(6543);
    arr.push_back(7645);
    arr.push_back(73);
    arr.push_back(71);
    arr.push_back(7456);
    arr.push_back(428);
    arr.push_back(19);

    cout << "Testing Size, empty and iterator : " << endl;
    cout << "Size: " << arr.size() << endl; 
    cout << "Empty: " << arr.empty() << "\t[0 is false 1 is true]"<< endl << endl; 

    //  iterators tests
    cout << "Elements: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl; 
    
    cout << "Testing erase functions (3 overloaded) [ 1-) Iterator<T> , Iterator<T>  : 2-) Iterator<T> : 3-) Value] " << endl << endl;    
    // Test erase first function
    cout << "Testing [ Iterator<T> , Iterator<T> ]  : [begin() + 2, begin() + 5]" << endl;
    arr.erase(arr.begin() + 2, arr.begin() + 5);
    
    cout << "Elements after erase: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) cout << *it << " ";
    cout << endl << endl;
    // Test erase second function
    cout << "Testing [ Iterator<T> ]  : [ begin() + 5 ] " << endl;
    arr.erase(arr.begin() + 5);

    cout << "Elements after erase: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) cout << *it << " ";
    cout << endl << endl;
    // Test erase third function
    cout << "Testing [ Value ]  : [51]" << endl;
    arr.erase(51);

    cout << "Elements after erase: " << endl;
    for (auto it = arr.begin(); it != arr.end(); ++it) cout << *it << " ";
    cout << endl << endl;

    cout << "Elements after erase: [ testing cbegin and cend ]" << endl;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) cout << *it << " ";
    cout << endl << endl;


    
    // testing for each loop
    cout << "Testing for each loop "<< endl;
    for(auto x : arr)
        cout << x << " ";
    cout << endl << endl;
    // end of testin for each loop

    // testing sort function
    cout << "Testing for sort function "<< endl;
    sort(arr.begin(), arr.end());
    for(auto x : arr)
        cout << x << " ";
    cout << endl << endl;
    // end of testing sort function

    // testing find function
    int target = 6543;
    cout << "Testing for find function "<< endl;
    int findedLoc = find(arr.begin(), arr.end(), target);
    if (findedLoc != -1) cout << "Element " << target <<" found at position : " << findedLoc << " \n" ;
    cout << endl << endl;
    // end of testing find function

    cout << endl << "Size after erase: " << arr.size() << endl;

    // Test clear function
    arr.clear();
    cout << "Size after clear: " << arr.size() << endl; 
    cout << "Empty after clear: " << arr.empty() << endl; 

    cout << endl << "Testing try except functions in erase function : " << endl << endl;
    
    try{
        arr.erase(arr.begin() + 5 , arr.begin() + 3 );
    }catch(IteratorInf ex){
        cout << ex.what() << endl;
    }

    try{
        arr.erase( arr.begin() + 12 , arr.begin() + 12 );
    }catch(IteratorCannotFinded ex){
        cout << ex.what() << endl;
    }
    
    return 0;   
}