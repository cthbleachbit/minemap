add_executable(minemap
        minemap.cpp
        Map.cpp
        VersionSpec.cpp
        ColorMap.cpp
        constants.h)

if(WIN32)
    target_link_libraries(minemap PRIVATE nbtp-static)
else()
    target_link_libraries(minemap PUBLIC nbtp)
endif()
target_link_libraries(minemap PUBLIC ${ImageMagick_LIBRARIES})
target_link_libraries(minemap PUBLIC ${zlib_LIBRARIES})
if (UNIX)
    target_link_libraries(minemap PUBLIC dl)
endif()

target_include_directories(minemap PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_include_directories(minemap PUBLIC ${Boost_INCLUDE_DIRS})

target_compile_options(minemap PUBLIC ${ImageMagick_CFLAGS})

install(TARGETS minemap DESTINATION bin)

if (BUILD_PAMENIM)
    add_executable(pamenim
            pamenim.cpp
            VersionSpec.cpp
            ColorMap.cpp
            constants.h)
    if(WIN32)
        target_link_libraries(pamenim PRIVATE nbtp-static)
    else()
        target_link_libraries(pamenim PUBLIC nbtp)
    endif()

    target_link_libraries(pamenim PUBLIC ${ImageMagick_LIBRARIES})
    target_link_libraries(pamenim PUBLIC ${zlib_LIBRARIES})
    if (UNIX)
        target_link_libraries(pamenim PUBLIC dl)
    endif()
    target_include_directories(pamenim PUBLIC ${ImageMagick_INCLUDE_DIRS})
    target_include_directories(pamenim PUBLIC ${Boost_INCLUDE_DIRS})

    target_compile_options(pamenim PUBLIC ${ImageMagick_CFLAGS})

    install(TARGETS pamenim DESTINATION bin)
endif ()