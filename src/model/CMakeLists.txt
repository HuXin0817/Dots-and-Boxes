set(ModelLibraryName "model${BoardSize}x${BoardSize}")
set(ModelSources
  Models.cpp
  SearchScoreMap.cpp
)
add_library(${ModelLibraryName} SHARED ${ModelSources})
set_target_properties(${ModelLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/model")
target_include_directories(${ModelLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${ModelLibraryName} PRIVATE __BoardSize__=${BoardSize})

set(ModelLibraryName ${ModelLibraryName} PARENT_SCOPE)