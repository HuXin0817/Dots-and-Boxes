cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME}
        Qt::Widgets
        OpenMP::OpenMP_CXX
)