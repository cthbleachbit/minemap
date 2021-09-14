# Common functionality
add_library(libminemap SHARED
        ColorMap.cpp
        Map.cpp
        VersionSpec.cpp
        common.h
        constants.h
        GZStream.h
        VersionSpec.h)
set_target_properties(libminemap PROPERTIES LIBRARY_OUTPUT_NAME minemap)
set_target_properties(libminemap PROPERTIES LIBRARY_SOVERSION 0)
install(TARGETS libminemap DESTINATION lib)

if(WIN32)
    target_link_libraries(libminemap PRIVATE nbtp-static)
else()
    target_link_libraries(libminemap PUBLIC nbtp)
endif()
target_link_libraries(libminemap PUBLIC ${ImageMagick_LIBRARIES})
target_compile_options(libminemap PUBLIC ${ImageMagick_CFLAGS})
target_include_directories(libminemap PUBLIC ${ImageMagick_INCLUDE_DIRS})

add_executable(minemap minemap.cpp)
target_link_libraries(minemap PRIVATE libminemap)
target_link_libraries(minemap PUBLIC ${zlib_LIBRARIES})
target_include_directories(minemap PUBLIC ${ImageMagick_INCLUDE_DIRS})
install(TARGETS minemap DESTINATION bin)

if (BUILD_PAMENIM)
    add_executable(pamenim pamenim.cpp)
    target_link_libraries(pamenim PRIVATE libminemap)
    target_link_libraries(pamenim PUBLIC ${zlib_LIBRARIES})
    target_include_directories(pamenim PUBLIC ${ImageMagick_INCLUDE_DIRS})
    install(TARGETS pamenim DESTINATION bin)
endif ()