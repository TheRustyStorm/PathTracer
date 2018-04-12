#include "Screen.hpp"
#include "Camera.hpp"
#include "YART.hpp"
#include "config.hpp"

int main(){
    std::srand(time(nullptr));
    Screen screen(IMAGE_WIDTH,IMAGE_HEIGHT);
    Camera camera = Camera();
    camera.setEyePoint(Point(0.0,1.0,3.0));
    camera.setLookAt(Point(0.0,0.0,0.0));

    YART renderer = YART(RECURSION_DEPTH,PIXEL_SAMPLES);
    renderer.setCamera(camera);
    Scene s = genScene();
    renderer.setScene(s);
    renderer.render(screen);
    screen.save("Test.png");


    return 0;
}
