if (BUILD_TESTS)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    file(GLOB TEST_FIXURES LIST_DIRECTORIES false "${CMAKE_CURRENT_SOURCE_DIR}/*.json")
    foreach (TEST_FIXURE IN LISTS TEST_FIXURES)
        if ("${TEST_FIXURE}" MATCHES "([^/]+)\\.json")
            add_test(NAME "${CMAKE_MATCH_1}"
                    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/verify.py" "$<TARGET_FILE:minemap>" "${TEST_FIXURE}"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            )
        endif ()
    endforeach ()
endif ()
