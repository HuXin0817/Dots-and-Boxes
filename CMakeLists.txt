cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UNITY_BUILD OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
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
  if(6 LESS ${MaxBoardSize})
    set(DefaultBoardSize 6)
  else()
    set(DefaultBoardSize ${MaxBoardSize})
  endif()
endif()

file(READ VERSION Version)
message(STATUS "Version: ${Version}")
message(STATUS "MaxBoardSize: ${MaxBoardSize}")
message(STATUS "DefaultBoardSize: ${DefaultBoardSize}")
message(STATUS "CPU Number: ${NUM_CPUS}")

include(ProcessorCount)
ProcessorCount(NUM_CPUS IGNORE_ERRORS)
add_compile_definitions(NUM_CPUS=${NUM_CPUS} __Version__="${Version}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(deps)

include_directories("${CMAKE_SOURCE_DIR}/src")

set(ExternLibraries)
foreach(BoardSize RANGE 1 ${MaxBoardSize})
  add_subdirectory(frontend "frontend/detail@${BoardSize}x${BoardSize}")
  list(APPEND ExternLibraries ${FrontendLibrary})
endforeach()

qt_add_executable("${PROJECT_NAME}" main.cpp)
target_compile_definitions("${PROJECT_NAME}" PRIVATE
  __MaxBoardSize__=${MaxBoardSize}
  __DefaultBoardSize__=${DefaultBoardSize}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
  nlohmann_json::nlohmann_json
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  spdlog::spdlog
  ${ExternLibraries}
)