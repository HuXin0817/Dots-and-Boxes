set(RobotLibrary "robot${BoardSize}x${BoardSize}")
set(RobotSources 
    Robot.cpp
    GreedyRobot.cpp
    ImproveGreedyRobot.cpp
    MonteCarloRobot.cpp
    ParallelSearchRobot.cpp
    SimulationRobot.cpp
)
add_library(${RobotLibrary} STATIC ${RobotSources})
target_compile_definitions(${RobotLibrary} PRIVATE __BoardSize__=${BoardSize})
target_link_libraries(${RobotLibrary} PRIVATE TBB::tbb)
set(RobotLibrary ${RobotLibrary} PARENT_SCOPE)