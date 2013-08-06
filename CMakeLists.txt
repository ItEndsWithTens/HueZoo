### Basic info ###

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

if(WIN32)
  project(HueZoo CXX)
else()
  project(huezoo CXX)
endif()

set(SRCS src/main.cpp)

add_executable(${PROJECT_NAME} ${SRCS})
