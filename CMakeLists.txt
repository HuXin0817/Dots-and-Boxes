cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UNITY_BUILD OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_UNITY_BUILD ON)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_UNITY_BUILD: ${CMAKE_UNITY_BUILD}")
message(STATUS "CMAKE_INTERPROCEDURAL_OPTIMIZATION: ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}")

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

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(ExternLibraries)
foreach(BoardSize RANGE 1 ${MaxBoardSize})
  add_subdirectory(src "src/detail@${BoardSize}x${BoardSize}")
  list(APPEND ExternLibraries ${ExternLibrary})
endforeach()

qt_add_executable("${PROJECT_NAME}" main.cpp)
target_compile_definitions("${PROJECT_NAME}" PRIVATE
  __Version__="${Version}"
  __MaxBoardSize__=${MaxBoardSize}
  __DefaultBoardSize__=${DefaultBoardSize}
)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${ExternLibraries})