cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic --pedantic-errors")

project(cxxcalc)

add_executable(cxxcalc main.cpp Calculator.cpp)

#target_link_libraries(${CMAKE_PROJECT_NAME} )
