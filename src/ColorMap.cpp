//
// Created by cth451 on 2020/05/15.
//

#include "ColorMap.h"

namespace Minemap {

	std::shared_ptr<ColorMap> loadColorMapFromPalette(const Magick::Image &palette_img) {
		auto color_map = std::make_shared<ColorMap>();
		int palette_width = palette_img.size().width();
		int palette_height = palette_img.size().height();
		int palette_number = palette_width * palette_height;

		for (int j = 0; j < palette_number; j++) {
			ssize_t palette_col = j % palette_width;
			ssize_t palette_row = j / palette_width;
			Magick::ColorRGB palette_pix = palette_img.pixelColor(palette_col, palette_row);

			// Insert the pixel into the color map
			(*color_map)[Minemap::TupleRGB(palette_pix)] = static_cast<int8_t>(j);
		}
		return color_map;
	}

	TupleRGB::TupleRGB()
		: r(0.0), g(0.0), b(0.0) {}

	TupleRGB::TupleRGB(const Magick::ColorRGB &color)
		: r(color.red()), g(color.green()), b(color.blue()) {}

	bool TupleRGB::operator==(const TupleRGB &rhs) const {
		return r == rhs.r &&
			g == rhs.g &&
			b == rhs.b;
	}

	bool TupleRGB::operator!=(const TupleRGB &rhs) const {
		return !(rhs == *this);
	};
}
