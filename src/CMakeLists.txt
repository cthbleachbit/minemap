add_executable(minemap
        minemap.cpp
        Map.cpp
        utils.cpp)

target_link_libraries(minemap PUBLIC ${NBTP_LIBRARIES})
target_link_libraries(minemap PUBLIC ${ImageMagick_LIBRARIES})
target_include_directories(minemap PUBLIC ${NBTP_INCLUDE_DIRS})
target_include_directories(minemap PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_compile_options(minemap PUBLIC ${ImageMagick_CFLAGS_OTHER})
target_link_libraries(minemap PUBLIC ${Boost_LIBRARIES})
target_include_directories(minemap PUBLIC ${Boost_INCLUDE_DIRS})

install(TARGETS minemap DESTINATION bin)

if (BUILD_PAMENIM)
    add_executable(pamenim
            pamenim.c)
    target_link_libraries(pamenim ${IMAGEMAGICK_LIBRARIES})
    target_include_directories(pamenim PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
    target_compile_options(pamenim PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})

    pkg_check_modules(ZLIB REQUIRED zlib)
    target_link_libraries(pamenim ${ZLIB_LIBRARIES})
    target_include_directories(pamenim PUBLIC ${ZLIB_INCLUDE_DIRS})
    target_compile_options(pamenim PUBLIC ${ZLIB_CFLAGS_OTHER})

    install(TARGETS pamenim DESTINATION bin)
endif ()