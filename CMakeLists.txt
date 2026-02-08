cmake_minimum_required(VERSION 3.16)
project(Dots_and_Boxes)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)

set(SOURCES
  main.cpp

  src/model/NearEdges.cpp
  src/model/NearBoxes.cpp
  src/model/SearchScoreMap.cpp

  src/board/AbsoluteScoreBoard.cpp
  src/board/ScoreableEdgeBoard.cpp
  src/board/EdgeCountableBoard.cpp
  src/board/RelativeScoreBoard.cpp
  src/board/BasicBoard.cpp

  src/robot/Robot.cpp
  src/robot/GreedyRobot.cpp
  src/robot/ImproveGreedyRobot.cpp
  src/robot/ParallelSearchRobot.cpp
  src/robot/SimulationRobot.cpp
  src/robot/MonteCarloRobot.cpp

  frontend/BoxCanvas.cpp
  frontend/CommandParser.cpp
  frontend/MainWindow.cpp
  frontend/EdgeCanvas.cpp
  frontend/BaseCanvas.cpp
  frontend/DotCanvas.cpp
)

foreach(BoardSize RANGE 1 36)
  set(Executable "${PROJECT_NAME}_${BoardSize}x${BoardSize}")
  qt_add_executable(${Executable} ${SOURCES})
  target_include_directories(${Executable} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${Executable} PRIVATE QT_NO_KEYWORDS BoardSize=${BoardSize})
  target_link_libraries(${Executable} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets TBB::tbb)
endforeach()