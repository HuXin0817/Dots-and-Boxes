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

foreach(i RANGE 1 36)
    set(GEN_FILE "${CMAKE_SOURCE_DIR}/autogen/MainWindow${i}.cpp")
    file(WRITE "${GEN_FILE}"
"#include \"Frontend.hpp\"

template class dab::detail::frontend::MainWindow<${i}>;
")
endforeach()

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/autogen/*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
)

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
