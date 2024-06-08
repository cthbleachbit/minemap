##### libminemap test harness

include_directories(${GTest_INCLUDE_DIR})

add_library(libminemap_test SHARED EXCLUDE_FROM_ALL test_helpers.cpp)
set_target_properties(libminemap_test PROPERTIES OUTPUT_NAME minemap_test)
target_link_libraries(libminemap_test INTERFACE gtest)
target_link_libraries(libminemap_test INTERFACE libminemap)
target_link_libraries(libminemap_test INTERFACE fmt)
target_compile_options(libminemap_test PUBLIC ${ImageMagick_CFLAGS})
target_include_directories(libminemap_test PUBLIC ${ImageMagick_INCLUDE_DIRS})

set(MINEMAP_UNIT_TESTS
        unit_text
)

foreach (MINEMAP_UNIT_TEST ${MINEMAP_UNIT_TESTS})
    add_executable(${MINEMAP_UNIT_TEST} ${MINEMAP_UNIT_TEST}.cpp)
    target_link_libraries(${MINEMAP_UNIT_TEST} PUBLIC libminemap)
    target_link_libraries(${MINEMAP_UNIT_TEST} PUBLIC gtest)
    target_link_libraries(${MINEMAP_UNIT_TEST} PRIVATE libminemap_test)
    gtest_discover_tests(${MINEMAP_UNIT_TEST}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../test-images
            EXTRA_ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../../test-images
    )
endforeach ()
