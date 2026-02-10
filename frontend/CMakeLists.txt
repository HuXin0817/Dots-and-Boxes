set(FrontendLibraryName "frontend_${BoardSize}x${BoardSize}")
set(FrontendSources
  BaseCanvas.cpp
  BoxCanvas.cpp
  DotCanvas.cpp
  EdgeCanvas.cpp
  MainWindow.cpp
  Models.cpp
)
qt_add_library(${FrontendLibraryName} SHARED ${FrontendSources})
set_target_properties(${FrontendLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/frontend")
target_include_directories(${FrontendLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${FrontendLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
target_link_libraries(${FrontendLibraryName} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${ModelLibraryName} ${BoardLibraryName} ${RobotLibraryName})

set(FrontendLibraryName ${FrontendLibraryName} PARENT_SCOPE)