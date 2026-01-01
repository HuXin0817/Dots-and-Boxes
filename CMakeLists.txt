cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(ProcessorCount)
ProcessorCount(CPU_NUM)
if(CPU_NUM EQUAL 0)
    set(CPU_NUM 1)
endif()
add_compile_definitions(CPU_COUNT=${CPU_NUM})

find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

file(GLOB_RECURSE HEADERS "*.hpp")

add_executable(${PROJECT_NAME} main.cpp ${HEADERS})

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        OpenMP::OpenMP_CXX
)
