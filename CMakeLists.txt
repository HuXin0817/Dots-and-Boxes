cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

find_package(OpenMP REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
        UNITY_BUILD ON
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        OpenMP::OpenMP_CXX
)
