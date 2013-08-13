### Basic info ###

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

if(WIN32)
  project(HueZoo CXX)
else()
  project(huezoo CXX)
endif()

set(HUEZOO_MAJOR 0)
set(HUEZOO_MINOR 1)
set(HUEZOO_PATCH 0)
set(HUEZOO_VERSION ${HUEZOO_MAJOR}.${HUEZOO_MINOR}.${HUEZOO_PATCH})
set(HUEZOO_AUTHOR "Robert Martens")
set(HUEZOO_AUTHOR_EMAIL "<robert.martens@gmail.com>")
set(HUEZOO_DESCRIPTION "HSL/HSV color spectrum plotter")

set(SRCS src/lodepng/lodepng.h src/lodepng/lodepng.cpp src/main.cpp)

if(WIN32)
  
  configure_file(src/HueZoo.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/src/HueZoo.rc)  
  list(APPEND SRCS src/HueZoo.rc)
  
  if(MSVC)
    source_group("Resource Files" FILES src/HueZoo.rc)
    set_property(SOURCE src/lodepng/lodepng.cpp
      PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_DEPRECATE)
  endif()
  
endif()

add_executable(${PROJECT_NAME} ${SRCS})
