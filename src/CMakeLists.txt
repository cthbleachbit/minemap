# Common functionality
add_library(libminemap SHARED
        ColorMap.cpp
        Map.cpp
        VersionSpec.cpp
        libminemap.cpp
        common.h
        constants.h
        GZStream.h
        VersionSpec.h
        libminemap.h)
set_target_properties(libminemap PROPERTIES OUTPUT_NAME minemap)
set_target_properties(libminemap PROPERTIES LIBRARY_SOVERSION 0)
if (WIN32)
    install(TARGETS libminemap DESTINATION ${PORTABLE_INSTALL_DIR})
else()
    install(TARGETS libminemap DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if (WIN32 OR (NOT SYSTEM_NBTP))
    target_link_libraries(libminemap PRIVATE nbtp-static)
    target_link_libraries(libminemap PRIVATE fmt::fmt)
else ()
    target_link_libraries(libminemap PUBLIC nbtp)
    target_link_libraries(libminemap PUBLIC fmt::fmt)
endif ()
if (WIN32)
    target_link_libraries(libminemap PUBLIC ws2_32)
endif()
if (WIN32 AND VCPKG_TARGET_TRIPLET)
    target_compile_definitions(libminemap PUBLIC STATIC_MAGICK)
endif ()
target_link_libraries(libminemap PUBLIC ${ImageMagick_LIBRARIES})
target_compile_options(libminemap PUBLIC ${ImageMagick_CFLAGS})
target_include_directories(libminemap PUBLIC ${ImageMagick_INCLUDE_DIRS})

###### Minemap main executable
add_executable(minemap minemap.cpp)
target_link_libraries(minemap PRIVATE libminemap)
target_link_libraries(minemap PUBLIC ZLIB::ZLIB)
target_include_directories(minemap PUBLIC ${ImageMagick_INCLUDE_DIRS})
if(WIN32)
    install(TARGETS minemap DESTINATION ${PORTABLE_INSTALL_DIR})
else()
    install(TARGETS minemap DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

###### Pamenim main executable
if (BUILD_PAMENIM)
    add_executable(pamenim pamenim.cpp)
    target_link_libraries(pamenim PRIVATE libminemap)
    target_link_libraries(pamenim PUBLIC ZLIB::ZLIB)
    target_include_directories(pamenim PUBLIC ${ImageMagick_INCLUDE_DIRS})
    if(WIN32)
        install(TARGETS pamenim DESTINATION ${PORTABLE_INSTALL_DIR})
    else()
        install(TARGETS pamenim DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif ()

add_subdirectory(tests)
