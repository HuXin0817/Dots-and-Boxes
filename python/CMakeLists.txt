pybind11_add_module(PyDab binding.cpp)
target_link_libraries(PyDab PUBLIC
  ${ExternLibraries}
  ${CommandLibraryName}
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)