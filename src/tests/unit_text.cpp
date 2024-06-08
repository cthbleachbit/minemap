#include <ColorMap.h>
#include <filesystem>
#include <gtest/gtest.h>
#include <nbtp/libnbtp.h>
#include "Map.h"
#include "libminemap.h"
#include "test_helpers.h"

INSTANTIATE_TEST_SUITE_P(AptWhiteOnAlpha_UpperLeft, ImageLoadingTest,
						 testing::Values(ImageLoadingTestParam{
							 .image_name = "apt-white-on-alpha.png",
							 .version = Minemap::Version::MC_1_17,
							 .expected_corner = {{
								 {},
								 {},
								 {},
								 {},
								 {},
								 {},
								 {0, 34, 34, 0, 0, 34, 34, 0},
								 {0, 0, 0, 34, 0, 34, 0, 34},
							 }},
							 .left = true,
							 .up = true,
						 }));
INSTANTIATE_TEST_SUITE_P(AptWhiteOnAlpha_UpperRight, ImageLoadingTest,
						 testing::Values(ImageLoadingTestParam{
							 .image_name = "apt-white-on-alpha.png",
							 .version = Minemap::Version::MC_1_17,
							 .expected_corner = {{
								 {34},
								 {0, 34},
								 {0, 0, 34},
								 {0, 0, 0, 34},
								 {0, 0, 0, 0, 34},
								 {0, 0, 0, 0, 0, 34},
								 {0, 0, 0, 0, 0, 0, 34},
								 {0, 0, 0, 0, 0, 0, 0, 34},
							 }},
							 .left = false,
							 .up = true,
						 }));
INSTANTIATE_TEST_SUITE_P(AptWhiteOnAlpha_LowerLeft, ImageLoadingTest,
						 testing::Values(ImageLoadingTestParam{
							 .image_name = "apt-white-on-alpha.png",
							 .version = Minemap::Version::MC_1_17,
							 .expected_corner = {{
								 {},
								 {},
								 {},
								 {},
								 {34, 34, 34, 34},
								 {0, 0, 0, 34},
								 {0, 0, 0, 34},
								 {0, 0, 0, 34},
							 }},
							 .left = true,
							 .up = false,
						 }));

INSTANTIATE_TEST_SUITE_P(AptWhiteOnGreen_UpperLeft, ImageLoadingTest,
						 testing::Values(ImageLoadingTestParam{
							 .image_name = "apt-white-on-green.png",
							 .version = Minemap::Version::MC_1_17,
							 .expected_corner = {{
								 {5, 5, 5, 5, 5, 5, 5, 5},
								 {5, 5, 5, 5, 5, 5, 5, 5},
								 {34, 34, 5, 5, 34, 34, 5, 5},
								 {5, 5, 34, 5, 34, 5, 34, 5},
								 {5, 34, 34, 5, 34, 5, 34, 5},
								 {34, 5, 34, 5, 34, 34, 5, 5},
								 {5, 34, 34, 5, 34, 5, 5, 5},
								 {5, 5, 5, 5, 5, 5, 5, 5},
							 }},
							 .left = true,
							 .up = true,
						 }));
INSTANTIATE_TEST_SUITE_P(AptWhiteOnGreen_UpperRight, ImageLoadingTest,
						 testing::Values(ImageLoadingTestParam{
							 .image_name = "apt-white-on-green.png",
							 .version = Minemap::Version::MC_1_17,
							 .expected_corner = {{
								 {69, 94, 69, 94, 69, 94, 69, 94},
								 {94, 69, 94, 94, 94, 94, 94, 94},
								 {69, 94, 69, 94, 69, 94, 69, 94},
								 {94, 94, 94, 94, 94, 94, 94, 94},
								 {69, 94, 69, 94, 69, 94, 69, 94},
								 {94, 69, 94, 94, 94, 69, 94, 94},
								 {69, 94, 69, 94, 69, 94, 69, 94},
								 {94, 94, 94, 94, 94, 94, 94, 94},
							 }},
							 .left = false,
							 .up = true,
						 }));


int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	Magick::InitializeMagick(nullptr);
	return RUN_ALL_TESTS();
}
