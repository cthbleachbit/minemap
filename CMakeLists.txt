cmake_minimum_required(VERSION 3.10)
project(Minemap)
include(GNUInstallDirs)

set(Minemap_VERSION_MAJOR 0)
set(Minemap_VERSION_MINOR 5)
set(Minemap_VERSION_PATCH 6)

if (${Minemap_VERSION_PATCH} EQUAL 0)
    set(CMAKE_PROJECT_VERSION ${Minemap_VERSION_MAJOR}.${Minemap_VERSION_MINOR})
else ()
    set(CMAKE_PROJECT_VERSION ${Minemap_VERSION_MAJOR}.${Minemap_VERSION_MINOR}.${Minemap_VERSION_PATCH})
endif ()

add_compile_definitions(MINEMAP_APP_VER=\"${CMAKE_PROJECT_VERSION}\")
option(USE_GETTEXT "Use GNU GetText for localization (Linux / Mac only)" OFF)
option(GLIBCXX_ASSERTIONS "Enable assertions in glibc++ standard library (For debugging use)" OFF)
option(BUILD_TESTS "Build library test harness" OFF)
option(BUILD_PAMENIM "Build Pamenim" ON)
option(BUILD_MINEMARKER "Build Minemarker (experimental)" OFF)
option(BUILD_GIMP_GPL "Generate and install palette data for GIMP" ON)
option(MINEMAP_ENABLE_LTO "Enable link-time optimization" OFF)

find_package(ZLIB REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(fmt CONFIG REQUIRED)
if (USE_GETTEXT)
    if (WIN32)
        message(WARNING "GNU GetText is not supported on Windows")
        set(USE_GETTEXT OFF)
    else()
        find_package(Intl REQUIRED)
        find_package(Gettext REQUIRED)
    endif()
endif()

if (VCPKG_TARGET_TRIPLET)
    find_package(unofficial-graphicsmagick CONFIG REQUIRED)
    set(ImageMagick_LIBRARIES unofficial::graphicsmagick::graphicsmagick)
else ()
    pkg_check_modules(ImageMagick REQUIRED GraphicsMagick++)
endif ()

if (WIN32)
    set(PORTABLE_INSTALL_DIR "minemap-${CMAKE_PROJECT_VERSION}")
    add_compile_definitions(UNICODE)
    add_compile_definitions(_UNICODE)
endif ()

# Compilation options
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (MSVC)
    # /Zi - Output debugging information
    add_compile_options(/W3 /WX /Zi)
else()
    add_compile_options(-g -Wall)
endif()
if (GLIBCXX_ASSERTIONS)
    add_compile_definitions(_GLIBCXX_ASSERTIONS)
endif()

if (MINEMAP_ENABLE_LTO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if (USE_GETTEXT)
    add_compile_definitions(_USE_GETTEXT)
    add_compile_definitions(_TEXT_DOMAIN="${PROJECT_NAME}")
    add_compile_definitions(_LOCALE_DIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")
endif()

if (BUILD_GIMP_GPL)
    set(GIMP_PALETTE_DIR ${CMAKE_INSTALL_DATADIR}/gimp/2.0/palettes CACHE STRING "Directory to place GIMP palettes")
endif()

if (BUILD_TESTS)
    enable_testing()
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

# Test time dependencies
if (BUILD_TESTS)
    find_package(Python REQUIRED COMPONENTS Interpreter)
endif ()

# Resources
add_subdirectory(misc)

# Source files
add_subdirectory(src)

# Translations
if (USE_GETTEXT)
    add_subdirectory(po)
endif()
