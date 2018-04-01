cmake_minimum_required(VERSION 3.8)
project(PathTracer)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCE_FILES
        "*.hpp"
        "*.cpp"
        )
set(CMAKE_CXX_FLAGS "-O3 -std=c++0x -pthread -pedantic -Wall -Wextra -Wcast-align -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs  -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused")

add_executable(PathTracer ${SOURCE_FILES})