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
      
    option(
      HUEZOO_STATIC_CRT "Link statically with the C Run-Time libraries." TRUE
    )
  
    if(HUEZOO_STATIC_CRT)
      
      foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endforeach()
      
    else()
    
      foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
      endforeach()
      
    endif()
  
  endif()
  
endif()

add_executable(${PROJECT_NAME} ${SRCS})



### Packaging options ###

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

set(CPACK_SOURCE_IGNORE_FILES
  # Directories
  "\\\\.git"
  "build"
  "scratch"
  # Project files
  "gitignore$"
  # Source
  "\\\\.rc$"
  "~$"
  # Packages
  "zip$"
)

install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .
  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

install(FILES README.txt DESTINATION .)

# Build the source package by changing to your build directory and
# running: cpack --config CPackSourceConfig.cmake
set(CPACK_SOURCE_GENERATOR "ZIP")
if(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/dist)

set(CPACK_PACKAGE_VERSION_MAJOR ${HUEZOO_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${HUEZOO_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${HUEZOO_PATCH})
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  set(HUEZOO_ARCH x64)
else()
  set(HUEZOO_ARCH x86)
endif()

set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${HUEZOO_ARCH})

include(CPack)
