# PathTracer
A C++ Pathtracer with global illumination.
This is a small side project, so don't expect any major breakthroughs in Computer Graphics history.
It works reasonably well for my purposes.

If you want to create your own Scene, check out the bottom of Scene.cpp.
I commented out the code for random Directions on each Intersection and the code for PointLights. Feel free to comment them in if you
feel like it.

## Configuration

If you want to check out how much your CPU can handle, here are the things you can do to make the picture better.
Go to config.cpp and edit the parameters you want to change.

- RECURSION_DEPTH 3 : How deep should the Raytracer recurse?
- PIXEL_SAMPLES 1 : How many samples per Pixel shall be made?
- RAY_SAMPLES 20 : How many samples per Ray shall be collected?
- IGNORE_EVERYTHING_BUT_EMISSION : If you define this, the program speeds up drastically, at the cost of spheres not casting correct lights. See the example pictures
- IMAGE_WIDTH 1000 : Width of the Image in Pixel
- IMAGE_HEIGHT 700 : Height of the Image in Pixel

## Getting Started

Use gcc or clang, have CMake installed, clone the Repo and run
```
cmake CMakeLists.txt
```
to generate the Makefile and run 
```
make
```
to generate the executable.
Execute it with 
```
./PathTracer
```

## Examples

### Example config with mostly correct Light
- RECURSION_DEPTH 3 
- PIXEL_SAMPLES 1 
- RAY_SAMPLES 20 
- IMAGE_WIDTH 1000
- IMAGE_HEIGHT 700
- //IGNORE_EVERYTHING_BUT_EMISSION : This is not defined
- Needs around 12,63s seconds on my Macbook
![Correct Light](https://github.com/TheSovietStorm/PathTracer/blob/master/SLOW.png)

### Example config with simplified Light
- RECURSION_DEPTH 3 
- PIXEL_SAMPLES 1 
- RAY_SAMPLES 20
- IGNORE_EVERYTHING_BUT_EMISSION : This is defined
- IMAGE_WIDTH 1000
- IMAGE_HEIGHT 700
- 55,44s on my Macbook
![Incorrect Light](https://github.com/TheSovietStorm/PathTracer/blob/master/FAST.png)

### Example config for a wallpaper
- RECURSION_DEPTH 9
- PIXEL_SAMPLES 4
- RAY_SAMPLES 1000
- IGNORE_EVERYTHING_BUT_EMISSION
- IMAGE_WIDTH 2560
- IMAGE_HEIGHT 1600
- 18m 35s on my Macbook
![Retina Sized Wallpaper](https://github.com/TheSovietStorm/PathTracer/blob/master/Wallpaper.png)

### Example config for Fancy Light effects
- RECURSION_DEPTH 9
- PIXEL_SAMPLES 1
- RAY_SAMPLES 10
- //IGNORE_EVERYTHING_BUT_EMISSION : This is not defined
- IMAGE_WIDTH 2560
- IMAGE_HEIGHT 1600
- Around 20 Minutes on my Macbook
![Fancy Light effects](https://github.com/TheSovietStorm/PathTracer/blob/master/Test.png)

## Thankings
Thanks to [LodePNG](http://lodev.org/lodepng/) for their awesome work to simplify the creation of PNG files. 
You guys rock
