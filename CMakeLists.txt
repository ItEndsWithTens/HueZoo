### Basic info ###

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

if(WIN32)
  project(HueZoo CXX)
else()
  project(huezoo CXX)
endif()

set(SRCS src/lodepng/lodepng.h src/lodepng/lodepng.cpp src/main.cpp)

if(MSVC)
  set_property(SOURCE src/lodepng/lodepng.cpp
    PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_DEPRECATE)
endif()

add_executable(${PROJECT_NAME} ${SRCS})
