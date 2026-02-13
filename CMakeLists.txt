cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(NOT MaxBoardSize)
  set(MaxBoardSize 36)
endif()

file(READ VERSION Version)
message(STATUS "Version: ${Version}")
add_compile_definitions(__Version__="${Version}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")
add_subdirectory(src)