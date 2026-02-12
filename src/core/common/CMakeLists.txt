set(CommonLibraryName "common${BoardSize}x${BoardSize}")
set(ModelSources
  Models.cpp
  ScoreMap.cpp
)
add_library(${CommonLibraryName} STATIC ${ModelSources})
target_compile_definitions(${CommonLibraryName} PRIVATE __BoardSize__=${BoardSize} __detail__=__detail${BoardSize}x${BoardSize}__)

set(CommonLibraryName ${CommonLibraryName} PARENT_SCOPE)