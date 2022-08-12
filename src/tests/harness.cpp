//
// Created by cth451 on 2021/9/14.
//

#include "ColorMap.h"
#include "VersionSpec.h"
#include "libminemap.h"
#include "Marker.h"

#include <algorithm>
#include <cstdio>
#include <cassert>

void test_colormap() {

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
	test_colormap();
	//test_pos_parser();
	return 0;
}