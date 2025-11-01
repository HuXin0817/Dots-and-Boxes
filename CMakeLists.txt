cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenMP REQUIRED)
if (OpenMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

file(GLOB_RECURSE HEADERS "*.h")

add_executable(Dots_and_Boxes main.cpp ${HEADERS})

set_target_properties(Dots_and_Boxes PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)

target_link_libraries(Dots_and_Boxes
        Qt::Core
        Qt::Gui
        Qt::Widgets
        OpenMP::OpenMP_CXX
)
