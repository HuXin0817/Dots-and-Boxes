set(FrontendLibrary "frontend${BoardSize}x${BoardSize}")
set(FrontendSources
  BaseCanvas.cpp
  BoxCanvas.cpp
  DotCanvas.cpp
  EdgeCanvas.cpp
  Frontend.cpp
  MainWindow.cpp
)
add_library(${FrontendLibrary} STATIC ${FrontendSources})
target_compile_definitions(${FrontendLibrary} PRIVATE QT_NO_KEYWORDS
  __BoardSize__=${BoardSize}
  __detail__=detail${BoardSize}x${BoardSize}
)
target_include_directories(${FrontendLibrary} PRIVATE
  ${CMAKE_SOURCE_DIR}/deps/lru
)
target_link_libraries(${FrontendLibrary} PRIVATE
  nlohmann_json::nlohmann_json
  TBB::tbb
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)
set(FrontendLibrary ${FrontendLibrary} PARENT_SCOPE)