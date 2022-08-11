//
// Created by cth451 on 2021/9/14.
//

#include "../ColorMap.h"
#include "../VersionSpec.h"
#include "libminemap.h"
#include "Marker.h"

#include <algorithm>
#include <cstdio>

void test_colormap() {
	Magick::Image palette_img;
	auto palette_data = Minemap::verSpecToPaletteData(Minemap::Version::MC_1_16);
	palette_img = Minemap::verspec_to_image(palette_data);
	auto palette_lookup_table = Minemap::loadColorMapFromPalette(palette_img);
	// Make sure every color is retrievable
	uint8_t size = palette_lookup_table->size();
	for (uint8_t i = 4; i < size; i++) {
		// Try to retrieve the color rgb and make sure it matches
		std::optional<Minemap::TupleRGB> rgb = palette_lookup_table->lookup(i);
		if (!rgb.has_value()) {
			std::printf("Color Code %u missing\n", i);
			exit(5);
		}
		std::optional<uint8_t> check = palette_lookup_table->lookup(rgb.value());
		if (!check.has_value()) {
			std::printf("Color Code %u has no RGB color mapped??\n", i);
			exit(5);
		}
		if (check.value() != i) {
			std::printf("Color Code %u doesn't match lookup result %u\n", i, check.value());
			exit(5);
		}
		printf("%03u %08lx\n", i, std::hash<Minemap::TupleRGB>{}(rgb.value()));
	}
}

void test_pos_parser() {
	Minemap::MarkerPosition pos1("abs:10:-20:30", 0, 0);
	Minemap::Banner banner(pos1, "red");
	banner = Minemap::Banner(pos1, "purple:bleh");
	auto tag = banner.toCompound();
	tag->textOutput(std::cout, 0);
	std::cout << fmt::format("{:a}", banner) << std::endl;
}

int main(int argc, char **argv) {
	// test_colormap();
	test_pos_parser();
	return 0;
}