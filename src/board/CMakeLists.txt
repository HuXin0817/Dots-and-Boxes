set(BoardLibrary "board${BoardSize}x${BoardSize}")
set(BoardSources Board.cpp)
add_library(${BoardLibrary} STATIC ${BoardSources})
target_compile_definitions(${BoardLibrary} PRIVATE
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
set(BoardLibrary ${BoardLibrary} PARENT_SCOPE)