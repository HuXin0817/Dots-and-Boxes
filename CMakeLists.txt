cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

qt_add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets TBB::tbb)