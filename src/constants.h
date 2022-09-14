//
// Created by cth451 on 2020/05/15.
//

#ifndef MINEMAP_CONSTANTS_H
#define MINEMAP_CONSTANTS_H

#ifdef _USE_GETTEXT

#include <libintl.h>
#include <fmt/format.h>

#ifndef _LOCALE_DIR
#define _LOCALE_DIR "/usr/local/share/locale/"
#endif
#ifndef _TEXT_DOMAIN
#define _TEXT_DOMAIN "Minemap"
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
	bindtextdomain(_TEXT_DOMAIN, _LOCALE_DIR);
	textdomain(_TEXT_DOMAIN);
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

#define MAP_NOT_COMPOUND _("The map data tag is not a compound but a {}. This file is probably not a map nbt.")
#define COLORS_NOT_BYTES _("Color data is not a byte array. This file is probably not a map nbt.")
#define BANNERS_MISSING _("The map does not supports banners - update to a newer Minecraft version and try again")
#define BANNERS_MALFORMED _("Banners in this map is malformed")
#define POSITION_MALFORMED _("Position coordinates in this marker is malformed")
#define COLOR_MISMATCH _("BUG: No color match for pixel at ({}, {}). Please submit a bug report.")
#define COLOR_PARTIALLY_TRANSPARENT _("Warning: Had to render {} partially transparent pixels into fully transparent ones. Your output might look incorrect.")
#define COLOR_OUT_OF_RANGE _("Color code {} at offset {} is out of range!")

// Usage Text
#define MINEMAP_USAGE _( "minemap <options>\n" \
"\t-d, --dithering\n" \
"\t\tOptional, turn on Floyd-Steinberg dithering\n" \
"\t-i, --input INPUT\n" \
"\t\tRequired, image input\n" \
"\t--no-gz\n" \
"\t\tOptional, do not gzip generated NBT file (for human inspection)\n" \
"\t-o, --output FILE\n" \
"\t\tRequired, output file in NBT format\n" \
"\t-e, --export FILE\n" \
"\t\tOptional, export the result of color reduction in png format\n" \
"\t-g, --game VER\n" \
"\t\tRequired, MINIMUM game version this map can be used in\n" \
"\t\tSelect from the following values: \n" \
)

#define PAMENIM_USAGE _( "pamenim <options>\n" \
"\t-i, --input INPUT\n" \
"\t\tRequired, nbt input\n" \
"\t--no-gz\n" \
"\t\tOptional, indicates that the input file is uncompressed\n" \
"\t-o, --output OUTPUT\n" \
"\t\tRequired, output picture\n" \
"\t-g, --game VER\n" \
"\t\tRequired, Minecraft game version this map is exported from\n" \
"\t\tSelect from the following values: \n" \
)

#define VERSION_RANGE _("\t\t\t{:>8} for game version {}\n")
#define VERSION_OLDER_UNSUPPORTED _("\t\tOlder versions are not supported.\n\n")

#ifdef USE_GM
#define MINEMAP_MAGICK_IMPLEMENTATION "GraphicsMagick"
#else
#define MINEMAP_MAGICK_IMPLEMENTATION "ImageMagick"
#endif
#define VERSION_MESSAGE "Application version " MINEMAP_APP_VER " with " MINEMAP_MAGICK_IMPLEMENTATION

#endif //CONSTANTS_H
