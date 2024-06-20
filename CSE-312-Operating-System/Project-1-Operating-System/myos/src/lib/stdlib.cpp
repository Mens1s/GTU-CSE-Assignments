#include <common/types.h>
#include <lib/stdlib.h>
#include <lib/buffer.h>


using namespace myos::common;
using namespace myos::lib;


namespace myos {
        
    void printf(char* str)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;

        static uint8_t x=0,y=0;

        for(int i = 0; str[i] != '\0'; ++i)
        {
            switch(str[i])
            {
                case '\n':
                    x = 0;
                    y++;
                    break;
                default:
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                    x++;
                    break;
            }

            if(x >= 80)
            {
                x = 0;
                y++;
            }

            if(y >= 25)
            {
                for(y = 0; y < 25; y++)
                    for(x = 0; x < 80; x++)
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                x = 0;
                y = 0;
            }
        }
    }

    void printfHex(uint8_t key)
    {
        char* foo = "00";
        char* hex = "0123456789ABCDEF";
        foo[0] = hex[(key >> 4) & 0xF];
        foo[1] = hex[key & 0xF];
        printf(foo);
    }
    void printfHex16(uint16_t key)
    {
        printfHex((key >> 8) & 0xFF);
        printfHex( key & 0xFF);
    }
    void printfHex32(uint32_t key)
    {
        printfHex((key >> 24) & 0xFF);
        printfHex((key >> 16) & 0xFF);
        printfHex((key >> 8) & 0xFF);
        printfHex( key & 0xFF);
    }
    // buffer size must be 10 :D
    void intToString(int number, char* buffer) {
        // clear buffer
        for(int i = 0; i < 10; i++){
            buffer[i] = 0;
        }
        int i = 0;
        if (number == 0) {
            buffer[i++] = '0';
            buffer[i] = '\0';
            return;
        }
        if (number < 0) {
            buffer[i++] = '-';
            number = -number;
        }
        int tmp = number;
        while (tmp != 0) {
            int digit = tmp % 10;
            buffer[i++] = digit + '0'; // Convert digit to character
            tmp /= 10;
        }
        buffer[i] = '\0';

        // Reverse the string
        int len = i;
        for (int j = 0; j < len / 2; j++) {
            char temp = buffer[j];
            buffer[j] = buffer[len - j - 1];
            buffer[len - j - 1] = temp;
        }
    }

    int getIntegerFromKeyboard(char *str){
        Buffer* buff = new Buffer();
        buff->Clear();
        int key = 0;
        int i = 0;
        int multiplier = 1;
        buff->setCriticalRegion(true);

        // critical region
        printf("\nEnter a number for ");
        printf(str);
        printf(": ");
        while(!(buff->isDone())){
            int8_t key = buff->Read();

            if(key != 10) {
                int num = static_cast<int>(key);
                char* buffer = new char[10];
                intToString(num, buffer);
                printf(buffer);
                i = i * multiplier + num;
                multiplier *= 10;

            }
        }
        buff->setDone(false);
        
        printf("\n\nYou entered: ");
        char* buffer = new char[10];
        intToString(i, buffer);
        printf(buffer);
        printf("\n");
        buff->setCriticalRegion(false);
        // end of critical region
        return i;
    }

    void linearSearch(){
        int arr[10] = {10,20,30,40,50,60,70,80,90,100};
        int i;  
        Buffer* buff = new Buffer();    

        printf("Array is: \n{ ");
        for(i = 0; i < 10; i++){
            char buffer[10];
            intToString(arr[i], buffer);
            printf(buffer);
            if(i != 9){
                printf(", ");
            }
        }
        printf(" }\n");

        int key = getIntegerFromKeyboard("Linear Search");

        for(i = 0; i < 10; i++){
            if(arr[i] == key){
                printf("Element found at index: ");
                char buffer[10];
                intToString(i, buffer);
                printf(buffer);
                printf("\n");
                break;
            }
        }
        if(i == 10){
            printf("Element not found\n");
        }

    }

    // it works as expected it gives 01/02 as output
    // now i have to set write/read 0 when i get to input 
    // after input is done user enter key then i will read with using like buffer[0]buffer[1]...
    void readBuffer(){
        Buffer *buffer = new Buffer();
        while(1){
            int8_t key = buffer->Read();
            if(key != 0) {
                printf("Key: ");
                printfHex(key);
                printf("KeyEND\n");
                
            }
        }
    }

    void long_running_program(){
        int n = 100;
        int r = 0;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                r += (i*j);
            }
        }
        // return r;
    }

    void collatz(){
        int i = getIntegerFromKeyboard("Collatz sequence");

        printf("Collatz sequence: \n");
        while(i != 1){
            if(i % 2 == 0){
                i = i / 2;
            } else {
                i = 3 * i + 1;
            }
            char* buffer = new char[10];
            intToString(i, buffer);
            printf(buffer);
            printf("\n");
        }
        printf("End of Collatz sequence\n");
    }
    void collatzVeryLong(){
        int i = getIntegerFromKeyboard("Collatz very long sequence");

        printf("Collatz very long sequence: \n");
        while(i != 1){
            if(i % 2 == 0){
                i = i / 2;
            } else {
                i = 3 * i + 1;
            }
            char* buffer = new char[10];
            intToString(i, buffer);
            printf(buffer);
            printf("\n");
        }
        
        for(int j=0;j<300000;j++){
            for(int q=0;q<2123;q++){
                i=i;
            }
        }

        printf("End of Collatz very longsequence\n");
    }
    void binarySearch(){
        int arr[10] = {10,20,30,40,50,60,70,80,90,100};
        int i;  
        Buffer* buff = new Buffer();    

        printf("Array is: \n{ ");
        for(i = 0; i < 10; i++){
            char buffer[10];
            intToString(arr[i], buffer);
            printf(buffer);
            if(i != 9){
                printf(", ");
            }
        }
        printf(" }\n");

        int key = getIntegerFromKeyboard("Binary Search");

        int low = 0;
        int high = 9;
        int mid;
        while(low <= high){
            mid = (low + high) / 2;
            if(arr[mid] == key){
                printf("Element found at index: ");
                char buffer[10];
                intToString(mid, buffer);
                printf(buffer);
                printf("\n");
                break;
            } else if(arr[mid] < key){
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        if(low > high){
            printf("Element not found\n");
        }
    }

    int random(int min, int max){
        uint32_t state = 777;      
        state = state * 1664425 + 1013904223;
        return (state % (max - min + 1)) + min;
    }

    static unsigned long int next = 1;

    int rand(int max, int min) // RAND_MAX assumed to be 32767
    {
        next = next * 1103515245 + 12345;
        next = next / 65536;
        next = next % (max+1);
        
        if(next < min)
            next += min;
        
        return (unsigned int)(next/65536) % (max+1);
    }

}