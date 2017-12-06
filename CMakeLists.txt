cmake_minimum_required (VERSION 2.8)
project (Minemap)
set (Minemap_VERSION_MAJOR 0)
set (Minemap_VERSION_MINOR 0)

set(basefiles src/mc_nbt.c src/mc_map.c src/utils.c src/mc_endian.c)
set(warnings "-Wall -Wextra")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

add_executable(minemap src/minemap.c ${basefiles})
install (TARGETS minemap DESTINATION bin)

# Testing
enable_testing()
add_test(minemap_test minemap)
add_subdirectory(tests)
