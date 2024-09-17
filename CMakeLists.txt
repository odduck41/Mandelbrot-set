cmake_minimum_required(VERSION 3.29)
project(Mandelbrot_set)

set(CMAKE_CXX_STANDARD 26)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_executable(Mandelbrot_set main.cpp
        Complex.h
        Complex.cpp
        App.cpp
        App.h)

set(SFML_DIR "C:/Users/GDcat/CLionProjects/Mandelbrot_set/cmake-build-debug/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

target_link_libraries(Mandelbrot_set PRIVATE
        sfml-system
        sfml-network
        sfml-graphics
        sfml-window
)
