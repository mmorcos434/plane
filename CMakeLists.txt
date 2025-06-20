cmake_minimum_required(VERSION 3.10)
project(sensitivity-study)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(sensitivity-study src/SensStudy.cpp)
