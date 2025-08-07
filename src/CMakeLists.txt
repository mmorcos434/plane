cmake_minimum_required(VERSION 3.10)
project(SensitivityStudy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ROOT via root-config
find_program(ROOT_CONFIG_EXECUTABLE root-config)

if(NOT ROOT_CONFIG_EXECUTABLE)
    message(FATAL_ERROR "root-config not found! Please source thisroot.sh first.")
endif()

# Get flags
execute_process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --cflags
    OUTPUT_VARIABLE ROOT_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE ROOT_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

add_library(SensitivityStudy
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/SensStudy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/SensStudy.h
)

target_include_directories(SensitivityStudy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Apply flags
separate_arguments(ROOT_CFLAGS)
separate_arguments(ROOT_LIBS)
target_compile_options(SensitivityStudy PRIVATE ${ROOT_CFLAGS})
target_link_libraries(SensitivityStudy PRIVATE ${ROOT_LIBS})
target_include_directories(SensitivityStudy PUBLIC ../include)
