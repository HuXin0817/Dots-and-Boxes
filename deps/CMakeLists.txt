set(TBB_TEST OFF CACHE BOOL "Disable TBB tests" FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL "Disable TBB examples" FORCE)

add_subdirectory(tbb)
add_subdirectory(json)
add_subdirectory(lru)