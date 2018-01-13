cmake_minimum_required (VERSION 2.8)
project (Minemap)
set (Minemap_VERSION_MAJOR 0)
set (Minemap_VERSION_MINOR 0)

set(basefiles src/mc_nbt.c src/mc_map.c src/utils.c src/mc_endian.c)
add_executable(minemap src/minemap.c ${basefiles})

find_package(PkgConfig REQUIRED)
pkg_check_modules(IMAGEMAGICK REQUIRED ImageMagick-7.Q16HDRI)
target_link_libraries(minemap ${IMAGEMAGICK_LIBRARIES})
target_include_directories(minemap PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
target_compile_options(minemap PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})

set(warnings "-g -Wall -Wextra")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

install (TARGETS minemap DESTINATION bin)

# Testing
enable_testing()
add_test(minemap_test minemap)
add_subdirectory(tests)
