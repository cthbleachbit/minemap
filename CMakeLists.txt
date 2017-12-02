cmake_minimum_required (VERSION 2.6)
project (Minemap)
set (Minemap_VERSION_MAJOR 0)
set (Minemap_VERSION_MINOR 0)

add_library(mc_nbt src/mc_nbt.c)
add_library(mc_map src/mc_map.c)
add_library(utils src/utils.c)
set (EXTRA_LIBS ${EXTRA_LIBS} mc_nbt mc_map utils)

add_executable(minemap src/minemap.c)
target_link_libraries(minemap  ${EXTRA_LIBS})
