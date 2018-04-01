# PathTracer
A C++ Pathtracer with global illumination.
This is a small side project, so don't expect any major breakthroughs in Computer Graphics history.
It works reasonably well for my purposes.

If you want to create your own Scene, check out the bottom of Scene.cpp.
I commented out the code for random Directions on each Intersection and the code for PointLights. Feel free to comment them in if you
feel like it.

If you want to check out how much your CPU can handle, here are the things you can do to make the picture better.
Go to config.cpp and edit the parameters you want to change.

- RECURSION_DEPTH 3 : How deep should the Raytracer recurse?
- PIXEL_SAMPLES 1 : How many samples per Pixel shall be made?
- RAY_SAMPLES 20 : How many samples per Ray shall be collected?
- IGNORE_EVERYTHING_BUT_EMISSION : If you define this, the program speeds up drastically, at the cost of spheres not casting correct lights. See the example pictures

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

- RECURSION_DEPTH 3 
- PIXEL_SAMPLES 1 
- RAY_SAMPLES 20 
- //IGNORE_EVERYTHING_BUT_EMISSION : This is not defined
- Needs around 12,63s seconds on my Macbook
![Correct Light](https://github.com/TheSovietStorm/PathTracer/blob/master/SLOW.png)

- RECURSION_DEPTH 3 
- PIXEL_SAMPLES 1 
- RAY_SAMPLES 20
- IGNORE_EVERYTHING_BUT_EMISSION : This is defined
- 55,44s on my Macbook
![Incorrect Light](https://github.com/TheSovietStorm/PathTracer/blob/master/FAST.png)


## Thankings
Thanks to [LodePNG](http://lodev.org/lodepng/) for their awesome work to simplify the creation of PNG files. 
You guys rock
