add_executable(gen_color_map gen_color_map.c)
target_link_libraries(gen_color_map ${IMAGEMAGICK_LIBRARIES})
target_include_directories(gen_color_map PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
target_compile_options(gen_color_map PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})

file(GLOB PALETTE_FILES
    LIST_DIRECTORIES FALSE
    RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}"
    "rgba-*.txt"
)

foreach(PALETTE_FILE IN LISTS PALETTE_FILES)
    get_filename_component(PALETTE_FILE_NAME "${PALETTE_FILE}" NAME)
    string(REPLACE ".txt" ".gif" PALETTE_FILE_OUTPUT "${PALETTE_FILE_NAME}")
    add_custom_target("gen_palettes_${PALETTE_FILE_NAME}" ALL
        DEPENDS gen_color_map
        COMMAND $<TARGET_FILE:gen_color_map> "${PALETTE_FILE}" "${PALETTE_FILE_OUTPUT}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PALETTE_FILE_OUTPUT}" DESTINATION share/minemap/palettes)
endforeach()
