find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

Python3_add_library(PyDab MODULE binding.cpp)
target_link_libraries(PyDab PUBLIC
  ${ExternLibraries}
  ${CommandLibraryName}
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)