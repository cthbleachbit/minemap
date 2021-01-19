cmake_minimum_required (VERSION 3.0)
project (Minemap)
set (Minemap_VERSION_MAJOR 0)
set (Minemap_VERSION_MINOR 4)
set (Minemap_VERSION_PATCH 0)

if (${Minemap_VERSION_PATCH} EQUAL 0)
    set(CMAKE_PROJECT_VERSION ${Minemap_VERSION_MAJOR}.${Minemap_VERSION_MINOR})
else()
    set(CMAKE_PROJECT_VERSION ${Minemap_VERSION_MAJOR}.${Minemap_VERSION_MINOR}.${Minemap_VERSION_PATCH})
endif()

add_compile_definitions(MINEMAP_APP_VER=\"${CMAKE_PROJECT_VERSION}\")

find_package(PkgConfig REQUIRED)
pkg_check_modules(ImageMagick REQUIRED Magick++-7.Q16HDRI)
if (WIN32)
    set(ENV{MAGICK_CODER_MODULE_PATH} /mingw64/lib/ImageMagick-${ImageMagick_VERSION}/modules-Q16HDRI/coders)
    add_compile_definitions(WIN_MAGICK_CODER_MODULE_PATH=L\"$ENV{MAGICK_CODER_MODULE_PATH}\")
    add_compile_definitions(UNICODE)
    add_compile_definitions(_UNICODE)
endif()
pkg_check_modules(zlib REQUIRED zlib)

# Compilation options
set(CMAKE_CXX_STANDARD 17)
set(warnings "-g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

option(BUILD_PAMENIM "Build Pamenim" ON)

set(MINEMAP_PALETTE_DIR ${CMAKE_INSTALL_PREFIX}/share/minemap/palettes CACHE STRING "Directory to minemap palette GIFs")
add_compile_definitions(MINEMAP_PALETTE_DIR=\"${MINEMAP_PALETTE_DIR}\")

# External
option(SYSTEM_NBTP "Use system nbtp" ON)
if (${SYSTEM_NBTP})
    pkg_check_modules(nbtp REQUIRED nbtp)
    include_directories(INTERFACE "${nbtp_INCLUDE_DIRS}")
else ()
    option(NBTP_BUILD_PYTHON_MODULE OFF)
    add_subdirectory(nbtp EXCLUDE_FROM_ALL)
    include_directories(INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/nbtp/include)
endif ()

# Source files
add_subdirectory(src)

# Resources
add_subdirectory(misc)
