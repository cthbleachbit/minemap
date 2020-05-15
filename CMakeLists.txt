cmake_minimum_required (VERSION 3.0)
project (Minemap)
set (Minemap_VERSION_MAJOR 0)
set (Minemap_VERSION_MINOR 2)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NBTP REQUIRED nbtp)
find_package(Boost REQUIRED COMPONENTS iostreams)
find_package(ImageMagick COMPONENTS Magick++)

# Compilation options
set (CMAKE_CXX_STANDARD 14)
set(warnings "-g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

option(BUILD_PAMENIM "Build Pamenim" OFF)

set(MINEMAP_PALETTE_DIR ${CMAKE_INSTALL_PREFIX}/share/minemap/palettes CACHE STRING "Directory to minemap palette GIFs")
add_compile_definitions(MINEMAP_PALETTE_DIR=\"${MINEMAP_PALETTE_DIR}\")

# Source files
add_subdirectory(src)

# Resources
add_subdirectory(misc)
