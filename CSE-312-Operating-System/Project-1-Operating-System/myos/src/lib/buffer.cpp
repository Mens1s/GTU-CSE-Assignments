#include <lib/buffer.h>
#include <common/types.h>

using namespace myos::common;
using namespace myos::lib;


Buffer::Buffer()
{

}

Buffer::~Buffer()
{
}

uint8_t Buffer::buffer[256];
uint32_t Buffer::size = 256;
uint32_t Buffer::read = 0;
uint32_t Buffer::write = 0;
bool Buffer::done = false;
bool Buffer::doneMouse = false;
bool Buffer::inCriticalRegionn = false;

void Buffer::Write(uint8_t data)
{
    if(!IsFull())
    {
        buffer[write] = data;
        write = (write + 1) % size;
    }
}

uint8_t Buffer::Read()
{
    if(!IsEmpty())
    {
        uint8_t result = buffer[read];
        read = (read + 1) % size;
        return result;
    }
    return 10;
}

void Buffer::Clear()
{
    read = 0; 
    write = 0;
}

bool Buffer::IsEmpty()
{
    return read == write;
}

bool Buffer::IsFull()
{
    return (write + 1) % size == read;
}

bool Buffer::isDone(){
    return this->done;
}
bool Buffer::isDoneMouse(){
    return this->doneMouse;
}
void Buffer::setDone(bool done){
    this->done = done;
}
void Buffer::setDoneMouse(bool done){
    this->doneMouse = done;
}
bool Buffer::isCriticalRegion(){
    return this->inCriticalRegionn;
}

void Buffer::setCriticalRegion(bool inCriticalRegionn){
    this->inCriticalRegionn = inCriticalRegionn;
}