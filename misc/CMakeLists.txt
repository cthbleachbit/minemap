add_executable(gen_color_map gen_color_map.cpp)
if (WIN32 AND VCPKG_TARGET_TRIPLET)
    target_compile_definitions(gen_color_map PUBLIC STATIC_MAGICK)
endif ()
target_link_libraries(gen_color_map PUBLIC ${ImageMagick_LIBRARIES})
target_include_directories(gen_color_map PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_compile_options(gen_color_map PUBLIC ${ImageMagick_CFLAGS_OTHER})

if (BUILD_GIMP_GPL)
    add_executable(gen_gimp_palette gen_gimp_palette.cpp)
endif ()

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
            DEPENDS gen_color_map
            COMMAND $<TARGET_FILE:gen_color_map> "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}" "${GIF_FILE_OUTPUT}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )
    if(WIN32)
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIF_FILE_OUTPUT}" DESTINATION ${PORTABLE_INSTALL_DIR})
    else()
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIF_FILE_OUTPUT}" DESTINATION ${MINEMAP_PALETTE_DIR})
    endif()
endforeach ()

if (BUILD_GIMP_GPL)
    foreach (MC_VERSION IN LISTS MC_VERSIONS)
        set(PALETTE_FILE_NAME "rgba-${MC_VERSION}.txt")
        set(GIMP_FILE_PREFIX "Minecraft${MC_VERSION}")
        add_custom_target("gen_gimp_palettes_${MC_VERSION}" ALL
                DEPENDS gen_gimp_palette
                COMMAND $<TARGET_FILE:gen_gimp_palette> "${CMAKE_CURRENT_SOURCE_DIR}/${PALETTE_FILE_NAME}" "${GIMP_FILE_PREFIX}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                )
        if(WIN32)
            install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIF_FILE_OUTPUT}" DESTINATION ${PORTABLE_INSTALL_DIR}/GIMP)
        else()
            install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIMP_FILE_PREFIX}.gpl" DESTINATION ${GIMP_PALETTE_DIR})
        endif()
    endforeach ()
endif ()
