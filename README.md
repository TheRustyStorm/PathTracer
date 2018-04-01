# PathTracer
A C++ Pathtracer with global illumination.
This is a small side project, so don't expect any major breakthroughs in Computer Graphics history.
It works reasonably well for my purposes.

If you want to create your own Scene, check out the bottom of Scene.cpp.
I commented out the code for random Directions on each Intersection and the code for PointLights. Feel free to comment them in if you
feel like it.

If you want to check out how much your CPU can handle, here are the things you can do to make the picture better.
1) Increase #NUM_SAMPLES in scene.cpp
2) Increase either of YART renderer = YART(3,1); in main.cpp

The first Parameter is the recursion depth. You want to have it at at least 3 to have some nice reflections.
The second Parameter is the amount of slightly different rays shot per pixel.

The example picture is done with 
- #NUM_SAMPLES 20
- recursion depth : 3
- sample amount : 1

and needed 45 seconds on a Macbook Pro
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

![alt text](https://github.com/TheSovietStorm/PathTracer/blob/master/Test.png)
