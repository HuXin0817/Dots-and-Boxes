add_subdirectory(robot)
add_subdirectory(frontend)

set(CoreLibrary ${RobotLibrary} ${FrontendLibrary} PARENT_SCOPE)