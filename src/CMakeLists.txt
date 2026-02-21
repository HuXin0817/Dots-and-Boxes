add_subdirectory(model)
add_subdirectory(board)
add_subdirectory(robot)
add_subdirectory(frontend)

set(ExternLibrary "src${BoardSize}x${BoardSize}")
set(ExternSources extern.cpp)
add_library(${ExternLibrary} STATIC ${ExternSources})
target_compile_definitions(${ExternLibrary} PRIVATE QT_NO_KEYWORDS
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${ExternLibrary} PRIVATE TBB::tbb Qt6::Core Qt6::Gui Qt6::Widgets ${FrontendLibrary})
set(ExternLibrary ${ExternLibrary} PARENT_SCOPE)