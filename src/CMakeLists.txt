# Common functionality
if (BUILD_MINEMARKER)
    set(PROGRAMS minemap pamenim minemarker)
else ()
    set(PROGRAMS minemap pamenim)
endif ()

add_library(libminemap SHARED
        ColorMap.cpp
        Map.cpp
        VersionSpec.cpp
        libminemap.cpp
        constants.h
        GZStream.h
        VersionSpec.h
        libminemap.h
        Marker.cpp
        Marker.h)
add_dependencies(libminemap color_maps)
target_include_directories(libminemap PRIVATE "${MINEMAP_COLORMAP_INCLUDES}")
set_target_properties(libminemap PROPERTIES OUTPUT_NAME minemap)
set_target_properties(libminemap PROPERTIES LIBRARY_SOVERSION 0)
if (WIN32)
    install(TARGETS libminemap DESTINATION ${PORTABLE_INSTALL_DIR})
else ()
    install(TARGETS libminemap DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()

if (WIN32 OR (NOT SYSTEM_NBTP))
    target_link_libraries(libminemap PRIVATE nbtp-static)
    target_link_libraries(libminemap PRIVATE fmt::fmt-header-only)
else ()
    target_link_libraries(libminemap PUBLIC nbtp)
    target_link_libraries(libminemap PUBLIC fmt::fmt)
endif ()
if (WIN32)
    target_link_libraries(libminemap PUBLIC ws2_32)
endif ()
if (WIN32 AND VCPKG_TARGET_TRIPLET)
    target_compile_definitions(libminemap PUBLIC STATIC_MAGICK)
endif ()
target_link_libraries(libminemap PUBLIC ${ImageMagick_LIBRARIES})
target_compile_options(libminemap PUBLIC ${ImageMagick_CFLAGS})
target_include_directories(libminemap PUBLIC ${ImageMagick_INCLUDE_DIRS})
if (USE_GETTEXT)
    target_include_directories(libminemap PUBLIC ${Intl_INCLUDE_DIRS})
    if (NOT Intl_IS_BUILT_IN)
        target_link_libraries(libminemap PUBLIC ${Intl_LIBRARY})
    endif()
endif()

foreach (PROGRAM IN LISTS PROGRAMS)
    add_executable(${PROGRAM} ${PROGRAM}.cpp)
    target_link_libraries(${PROGRAM} PRIVATE libminemap)
    if (WIN32)
        target_link_libraries(${PROGRAM} PRIVATE fmt::fmt-header-only)
    endif ()
    target_link_libraries(${PROGRAM} PUBLIC ZLIB::ZLIB)
    target_include_directories(${PROGRAM} PUBLIC ${ImageMagick_INCLUDE_DIRS})
    if (WIN32)
        install(TARGETS ${PROGRAM} DESTINATION ${PORTABLE_INSTALL_DIR})
    else ()
        install(TARGETS ${PROGRAM} DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif ()
endforeach ()

add_subdirectory(tests)

if (BUILD_TESTS)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    file(GLOB TEST_FIXURES LIST_DIRECTORIES false "${CMAKE_CURRENT_SOURCE_DIR}/../test-images/*.json")
    foreach(TEST_FIXURE IN LISTS TEST_FIXURES)
        if ("${TEST_FIXURE}" MATCHES "([^/]+)\\.json")
            add_test(NAME "${CMAKE_MATCH_1}"
                COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../test-images/verify.py" "$<TARGET_FILE:minemap>" "${TEST_FIXURE}"
            )
        endif ()
    endforeach()
endif ()
