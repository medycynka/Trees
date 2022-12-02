cmake_minimum_required(VERSION 3.23)
project(Trees)

set(CMAKE_CXX_STANDARD 20)

file(GLOB trees "*.hpp")

add_executable(Trees main.cpp trees)
