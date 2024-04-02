cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

include(ParseColorMaps)

# main entry point
if (NOT COLOR_MAP_INPUTS)
    message(FATAL_ERROR "Please set the COLOR_MAP_INPUTS variable to a list of inputs!")
endif ()
list(LENGTH COLOR_MAP_INPUTS COLOR_MAP_INPUTS_LEN)
if (COLOR_MAP_INPUTS_LEN LESS 1)
    message(FATAL_ERROR "COLOR_MAP_INPUTS should contain at least one input!")
endif ()
if (NOT COLOR_MAP_OUTPUT)
    message(FATAL_ERROR "Please set the COLOR_MAP_OUTPUT variable to a valid output!")
endif ()

set(file_content [=[
// Automatically generated. DO NOT EDIT.
// To create maps for new versions, create a new table in /misc directory.
#pragma once
#include <cstddef>
#include <cstdint>
]=])
foreach(INPUT IN LISTS COLOR_MAP_INPUTS)
    if ("${INPUT}" MATCHES [[rgb-base-([0-9].[0-9]+)\.txt]])
        string(MAKE_C_IDENTIFIER "MC_${CMAKE_MATCH_1}" c_ident)
    else ()
        message(FATAL_ERROR "Unknown palette filename: ${INPUT}")
    endif ()
    set(output "")
    message(STATUS "Generating color values for ${INPUT} ...")
    parse_color_map("${INPUT}" output)
    generate_c_header("${INPUT}" "${c_ident}" "${output}" output)
    set(file_content "${file_content}${output}")
endforeach()
file(WRITE "${COLOR_MAP_OUTPUT}" "${file_content}")
