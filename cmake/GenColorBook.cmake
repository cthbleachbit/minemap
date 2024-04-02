cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

include(ParseColorMaps)

# main entry point
if (NOT COLOR_MAP_INPUT)
    message(FATAL_ERROR "Please set the COLOR_MAP_INPUT variable to an input!")
endif ()
if (NOT COLOR_BOOK_PREFIX)
    message(FATAL_ERROR "Please set the COLOR_BOOK_PREFIX variable to a valid output prefix!")
endif ()

message(STATUS "Generating GIMP color book for ${COLOR_MAP_INPUT} ...")
parse_color_map("${COLOR_MAP_INPUT}" output)
set(output_file "${COLOR_BOOK_PREFIX}.gpl")
set(file_content "")
generate_gimp_color_book("${output}" "${COLOR_BOOK_PREFIX}" file_content)
file(WRITE "${output_file}" "${file_content}")
