set(ModelLibraryName "model${BoardSize}x${BoardSize}")
set(ModelSources
  Assert.cpp
  Models.cpp
  Random.cpp
  ScoreMap.cpp
)
add_library(${ModelLibraryName} STATIC ${ModelSources})
target_compile_definitions(${ModelLibraryName} PRIVATE __BoardSize__=${BoardSize} __detail__=__detail${BoardSize}x${BoardSize}__)
set(ModelLibraryName ${ModelLibraryName} PARENT_SCOPE)