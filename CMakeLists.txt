cmake_minimum_required(VERSION 3.28)
project(IniManager)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(IniManager
        IniFile.cpp
        IniFile.h
        main.cpp)
