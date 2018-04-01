#include "Screen.hpp"
#include "Camera.hpp"
#include "YART.hpp"

int main(){
    std::srand(std::time(nullptr));
    Screen screen(1000,700);
    Camera camera = Camera();
    camera.setEyePoint(Point(0.0,1.0,3.0));
    camera.setLookAt(Point(0.0,0.0,0.0));

    YART renderer = YART(3,1);
    renderer.setCamera(camera);
    Scene s = genScene();
    renderer.setScene(s);
    uint64_t  height  = screen.getHeight();
    uint64_t  width = screen.getWidth();
    renderer.render(screen);
    screen.save("Test.png");


    return 0;
}