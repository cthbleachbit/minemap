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
add_custom_command(OUTPUT "color_maps.gen.h"
        DEPENDS ${COLOR_MAPS}
        COMMAND "${CMAKE_COMMAND}" "-DCOLOR_MAP_INPUTS=${COLOR_MAPS}" "-DCOLOR_MAP_OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/color_maps.gen.h" -P "${CMAKE_SOURCE_DIR}/cmake/GenColorMaps.cmake"
        COMMENT "Generating color maps headers ..."
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
)
add_custom_target(color_maps DEPENDS "color_maps.gen.h")
set(MINEMAP_COLORMAP_INCLUDES "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

if (BUILD_GIMP_GPL)
    foreach (MC_VERSION IN LISTS MC_VERSIONS)
        set(PALETTE_FILE_NAME "rgb-base-${MC_VERSION}.txt")
        set(GIMP_FILE_PREFIX "Minecraft${MC_VERSION}")
        add_custom_command(OUTPUT "${GIMP_FILE_PREFIX}.gpl"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}"
            COMMAND "${CMAKE_COMMAND}" "-DCOLOR_MAP_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}" "-DCOLOR_BOOK_PREFIX=${GIMP_FILE_PREFIX}" -P "${CMAKE_SOURCE_DIR}/cmake/GenColorBook.cmake"
            COMMENT "Generating GIMP palettes for ${MC_VERSION} ..."
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM
        )
        add_custom_target("gen_gimp_palettes_${MC_VERSION}" ALL DEPENDS "${GIMP_FILE_PREFIX}.gpl")
        if (WIN32)
            install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl" DESTINATION ${PORTABLE_INSTALL_DIR}/GIMP)
        else ()
            install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl" DESTINATION ${GIMP_PALETTE_DIR})
        endif ()
    endforeach ()
endif ()
