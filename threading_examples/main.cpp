//
//  main.cpp
//  threading_examples
//
//  Created by Jack Campbell on 5/20/18.
//  Copyright © 2018 Jack Campbell. All rights reserved.
//

#include <stdio.h>

#include <iostream>
#include <vector>
#include <thread>
#include "example1/RingBuffer.hpp"
#include "resources.h"
#include "example2/thread_stack.hpp"
#include <chrono>

using namespace std::chrono_literals;

static int const numThreads = 100;
    
    // exercise 1:
    // using bind syntax, print thread ids for 100 threads


void Print_Thread_Ids()
{
    std::vector<std::thread> threads(numThreads);
    for(int32_t i = 0; i < numThreads; i++)
    {
        threadPrinter printer;
        threads.emplace_back(printer);
        std::cout << "\n";
    }
    
    for(auto& thread : threads)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
}

void Detach_Shutdown()
{
    // exercise 2:
    // using a ring buffer, detach a thread and synchronize its shutdown via this thread
    // (simulate .join() behavior but on a detached thread)
    
    // who owns the ring buffer?
    RingBuffer* toSecondary = new RingBuffer();
    RingBuffer* toPrimary = new RingBuffer();
    std::thread t([&toSecondary, &toPrimary]{
        int32_t count = 0;
        
        msgs msg;
        while(toSecondary->PopFront(msg))
        {
            switch (msg)
            {
                case msgs::keep_going:
                    std::cout << "Count: " << count++ << "\n";
                    break;
                case msgs::kill_thread:
                    toPrimary->PushBack(msgs::kill_thread);
                    break;
                default:
                    break;
            }
        }
        std::this_thread::sleep_for(10ms);
    });
    
    t.detach();
    
    int32_t count = 0;
    while(count < 100)
    {
        toSecondary->PushBack(msgs::keep_going);
        count++;
    }
    
    toSecondary->PushBack(msgs::kill_thread);
    
    msgs msg = msgs::keep_going;
    while(msg != msgs::kill_thread)
    {
        toPrimary->PopFront(msg);
    }
    
    std::cout << "finished!\n";
}

// add prints
void Stack_Interleaved_Pop()
{
    // two threads...one stack
    thread_stack<vect4> s;
    
    // function that pushes 1000 vectors, pops 500, pushes 500, pops 1000
    auto fn = [&s](){
        for(float i = 0; i < 1000; i++)
        {
            s.push({1.0f/i, 2.0f/i, 3.0f/i, 4.0f/i});
            
        }
        
        for(float i = 0; i < 500; i++)
        {
            vect4 v = {0, 0, 0, 0};
            s.pop(v);
            v.print();
        }
        
        for(float i = 0; i < 500; i++)
        {
            s.push({1.0f/i, 2.0f/i, 3.0f/i, 4.0f/i});
        }
        
        for(float i = 0; i < 500; i++)
        {
            auto v = std::shared_ptr<vect4>();
            v = s.pop();
            v->print();
        }
    };
    
    std::thread t1(fn);
    std::thread t2(fn);
    
    t1.join();
    t2.join();
}

// add prints
void Queue_Interleaved_Pop()
{
    // two threads...one stack
    thread_stack<vect4> s;
    
    // function that pushes 1000 vectors, pops 500, pushes 500, pops 1000
    auto fn = [&s](){
        for(float i = 0; i < 1000; i++)
        {
            s.push({1.0f/i, 2.0f/i, 3.0f/i, 4.0f/i});
            
        }
        
        for(float i = 0; i < 500; i++)
        {
            vect4 v = {0, 0, 0, 0};
            s.pop(v);
            v.print();
        }
        
        for(float i = 0; i < 500; i++)
        {
            s.push({1.0f/i, 2.0f/i, 3.0f/i, 4.0f/i});
        }
        
        for(float i = 0; i < 500; i++)
        {
            auto v = std::shared_ptr<vect4>();
            v = s.pop();
            v->print();
        }
    };
    
    std::thread t1(fn);
    std::thread t2(fn);
    
    t1.join();
    t2.join();
}
    
int main()
{
    //Print_Thread_Ids();
    //Detach_Shutdown();
    Stack_Interleaved_Pop();
    return 0;
}




