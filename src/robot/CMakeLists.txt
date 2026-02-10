set(RobotLibraryName "robot_${BoardSize}x${BoardSize}")
set(RobotSources
  GreedyRobot.cpp
  ImproveGreedyRobot.cpp
  MonteCarloRobot.cpp
  ParallelSearchRobot.cpp
  Robot.cpp
  SimulationRobot.cpp
)
add_library(${RobotLibraryName} SHARED ${RobotSources})
set_target_properties(${RobotLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/robot")
target_include_directories(${RobotLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${RobotLibraryName} PRIVATE __BoardSize__=${BoardSize})
target_link_libraries(${RobotLibraryName} PRIVATE TBB::tbb ${ModelLibraryName} ${BoardLibraryName})

set(RobotLibraryName ${RobotLibraryName} PARENT_SCOPE)