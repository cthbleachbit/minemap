find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(MC_VERSIONS
        1.8
        1.12
        1.16
        1.17
        )

foreach (MC_VERSION IN LISTS MC_VERSIONS)
    set(PALETTE_FILE_NAME "rgba-${MC_VERSION}.txt")
    set(GIF_FILE_OUTPUT "rgba-${MC_VERSION}.gif")
    add_custom_target("gen_gif_palettes_${MC_VERSION}" ALL
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/gen_color_map.py" "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}" "${GIF_FILE_OUTPUT}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIF_FILE_OUTPUT}" DESTINATION ${MINEMAP_PALETTE_DIR})
endforeach ()

if (BUILD_GIMP_GPL)
    foreach (MC_VERSION IN LISTS MC_VERSIONS)
        set(PALETTE_FILE_NAME "rgba-${MC_VERSION}.txt")
        set(GIMP_FILE_PREFIX "Minecraft${MC_VERSION}")
        add_custom_target("gen_gimp_palettes_${MC_VERSION}" ALL
                COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/gen_gimp_palette.py" "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}" "${GIMP_FILE_PREFIX}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                )
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl" DESTINATION ${GIMP_PALETTE_DIR})
    endforeach ()
endif ()
