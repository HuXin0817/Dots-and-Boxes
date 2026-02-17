set(BoardLibrary "board${BoardSize}x${BoardSize}")
set(BoardSources
  AbsoluteScoreBoard.cpp
  BasicBoard.cpp
  EdgeCountableBoard.cpp
  LoggingBoard.cpp
  RelativeScoreBoard.cpp
  ScoreableEdgeBoard.cpp
)
add_library(${BoardLibrary} STATIC ${BoardSources})
target_compile_definitions(${BoardLibrary} PRIVATE
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${BoardLibrary} PRIVATE
  ${CommonLibrary}
  ${ModelLibrary}
)
set(BoardLibrary ${BoardLibrary} PARENT_SCOPE)