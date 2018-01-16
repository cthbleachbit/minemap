set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")
add_executable(gen_color_map gen_color_map.c)

target_link_libraries(gen_color_map ${IMAGEMAGICK_LIBRARIES})
target_include_directories(gen_color_map PUBLIC ${IMAGEMAGICK_INCLUDE_DIRS})
target_compile_options(gen_color_map PUBLIC ${IMAGEMAGICK_CFLAGS_OTHER})
