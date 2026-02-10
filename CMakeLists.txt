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

if(NOT MaxBoardSize)
  set(MaxBoardSize 36)
endif()
message(STATUS "MaxBoardSize: ${MaxBoardSize}")

foreach(BoardSize RANGE 1 ${MaxBoardSize})
  set(ModelLibraryName "model_${BoardSize}x${BoardSize}")
  set(ModelSources
    src/model/Models.cpp
    src/model/SearchScoreMap.cpp
  )
  qt_add_library(${ModelLibraryName} SHARED ${ModelSources})
  set_target_properties(${ModelLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  target_include_directories(${ModelLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${ModelLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
  target_link_libraries(${ModelLibraryName} PRIVATE Qt6::Core TBB::tbb)

  set(BoardLibraryName "board_${BoardSize}x${BoardSize}")
  set(BoardSources
    src/board/AbsoluteScoreBoard.cpp
    src/board/BasicBoard.cpp
    src/board/EdgeCountableBoard.cpp
    src/board/RelativeScoreBoard.cpp
    src/board/ScoreableEdgeBoard.cpp
  )
  qt_add_library(${BoardLibraryName} SHARED ${BoardSources})
  set_target_properties(${BoardLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  target_include_directories(${BoardLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${BoardLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
  target_link_libraries(${BoardLibraryName} PRIVATE Qt6::Core TBB::tbb ${ModelLibraryName})

  set(RobotLibraryName "robot_${BoardSize}x${BoardSize}")
  set(RobotSources
    src/robot/GreedyRobot.cpp
    src/robot/ImproveGreedyRobot.cpp
    src/robot/MonteCarloRobot.cpp
    src/robot/ParallelSearchRobot.cpp
    src/robot/Robot.cpp
    src/robot/SimulationRobot.cpp
  )
  qt_add_library(${RobotLibraryName} SHARED ${RobotSources})
  set_target_properties(${RobotLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  target_include_directories(${RobotLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${RobotLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
  target_link_libraries(${RobotLibraryName} PRIVATE Qt6::Core TBB::tbb ${ModelLibraryName} ${BoardLibraryName})

  set(FrontendLibraryName "frontend_${BoardSize}x${BoardSize}")
  set(FrontendSources
    frontend/BaseCanvas.cpp
    frontend/BoxCanvas.cpp
    frontend/DotCanvas.cpp
    frontend/EdgeCanvas.cpp
    frontend/MainWindow.cpp
    frontend/Models.cpp
  )
  qt_add_library(${FrontendLibraryName} SHARED ${FrontendSources})
  set_target_properties(${FrontendLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  target_include_directories(${FrontendLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${FrontendLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
  target_link_libraries(${FrontendLibraryName} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${ModelLibraryName} ${BoardLibraryName} ${RobotLibraryName})

  set(MainLibraryName "${PROJECT_NAME}_${BoardSize}x${BoardSize}")
  set(MainSources
    extern/extern.cpp
  )
  qt_add_library(${MainLibraryName} SHARED ${MainSources})
  set_target_properties(${MainLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  target_include_directories(${MainLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
  target_compile_definitions(${MainLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
  target_link_libraries(${MainLibraryName} PRIVATE 
    Qt6::Core Qt6::Gui Qt6::Widgets TBB::tbb
    ${ModelLibraryName}
    ${BoardLibraryName}
    ${RobotLibraryName}
    ${FrontendLibraryName}
  )
endforeach()

qt_add_executable("${PROJECT_NAME}" main.cpp)
target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions("${PROJECT_NAME}" PRIVATE QT_NO_KEYWORDS __MaxBoardSize__=${MaxBoardSize})
target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)