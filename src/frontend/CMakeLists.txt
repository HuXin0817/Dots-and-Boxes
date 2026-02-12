set(FrontendLibraryName "frontend${BoardSize}x${BoardSize}")
set(FrontendSources
  BaseCanvas.cpp
  BoxCanvas.cpp
  DotCanvas.cpp
  EdgeCanvas.cpp
  MainWindow.cpp
  Models.cpp
)
qt_add_library(${FrontendLibraryName} STATIC ${FrontendSources})
target_compile_definitions(${FrontendLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize} __detail__=__detail${BoardSize}x${BoardSize}__)
target_link_libraries(${FrontendLibraryName} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${CommonLibraryName} ${BoardLibraryName} ${RobotLibraryName})

set(FrontendLibraryName ${FrontendLibraryName} PARENT_SCOPE)