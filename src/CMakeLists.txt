set(BASE_FILES
    mc_nbt.c
    mc_map.c
    utils.c
    mc_endian.c)

add_library(mcmap ${BASE_FILES})

add_executable(minemap
               minemap.c)
target_link_libraries(minemap mcmap)
target_link_libraries(minemap ${IMAGEMAGICK_LIBRARIES})
target_include_directories(minemap PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
target_compile_options(minemap PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})

pkg_check_modules(ZLIB REQUIRED zlib)
target_link_libraries(minemap ${ZLIB_LIBRARIES})
target_include_directories(minemap PUBLIC ${ZLIB_INCLUDE_DIRS})
target_compile_options(minemap PUBLIC ${ZLIB_CFLAGS_OTHER})

install (TARGETS minemap DESTINATION bin)

add_executable(pamenim
               pamenim.c)
target_link_libraries(pamenim mcmap)
target_link_libraries(pamenim ${IMAGEMAGICK_LIBRARIES})
target_include_directories(pamenim PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
target_compile_options(pamenim PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})

pkg_check_modules(ZLIB REQUIRED zlib)
target_link_libraries(pamenim ${ZLIB_LIBRARIES})
target_include_directories(pamenim PUBLIC ${ZLIB_INCLUDE_DIRS})
target_compile_options(pamenim PUBLIC ${ZLIB_CFLAGS_OTHER})

install (TARGETS pamenim DESTINATION bin)
