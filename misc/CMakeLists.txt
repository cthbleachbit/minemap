find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(MC_VERSIONS
        1.8
        1.12
        1.16
        1.17
        )
set(COLOR_MAPS "")

foreach (MC_VERSION IN LISTS MC_VERSIONS)
    set(PALETTE_FILE_NAME "rgb-base-${MC_VERSION}.txt")
    list(APPEND COLOR_MAPS "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}")
endforeach ()
add_custom_target("gen_h_palettes" ALL
        BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/color_maps.gen.h"
        DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/gen_color_map.py"
        ${COLOR_MAPS}
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/gen_color_map.py" "${COLOR_MAPS}" "color_maps.gen.h"
        COMMENT "Generating color maps headers ..."
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
set(MINEMAP_COLORMAP_INCLUDES "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

if (BUILD_GIMP_GPL)
    foreach (MC_VERSION IN LISTS MC_VERSIONS)
        set(PALETTE_FILE_NAME "rgb-base-${MC_VERSION}.txt")
        set(GIMP_FILE_PREFIX "Minecraft${MC_VERSION}")
        add_custom_target("gen_gimp_palettes_${MC_VERSION}" ALL
                BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/gen_gimp_palette.py" "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}"
                COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/gen_gimp_palette.py" "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}" "${GIMP_FILE_PREFIX}"
                COMMENT "Generating GIMP palettes for ${MC_VERSION} ..."
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                )
        if (WIN32)
            install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl" DESTINATION ${PORTABLE_INSTALL_DIR}/GIMP)
        else ()
            install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl" DESTINATION ${GIMP_PALETTE_DIR})
        endif ()
    endforeach ()
endif ()
