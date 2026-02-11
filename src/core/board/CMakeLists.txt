set(BoardLibraryName "board${BoardSize}x${BoardSize}")
set(BoardSources
  AbsoluteScoreBoard.cpp
  BasicBoard.cpp
  EdgeCountableBoard.cpp
  RelativeScoreBoard.cpp
  ScoreableEdgeBoard.cpp
)
add_library(${BoardLibraryName} STATIC ${BoardSources})
target_compile_definitions(${BoardLibraryName} PRIVATE __BoardSize__=${BoardSize} dab=dab${BoardSize}x${BoardSize})
target_link_libraries(${BoardLibraryName} PRIVATE ${ModelLibraryName})

set(BoardLibraryName ${BoardLibraryName} PARENT_SCOPE)