cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(TBB REQUIRED)

set(COMMON_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")
set(COMMON_COMPILE_DEFS PRIVATE QT_NO_KEYWORDS)
set(COMMON_LINK_LIBS PRIVATE Qt::Widgets TBB::tbb)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} ${COMMON_COMPILE_DEFS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LINK_LIBS})