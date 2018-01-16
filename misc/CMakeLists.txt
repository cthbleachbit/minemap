add_executable(gen_color_map gen_color_map.c)
target_link_libraries(gen_color_map ${IMAGEMAGICK_LIBRARIES})
target_include_directories(gen_color_map PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
target_compile_options(gen_color_map PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})

add_custom_target(gen_palettes ALL
    DEPENDS gen_color_map
    COMMAND make -f Makefile.palette all
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RESOURCE_FILES
    rgba-1.12.gif
    rgba-1.8.gif)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILES} DESTINATION share/minemap/palettes)
