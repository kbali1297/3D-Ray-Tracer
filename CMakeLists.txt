cmake_minimum_required(VERSION 3.0.0)
project(2D_RAY_TRACER VERSION 0.1.0)

#include(CTest)
#enable_testing()

add_executable(2D_RAY_TRACER src/main.cpp src/Camera.cpp src/Pixel.cpp src/Screen.cpp src/Obstacles.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(2D_RAY_TRACER SDL2main SDL2 OpenMP::OpenMP_CXX)
endif()
set(CMAKE_CXX_FLAGS "-O2")
#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
