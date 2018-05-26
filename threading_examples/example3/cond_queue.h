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

template <typename T>
class cond_queue
{
public:
    cond_queue() {};
    cond_queue(const cond_queue& rhs);
    cond_queue& operator=(const cond_queue&) = delete;
    
    // interface
    void push(T val);
    
    bool try_pop(T& val);
    std::shared_ptr<T> try_pop();
    
    void wait_and_pop(T& val);
    std::shared_ptr<T> wait_and_pop();
    
    bool empty() const
    {}
    
private:
    std::queue<T> data;
    std::mutex<T> mtx;
    std::condition_variable cond;
};

#endif /* cond_queue_h */
