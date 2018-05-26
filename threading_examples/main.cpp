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
#include <condition_variable>
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
    std::cout << "\n ============ DETACHED RING BUFFER EXAMPLE: this thread is: " << std::this_thread::get_id() <<  " ===============\n";
    
    // who owns the ring buffer?
    RingBuffer* toSecondary = new RingBuffer();
    RingBuffer* toPrimary = new RingBuffer();
    std::condition_variable cond;
    
    std::mutex mtx;
    
    // start the background thread
    std::thread t([&toSecondary, &toPrimary, &mtx, &cond]{
        int32_t count = 0;
        std::cout << "\n ============ Starting extra thread:" << std::this_thread::get_id() << " ===============\n";
        msgs msg = msgs::keep_going;
        while(msg != msgs::kill_thread) //
        {
            std::cout << "\n ============ Reading messages ===============\n";
            
            if(toSecondary->PopFront(msg)) // usually more complex message handling going on here
            {
                if(msg == msgs::keep_going)
                {
                    std::lock_guard<std::mutex> lck(mtx);
                    std::cout << "Count: " << count++ << "\n";
                }
            }
            std::this_thread::sleep_for(1ms);
        }
        
        // Gonna kill this thread! Let the foreground thread know so it can complete/stop blocking.
        std::lock_guard<std::mutex> lck(mtx);
        toPrimary->PushBack(msgs::kill_thread);
        cond.notify_one();
    });
    
    t.detach();
    
    int32_t count = 0;

    while(count < 100)
    {
        toSecondary->PushBack(msgs::keep_going);
        count++;
    }

    std::cout << "\n ============ Sending kill message ===============\n";
    toSecondary->PushBack(msgs::kill_thread);
    std::cout << "\n ============ Sent kill message ===============\n";
    std::unique_lock<std::mutex> lck(mtx);
    cond.wait(lck, [&toPrimary]{
        msgs msg = msgs::keep_going;
        toPrimary->PopFront(msg);
        return msg == msgs::kill_thread;
    });
    lck.unlock();
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
    Print_Thread_Ids();
    Detach_Shutdown();
    Stack_Interleaved_Pop();
    return 0;
}




