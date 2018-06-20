#include "Screen.hpp"
#include "Camera.hpp"
#include "YART.hpp"
#include "config.hpp"
#include "MultiCoreYART.hpp"
#include <thread>

int main(){
    std::srand(time(nullptr));
    Screen screen(IMAGE_WIDTH,IMAGE_HEIGHT);
    Camera camera = Camera();
    camera.setEyePoint(Point(0.0,1.0,5.0));
    //camera.setEyePoint(Point(0,0,2));
    camera.setLookAt(Point(0.0,0.0,0.0));

    YART renderer = YART(RECURSION_DEPTH,PIXEL_SAMPLES);

    //MultiCoreYART *yart = new MultiCoreYART(RECURSION_DEPTH, PIXEL_SAMPLES);
    //yart->setCamera(camera);

    renderer.setCamera(camera);
  //  Scene s = genScene();
    //Scene s = genOtherScene();
    //Scene s = genSimpleScene();
//    Scene s = genBoxScene();
//    Scene s = genWeirdBoxScene();
      Scene s = genSmallptScene();
  //  Scene s = genPlaneScene();
   // Scene s = genStevenScene();

    renderer.setScene(s);
    renderer.render(screen);

    //yart->setScene(s);
    //std::thread th(&MultiCoreYART::render, &yart, screen, 0, screen.getHeight() );


    screen.save("Test.png");


    return 0;
}
