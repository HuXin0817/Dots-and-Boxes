set(FrontendLibrary "frontend${BoardSize}x${BoardSize}")
set(FrontendSources
  BoxCanvas.cpp
  DotCanvas.cpp
  EdgeCanvas.cpp
  Frontend.cpp
  GlobalEnv.cpp
  MainWindow.cpp
)
add_library(${FrontendLibrary} STATIC ${FrontendSources})
target_compile_definitions(${FrontendLibrary} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
target_link_libraries(${FrontendLibrary} PRIVATE
  TBB::tbb
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)
set(FrontendLibrary ${FrontendLibrary} PARENT_SCOPE)