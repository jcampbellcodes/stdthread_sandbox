//
//  resources.h
//  threading_examples
//
//  Created by Jack Campbell on 5/20/18.
//  Copyright © 2018 Jack Campbell. All rights reserved.
//

#ifndef resources_h
#define resources_h

#include <iostream>

class vect4
{
public:
    vect4(): x(0), y(0), z(0), w(0) {};
    vect4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
    vect4(const vect4&) = default;
    vect4& operator=(const vect4&) = default;
    ~vect4() {};
    
    vect4 add(const vect4& v)
    {
        return {v.x+x, v.y+y, v.z+z, v.w+w};
    };
    
    vect4 sub(const vect4& v)
    {
        return {x-v.x, y-v.y, z-v.z, w-v.w};
    };
    
    float dot(const vect4& v)
    {
        return (v.x*x) + (v.y*y) + (v.z*z) + (v.w*w);
    };
    
    void print()
    {
        printf("\n     ======> x: %f, y: %f, z: %f, w: %f", x, y, z, w);
    };
    
private:
    
    float x;
    float y;
    float z;
    float w;
    
};

class threadPrinter
{
public:
    void operator()()
    {
        std::cout << "\nthread:" << std::this_thread::get_id();
    };
};

#endif /* resources_h */
