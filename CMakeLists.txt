cmake_minimum_required(VERSION 3.10)
project(Minemap)
include(GNUInstallDirs)

set(Minemap_VERSION_MAJOR 0)
set(Minemap_VERSION_MINOR 5)
set(Minemap_VERSION_PATCH 0)

if (${Minemap_VERSION_PATCH} EQUAL 0)
    set(CMAKE_PROJECT_VERSION ${Minemap_VERSION_MAJOR}.${Minemap_VERSION_MINOR})
else ()
    set(CMAKE_PROJECT_VERSION ${Minemap_VERSION_MAJOR}.${Minemap_VERSION_MINOR}.${Minemap_VERSION_PATCH})
endif ()

add_compile_definitions(MINEMAP_APP_VER=\"${CMAKE_PROJECT_VERSION}\")
option(USE_GRAPHICSMAGICK "Use GraphicsMagick instead of ImageMagick" ON)
option(GLIBCXX_ASSERTIONS "Enable assertions in glibc++ standard library (For debugging use)" OFF)

find_package(ZLIB REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(fmt CONFIG REQUIRED)

if (USE_GRAPHICSMAGICK)
    if (VCPKG_TARGET_TRIPLET)
        find_package(unofficial-graphicsmagick CONFIG REQUIRED)
        set(ImageMagick_LIBRARIES unofficial::graphicsmagick::graphicsmagick)
    else ()
        pkg_check_modules(ImageMagick REQUIRED GraphicsMagick++)
    endif ()
    add_compile_definitions(USE_GM)
else ()
    pkg_check_modules(ImageMagick REQUIRED Magick++-7.Q16HDRI)
endif ()

if (WIN32)
    set(PORTABLE_INSTALL_DIR "minemap-${CMAKE_PROJECT_VERSION}")
    add_compile_definitions(UNICODE)
    add_compile_definitions(_UNICODE)
endif ()

# Compilation options
set(CMAKE_CXX_STANDARD 20)
set(warnings "-g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} -fPIC")
if (GLIBCXX_ASSERTIONS)
    add_compile_definitions(_GLIBCXX_ASSERTIONS)
endif()


option(BUILD_TESTS "Build library test harness" OFF)
option(BUILD_PAMENIM "Build Pamenim" ON)
option(BUILD_MINEMARKER "Build Minemarker (experimental)" OFF)
option(BUILD_GIMP_GPL "Generate and install palette data for GIMP" ON)

if (BUILD_GIMP_GPL)
    set(GIMP_PALETTE_DIR ${CMAKE_INSTALL_DATADIR}/gimp/2.0/palettes CACHE STRING "Directory to place GIMP palettes")
endif()

# External
option(SYSTEM_NBTP "Use system nbtp" ON)
if (${SYSTEM_NBTP})
    pkg_check_modules(nbtp REQUIRED nbtp)
    include_directories(INTERFACE "${nbtp_INCLUDE_DIRS}")
else ()
    set(NBTP_BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)
    option(NBTP_BUILD_PYTHON_MODULE OFF)
    add_subdirectory(nbtp EXCLUDE_FROM_ALL)
    include_directories(INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/nbtp/include)
endif ()

# Resources
add_subdirectory(misc)

# Source files
add_subdirectory(src)
