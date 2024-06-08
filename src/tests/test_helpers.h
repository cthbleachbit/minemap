#ifndef MINEMAP_TEST_HELPERS_H
#define MINEMAP_TEST_HELPERS_H

#include <array>
#include <filesystem>
#include <gtest/gtest.h>
#include <nbtp/libnbtp.h>
#include "Map.h"
#include "libminemap.h"

#include <ColorMap.h>

/* Corner Select Functions */
constexpr ssize_t C_WIDTH = 8;
constexpr ssize_t F_WIDTH = 128;
static_assert(F_WIDTH - C_WIDTH > 0);
typedef std::array<std::array<uint8_t, C_WIDTH>, C_WIDTH> TestSquare;
std::ostream& operator<<(std::ostream& os, const TestSquare& square);
TestSquare select_corner(bool left, bool up, const std::vector<std::shared_ptr<NBTP::Tag>>& colors);

/* Testing parameters */
struct ImageLoadingTestParam {
	/* Inputs */
	std::string image_name;
	Minemap::Version version = Minemap::Version::INVALID;

	/* Expected values */
	TestSquare expected_corner{};
	bool left = false;
	bool up = false;
};
std::ostream& operator<<(std::ostream& os, const ImageLoadingTestParam& param);


class ImageLoadingTest : public ::testing::TestWithParam<ImageLoadingTestParam> {
protected:
	ImageLoadingTest() {
		const ImageLoadingTestParam& param = GetParam();
		this->input_img.read(std::filesystem::current_path() / param.image_name);
		this->version = param.version;
		this->map_tag = Minemap::Map::makeMapRoot(param.version);
		this->expected_corner = param.expected_corner;
		this->left = param.left;
		this->up = param.up;
	}

	Magick::Image input_img;
	Minemap::Version version;
	std::shared_ptr<NBTP::CompoundTag> map_tag;
	TestSquare expected_corner{};
	bool left;
	bool up;
};

TEST_P(ImageLoadingTest, ConversionTest) {
	auto size = this->input_img.size();
	if (size.width() != 128 || size.height() != 128) {
		GTEST_SKIP();
	}

	auto colors_tag = Minemap::Map::getModifiableColors(this->map_tag);
	colors_tag->getPayload().clear();
	auto color_map_from_data = Minemap::loadColorMapFromVersion(Minemap::verSpecToPaletteData(this->version));
	Minemap::mapped_to_tag(input_img, input_img, color_map_from_data, colors_tag);

	/* Check whether the top left corner matches */
	TestSquare converted_corner = select_corner(this->left, this->up, colors_tag->getPayload());
	std::cout << converted_corner;
	ASSERT_EQ(this->expected_corner, converted_corner);
}

#endif /* MINEMAP_TEST_HELPERS_H */
