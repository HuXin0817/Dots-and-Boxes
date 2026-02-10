add_subdirectory(model "model_${BoardSize}x${BoardSize}")
add_subdirectory(board "board_${BoardSize}x${BoardSize}")
add_subdirectory(robot "robot_${BoardSize}x${BoardSize}")

set(ModelLibraryName ${ModelLibraryName} PARENT_SCOPE)
set(BoardLibraryName ${BoardLibraryName} PARENT_SCOPE)
set(RobotLibraryName ${RobotLibraryName} PARENT_SCOPE)