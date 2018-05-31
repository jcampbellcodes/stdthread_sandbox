//
//  thread_stack.hpp
//  threading_examples
//
//  Created by Jack Campbell on 5/20/18.
//  Copyright © 2018 Jack Campbell. All rights reserved.
//

#ifndef thread_stack_hpp
#define thread_stack_hpp

#include <memory>
#include <stack>
#include <mutex>
#include <iostream>

enum empty_result
{
    ok,
    not_ok
};

template<typename T>
class thread_stack
{
public:
    thread_stack()
    {};
    
    thread_stack(const thread_stack& other)
    {
        std::lock_guard<class std::mutex> lck(m);
        data = other.data;
    }
    
    thread_stack& operator=(const thread_stack&) = delete;
    
    void push(T val)
    {
        std::lock_guard<std::mutex> lck(m);
        data.push(val);
        std::cout << "\nThis thread ("<< std::this_thread::get_id() <<") is pushing.";
    }
    
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lck(m);
        if(!data.empty())
        {
            auto out = std::make_shared<T>(data.top());
            data.pop();
        std::cout << "\nThis thread ("<< std::this_thread::get_id() <<") is popping.";
            return out;
        }
        return std::shared_ptr<T>();
    }
    
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lck(m);
        if(!data.empty())
        {
            value = data.top();
            data.pop();
            std::cout << "\nThis thread ("<< std::this_thread::get_id() <<") is popping by reference.";
        }
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lck(m);
        return data.empty();
    }
    
private:
    std::stack<T> data;
    mutable std::mutex m;
    
};
#endif /* thread_stack_hpp */
