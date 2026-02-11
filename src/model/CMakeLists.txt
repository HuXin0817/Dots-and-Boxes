set(ModelLibraryName "model${BoardSize}x${BoardSize}")
set(ModelSources
  Models.cpp
  SearchScoreMap.cpp
)
add_library(${ModelLibraryName} STATIC ${ModelSources})
target_compile_definitions(${ModelLibraryName} PRIVATE __BoardSize__=${BoardSize} dab=dab${BoardSize}x${BoardSize})

set(ModelLibraryName ${ModelLibraryName} PARENT_SCOPE)