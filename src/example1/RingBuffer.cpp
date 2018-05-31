//
//  RingBuffer.cpp
//  threading_examples
//
//  Created by Jack Campbell on 4/17/18.
//  Copyright © 2018 Jack Campbell. All rights reserved.
//

#include "RingBuffer.hpp"

// constructor
RingBuffer::RingBuffer() :
data{{}},
front(CIRCULAR_DATA_SIZE),
back(CIRCULAR_DATA_SIZE),
empty(true),
full(false)
{
    
};

// destructor
RingBuffer::~RingBuffer() {};

// push back
bool RingBuffer::PushBack(msgs val)
{
    mtx.lock();
    bool status = true;
    
    if(front != back || empty)
    {
        data[back.getIndex()] = val;
        back++;
        if(front == back)
        {
            full = true;
        }
        
    }
    else
    {
        status = false;
    }
    
    mtx.unlock();
    return status;
};

// pop front
bool RingBuffer::PopFront(msgs& val)
{
    mtx.lock();
    bool status = true;
    
    if(front != back || full)
    {
        val = data[front.getIndex()];
        front++;
        
        full = false;
        if(front == back)
        {
            empty = true;
        }
    }
    else
    {
        status = false;
    }
    
    mtx.unlock();
    return status;
    
}
