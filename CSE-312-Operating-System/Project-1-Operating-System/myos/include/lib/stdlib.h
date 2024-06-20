#ifndef __STDLIB_H
#define __STDLIB_H

#include <common/types.h>
#include <lib/buffer.h>

using namespace myos::common;
using namespace myos::lib;

namespace myos{
    
    void printf(char* str);
    void printfHex(uint8_t key);
    void printfHex16(uint16_t key);
    void printfHex32(uint32_t key);
    int  getIntegerFromKeyboard(char *str);
    void linearSearch();
    void readBuffer();
    void intToString(int number, char* buffer);
    void long_running_program();
    void collatz();
    void collatzVeryLong();
    void binarySearch();
    int random(int min, int max);
}

#endif