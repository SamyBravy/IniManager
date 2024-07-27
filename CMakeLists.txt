cmake_minimum_required(VERSION 3.28)
project(IniManager)

add_subdirectory(test)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES IniFile.cpp IniFile.h)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
