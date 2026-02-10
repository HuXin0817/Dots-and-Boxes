set(BoardLibraryName "board${BoardSize}x${BoardSize}")
set(BoardSources
  AbsoluteScoreBoard.cpp
  BasicBoard.cpp
  EdgeCountableBoard.cpp
  RelativeScoreBoard.cpp
  ScoreableEdgeBoard.cpp
)
add_library(${BoardLibraryName} SHARED ${BoardSources})
set_target_properties(${BoardLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/board")
target_include_directories(${BoardLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${BoardLibraryName} PRIVATE __BoardSize__=${BoardSize})
target_link_libraries(${BoardLibraryName} PRIVATE ${ModelLibraryName})

set(BoardLibraryName ${BoardLibraryName} PARENT_SCOPE)