set(FrontendLibrary "frontend${BoardSize}x${BoardSize}")
set(FrontendSources
  BaseCanvas.cpp
  BoxCanvas.cpp
  DotCanvas.cpp
  EdgeCanvas.cpp
  MainWindow.cpp
)
add_library(${FrontendLibrary} STATIC ${FrontendSources})
target_compile_definitions(${FrontendLibrary} PRIVATE QT_NO_KEYWORDS 
  __BoardSize__=${BoardSize} 
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${FrontendLibrary} PRIVATE 
  Qt6::Core Qt6::Gui Qt6::Widgets 
  ${CommonLibrary} 
  ${ModelLibrary} 
  ${BoardLibrary} 
  ${RobotLibrary}
)
set(FrontendLibrary ${FrontendLibrary} PARENT_SCOPE)