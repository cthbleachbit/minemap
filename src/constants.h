//
// Created by cth451 on 2020/05/15.
//

#ifndef MINEMAP_CONSTANTS_H
#define MINEMAP_CONSTANTS_H

#ifdef _USE_GETTEXT

#include <libintl.h>
#include <fmt/format.h>
#define MINEMAP_TEXTDOMAIN "minemap"
#ifndef _LOCALE_DIR
#define _LOCALE_DIR "/usr/local/share/locale/"
#endif
#define _(x) gettext (x)

template<typename ... T>
inline auto localizedFormat(std::string_view fmt, T &&... args) {
	return fmt::vformat(fmt, fmt::make_format_args(std::forward<T>(args)...));
}

#else
#define _(x) x
#define localizedFormat fmt::format
#endif

inline void initializeLocale() {
#ifdef _USE_GETTEXT
	setlocale(LC_ALL, "");
	bindtextdomain(MINEMAP_TEXTDOMAIN, _LOCALE_DIR);
	textdomain(MINEMAP_TEXTDOMAIN);
#else
	return;
#endif
}

#define INVALID_GAME_VER _("Invalid Minecraft Version Specifier")

#define MISSING_GAME_VER _("No game version or invalid version specified")
#define MISSING_IN_FILE  _("No input file specified")
#define MISSING_OUT_FILE _("No output file specified")
#define MISSING_MARKER _("Missing marker type / operation")

#define INVALID_ARGUMENT _("Invalid argument passed: {}")
#define INVALID_POSITION _("Invalid position string")
#define INVALID_BANNER _("Invalid banner specifier")
#define POSITION_OUT_OF_RANGE _("Coordinates out of range")

#define MAP_NOT_COMPOUND _("The map data tag is not a compound but a {}. Is this a map nbt file?")
#define COLORS_NOT_BYTES _("Color data is not a byte array. Is this a map nbt file?")
#define BANNERS_MISSING _("The map does not supports banners - update to a newer Minecraft version and try again")
#define BANNERS_MALFORMED _("Banners in this map is malformed")
#define POSITION_MALFORMED _("Position coordinates in this marker is malformed")
#define COLOR_MISMATCH _("BUG: No color match for pixel at ({}, {}). Please submit a bug report.")
#define COLOR_PARTIALLY_TRANSPARENT _("Warning: {} partially transparent pixels are rendered as fully transparent!")
#define COLOR_OUT_OF_RANGE _("Color code {} at offset {} is out of range!")

#ifdef USE_GM
#define MINEMAP_MAGICK_IMPLEMENTATION "GraphicsMagick"
#else
#define MINEMAP_MAGICK_IMPLEMENTATION "ImageMagick"
#endif
#define VERSION_MESSAGE "Application version " MINEMAP_APP_VER " with " MINEMAP_MAGICK_IMPLEMENTATION

#endif //CONSTANTS_H
