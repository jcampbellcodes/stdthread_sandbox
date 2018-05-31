//
//  RingBuffer.hpp
//  threading_examples
//
//  Created by Jack Campbell on 4/17/18.
//  Copyright © 2018 Jack Campbell. All rights reserved.
//

#ifndef RingBuffer_hpp
#define RingBuffer_hpp

#include <stdio.h>
#include <cstdint>
#include <mutex>
#include <array>

enum msgs
{
    keep_going,
    kill_thread
};

class CircularIndex
{
public:
    CircularIndex(int32_t sz) : mask(sz - 1), index(0) {}
    
    // inc
    int operator++(int)
    {
        index++;
        index &= mask;
        return index;
    };
    
    // equality
    bool operator==(const CircularIndex& other) { return this->index == other.index; }
    
    // inequality
    bool operator!=(const CircularIndex& other) { return this->index != other.index; }
    
    // get index
    int32_t getIndex() const { return index; };
    
private:
    uint32_t mask;
    int32_t index;
};

const static int CIRCULAR_DATA_SIZE = 256;

class RingBuffer
{
public:
    RingBuffer();
    
    RingBuffer(const RingBuffer &) = delete;
    const RingBuffer &operator = (const RingBuffer &) = delete;
    ~RingBuffer();
    
    bool PushBack(msgs val);
    bool PopFront(msgs &val);
    
private:
    std::array<msgs, CIRCULAR_DATA_SIZE> data;
    CircularIndex front;
    CircularIndex back;
    bool empty;
    bool full;
    
    std::mutex mtx;
};

#endif /* RingBuffer_hpp */
