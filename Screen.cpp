//
// Created by Peter Zdankin on 26.03.18.
//

#include "Screen.hpp"
#include "LodePNG.hpp"

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

void Screen::setPixel(uint64_t  x, uint64_t  y, Point c){
    data[4 * (width * y + x) + 0] = (uint8_t)(c.getX()* 255.0);
    data[4 * (width * y + x) + 1] = (uint8_t)(c.getY()* 255.0);
    data[4 * (width * y + x) + 2] = (uint8_t)(c.getZ()* 255.0);
    data[4 * (width * y + x) + 3] = (uint8_t)(255.0);
}
uint64_t  Screen::getWidth() const{
    return width;
}
uint64_t  Screen::getHeight() const{
    return height;
}
void Screen::clear(){
    for(auto& i: data){
        i = 0;
    }
}
void Screen::save(const char *filename){
    encodeOneStep(filename, data, width, height);
}