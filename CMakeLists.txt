cmake_minimum_required(VERSION 3.20)
project(my_DB)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_DB main.cpp database.cpp database.h faculty.cpp faculty.h dbcontrol.h)
