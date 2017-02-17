cmake_minimum_required(VERSION 2.8)
project(CppBF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o0 -std=c++11")

include_directories(inc/)

set(SRC_FILES src/expression.cpp 
              src/factory.cpp 
              src/parser.cpp 
              src/main.cpp )

add_executable(brainfuck ${SRC_FILES})
