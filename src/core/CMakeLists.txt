add_subdirectory(common "common${BoardSize}x${BoardSize}")
add_subdirectory(board "board${BoardSize}x${BoardSize}")
add_subdirectory(robot "robot${BoardSize}x${BoardSize}")

set(CommonLibraryName ${CommonLibraryName} PARENT_SCOPE)
set(BoardLibraryName ${BoardLibraryName} PARENT_SCOPE)
set(RobotLibraryName ${RobotLibraryName} PARENT_SCOPE)