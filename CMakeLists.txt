cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

set(LIBRARY_SOURCES
  src/model/Models.cpp
  src/model/SearchScoreMap.cpp

  src/board/AbsoluteScoreBoard.cpp
  src/board/BasicBoard.cpp
  src/board/EdgeCountableBoard.cpp
  src/board/RelativeScoreBoard.cpp
  src/board/ScoreableEdgeBoard.cpp

  src/robot/GreedyRobot.cpp
  src/robot/ImproveGreedyRobot.cpp
  src/robot/MonteCarloRobot.cpp
  src/robot/ParallelSearchRobot.cpp
  src/robot/Robot.cpp
  src/robot/SimulationRobot.cpp

  frontend/BaseCanvas.cpp
  frontend/BoxCanvas.cpp
  frontend/DotCanvas.cpp
  frontend/EdgeCanvas.cpp
  frontend/MainWindow.cpp
  frontend/Models.cpp

  extern/extern.cpp
)

if(NOT MaxBoardSize)
  set(MaxBoardSize 36)
endif()
message(STATUS "MaxBoardSize: ${MaxBoardSize}")

foreach(BoardSize RANGE 1 ${MaxBoardSize})
  set(LibraryName "${PROJECT_NAME}_${BoardSize}x${BoardSize}")
  qt_add_library(${LibraryName} SHARED ${LIBRARY_SOURCES})
  target_include_directories(${LibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${LibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
  target_link_libraries(${LibraryName} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets TBB::tbb)
endforeach()

qt_add_executable("${PROJECT_NAME}" main.cpp)
target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions("${PROJECT_NAME}" PRIVATE QT_NO_KEYWORDS MaxBoardSize=${MaxBoardSize})
target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)