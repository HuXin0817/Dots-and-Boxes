set(RobotLibraryName "robot${BoardSize}x${BoardSize}")
set(RobotSources
  GreedyRobot.cpp
  ImproveGreedyRobot.cpp
  MonteCarloRobot.cpp
  ParallelSearchRobot.cpp
  Robot.cpp
  SimulationRobot.cpp
)
add_library(${RobotLibraryName} STATIC ${RobotSources})
target_compile_definitions(${RobotLibraryName} PRIVATE 
  __BoardSize__=${BoardSize} 
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${RobotLibraryName} PRIVATE TBB::tbb ${ModelLibraryName} ${BoardLibraryName})
set(RobotLibraryName ${RobotLibraryName} PARENT_SCOPE)