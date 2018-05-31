//
//  cond_queue.h
//  threading_examples
//
//  Created by Jack Campbell on 5/20/18.
//  Copyright © 2018 Jack Campbell. All rights reserved.
//

#ifndef cond_queue_h
#define cond_queue_h

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <typename T>
class cond_queue
{
public:
    cond_queue() {};
    cond_queue(const cond_queue& rhs)
    {
        std::lock_guard<std::mutex> lck(mtx);
        data = rhs.data;
    }
    cond_queue& operator=(const cond_queue&) = delete;
    
    // interface
    void push(T val)
    {
        std::lock_guard<std::mutex> lck(mtx);
        data.push(val);
        cond.notify_one();
    }
    
    bool try_pop(T& val)
    {
        std::lock_guard<std::mutex> lck(mtx);
        if(data.empty())
        {
            return false;
        }
        else
        {
            val = data.front();
            data.pop();
            return true;
        }
    }
    
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lck(mtx);
        
        if(data.empty())
        {
            return nullptr;
        }
        else
        {
            auto ret = std::make_shared<T>(data.front());
            data.pop();
            return ret;
        }
    }
    
    void wait_and_pop(T& val)
    {
        std::unique_lock<std::mutex> lck(mtx);
        auto start = std::chrono::high_resolution_clock::now();
        cond.wait(lck, [this, &start]{
            std::chrono::duration<double, std::micro> time = std::chrono::high_resolution_clock::now() - start;
            std::cout << "\nThis thread (" <<  std::this_thread::get_id() << ") waited " << time.count() << "ms to pop.\n";
            return !data.empty();
        });
        val = data.front();
        data.pop();
    }
    
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lck(mtx);
        auto start = std::chrono::high_resolution_clock::now();
        cond.wait(lck, [this, &start]{
            std::chrono::duration<double, std::micro> time = std::chrono::high_resolution_clock::now() - start;
            std::cout << "\nThis thread (" <<  std::this_thread::get_id() << ") waited " << time.count() << "ms to pop.\n";
            return !data.empty();
        });
        auto ret = std::make_shared<T>(data.front());
        data.pop();
        return ret;
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lck(mtx);
        return data.empty();
    }
    
private:
    std::queue<T> data;
    std::mutex mtx;
    std::condition_variable cond;
};

#endif /* cond_queue_h */
