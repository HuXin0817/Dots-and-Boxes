set(MainLibraryName "main_${BoardSize}x${BoardSize}")
set(MainSources extern.cpp)
qt_add_library(${MainLibraryName} SHARED ${MainSources})
set_target_properties(${MainLibraryName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
target_include_directories(${MainLibraryName} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(${MainLibraryName} PRIVATE QT_NO_KEYWORDS __BoardSize__=${BoardSize})
target_link_libraries(${MainLibraryName} PRIVATE 
  Qt6::Core Qt6::Gui Qt6::Widgets TBB::tbb
  ${ModelLibraryName}
  ${BoardLibraryName}
  ${RobotLibraryName}
  ${FrontendLibraryName}
)