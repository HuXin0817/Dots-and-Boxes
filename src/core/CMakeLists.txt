add_subdirectory(model "model${BoardSize}x${BoardSize}")
add_subdirectory(board "board${BoardSize}x${BoardSize}")
add_subdirectory(robot "robot${BoardSize}x${BoardSize}")

set(ModelLibraryName ${ModelLibraryName} PARENT_SCOPE)
set(BoardLibraryName ${BoardLibraryName} PARENT_SCOPE)
set(RobotLibraryName ${RobotLibraryName} PARENT_SCOPE)