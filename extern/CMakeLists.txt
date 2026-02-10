set(ExternLibraryName "extern${BoardSize}x${BoardSize}")
set(ExternSources extern.cpp)
qt_add_library(${ExternLibraryName} STATIC ${ExternSources})
set_target_properties(${ExternLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/extern")
target_include_directories(${ExternLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${ExternLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize} dab=dab${BoardSize}x${BoardSize})
target_link_libraries(${ExternLibraryName} PRIVATE 
  Qt6::Core Qt6::Gui Qt6::Widgets TBB::tbb
  ${ModelLibraryName}
  ${BoardLibraryName}
  ${RobotLibraryName}
  ${FrontendLibraryName}
)

set(ExternLibraryName ${ExternLibraryName} PARENT_SCOPE)