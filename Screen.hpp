//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_SCREEN_HPP
#define PATHTRACER_SCREEN_HPP

#include <vector>
#include "Point.hpp"

class Screen{
private:
    uint64_t width;
    uint64_t  height;
    std::vector<unsigned char> data;
public:
    Screen():width(1000), height(1000){
        data.resize(4000000);
    }
    Screen(uint64_t  _width, uint64_t  _height):width(_width), height(_height){
        data.resize(4*_width * _height);
    }
    void setPixel(uint64_t  x, uint64_t  y, Point c);
    uint64_t getWidth() const;
    uint64_t getHeight() const;
    void clear();
    void save(const char *filename);
};

#endif //PATHTRACER_SCREEN_HPP
