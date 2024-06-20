#ifndef __MYOS__LIB__buffer_H
#define __MYOS__LIB__buffer_H

#include <common/types.h>

using namespace myos::common;

// data will come as integer for bineary or linear search so there is no need to string i thought
namespace myos
{
    namespace lib
    {
        
        class Buffer
        {
        public:
            Buffer();
            ~Buffer();
            
            void Write(uint8_t data);
            uint8_t Read();
            
            void Clear();
            bool IsEmpty();
            bool IsFull();
            bool isDone();
            void setDone(bool);
            bool isDoneMouse();
            void setDoneMouse(bool);
            bool isCriticalRegion();
            void setCriticalRegion(bool);
        private:
            static uint8_t buffer[256];
            static uint32_t size;
            static uint32_t read;
            static uint32_t write;
            static bool     done;
            static bool     doneMouse;
            static bool     inCriticalRegionn;
        };
    }
}

#endif