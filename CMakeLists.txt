cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(TBB REQUIRED)

file(GLOB AUTOGEN_CPP_FILES "${CMAKE_SOURCE_DIR}/autogen/*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${AUTOGEN_CPP_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)
target_link_libraries(${PROJECT_NAME}
        Qt::Widgets
        TBB::tbb
)