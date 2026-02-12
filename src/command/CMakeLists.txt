set(CommandLibraryName "command")
set(CommandSources 
    Command.cpp
    CreateMainWindow.cpp
)
qt_add_library(${CommandLibraryName} STATIC ${CommandSources})
target_compile_definitions(${CommandLibraryName} PRIVATE __MaxBoardSize__=${MaxBoardSize})
target_link_libraries(${CommandLibraryName} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets ${ExternLibraries})
set(CommandLibraryName ${CommandLibraryName} PARENT_SCOPE)