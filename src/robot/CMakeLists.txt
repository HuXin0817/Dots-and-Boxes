set(RobotLibrary "robot${BoardSize}x${BoardSize}")
set(RobotSources ScoreMap.cpp)
add_library(${RobotLibrary} STATIC ${RobotSources})
target_compile_definitions(${RobotLibrary} PRIVATE
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${RobotLibrary} PRIVATE TBB::tbb)
set(RobotLibrary ${RobotLibrary} PARENT_SCOPE)