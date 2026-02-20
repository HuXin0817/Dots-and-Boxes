set(RobotLibrary "robot${BoardSize}x${BoardSize}")
set(RobotSources Robot.cpp)
add_library(${RobotLibrary} STATIC ${RobotSources})
target_compile_definitions(${RobotLibrary} PRIVATE
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${RobotLibrary} PRIVATE
  TBB::tbb
  ${CommonLibrary}
  ${ModelLibrary}
  ${BoardLibrary}
)
set(RobotLibrary ${RobotLibrary} PARENT_SCOPE)