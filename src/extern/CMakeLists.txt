set(ExternLibraryName "extern${BoardSize}x${BoardSize}")
set(ExternSources 
  CreateMainWindow.cpp
  MockRunningGame.cpp
)
qt_add_library(${ExternLibraryName} STATIC ${ExternSources})
target_compile_definitions(${ExternLibraryName} PRIVATE QT_NO_KEYWORDS 
  __BoardSize__=${BoardSize} 
  __detail__=detail${BoardSize}x${BoardSize}
)
target_link_libraries(${ExternLibraryName} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${FrontendLibraryName})
set(ExternLibraryName ${ExternLibraryName} PARENT_SCOPE)