##### libminemap test harness
if (BUILD_TESTS)
    add_executable(harness harness.cpp)
    target_link_libraries(harness PRIVATE libminemap)
    target_include_directories(harness PUBLIC ${ImageMagick_INCLUDE_DIRS})
    target_include_directories(harness PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
endif ()