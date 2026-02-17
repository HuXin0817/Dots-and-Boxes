set(ModelLibrary "model${BoardSize}x${BoardSize}")
set(ModelSources
  Models.cpp
  ScoreMap.cpp
)
add_library(${ModelLibrary} STATIC ${ModelSources})
target_compile_definitions(${ModelLibrary} PRIVATE
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${ModelLibrary} PRIVATE ${CommonLibrary})
set(ModelLibrary ${ModelLibrary} PARENT_SCOPE)