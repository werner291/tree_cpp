cmake_minimum_required(VERSION 3.17)
project(tree_cpp)

set(CMAKE_CXX_STANDARD 20)

set(COPPELLIA_ROOT /home/werner/CoppeliaSim_Edu_V4_1_0_Ubuntu18_04)

include_directories(
        ${COPPELLIA_ROOT}/programming/include
        ${COPPELLIA_ROOT}/programming/simMath)

add_library(simExtTreeGen SHARED tree_ext.cpp tree_ext.hpp
        ${COPPELLIA_ROOT}/programming/common/simLib.cpp
        ${COPPELLIA_ROOT}/programming/simMath/3Vector.cpp
        ${COPPELLIA_ROOT}/programming/simMath/3X3Matrix.cpp
        ${COPPELLIA_ROOT}/programming/simMath/MyMath.cpp)