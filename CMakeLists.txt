cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(DCMAKE_UNITY_BUILD ON)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(NOT MaxBoardSize)
  set(MaxBoardSize 36)
endif()

if(NOT DefaultBoardSize)
  set(DefaultBoardSize 6)
endif()

file(READ VERSION Version)
message(STATUS "Version: ${Version}")
message(STATUS "MaxBoardSize: ${MaxBoardSize}")
message(STATUS "DefaultBoardSize: ${DefaultBoardSize}")
add_compile_definitions(__Version__="${Version}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")
add_subdirectory(src)