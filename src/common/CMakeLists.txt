set(CommonLibrary "common${BoardSize}x${BoardSize}")
set(CommonSources Random.cpp)
add_library(${CommonLibrary} STATIC ${CommonSources})
target_compile_definitions(${CommonLibrary} PRIVATE 
  __BoardSize__=${BoardSize} 
  __detail__=detail${BoardSize}x${BoardSize}
)
set(CommonLibrary ${CommonLibrary} PARENT_SCOPE)