//
// Created by cth451 on 2020/05/15.
//

#include "common.h"
#include "ColorMap.h"

namespace Minemap {

	/**
	 * Constructs a bijective map between every color in the palette and corresponding colro code in game. This data
	 * structure allows fast lookup into both representations.
	 *
	 * @param img       Loaded Magick image
	 * @return          the said bijective map
	 */
	std::shared_ptr<ColorMap> loadColorMapFromPalette(const Magick::Image &palette_img) {
		auto color_map = std::make_shared<ColorMap>();
		int palette_width = palette_img.size().width();
		int palette_height = palette_img.size().height();
		int palette_number = palette_width * palette_height;

		for (int j = 0; j < palette_number; j++) {
			ssize_t palette_col = j % palette_width;
			ssize_t palette_row = j / palette_width;
			// This should have been palette_img.pixelColor, but visual studio refuses to link to it
			auto q = palette_img.getConstPixels(palette_col, palette_row, 1, 1);
			Magick::PixelInfo p_inf;
			MagickCore::GetPixelInfoPixel(palette_img.constImage(), q, &p_inf);
			Magick::ColorRGB palette_pix = Magick::Color(p_inf);

			// Insert the pixel into the color map
			color_map->insert(ColorEntry(Minemap::TupleRGB(palette_pix), static_cast<int8_t>(j)));
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
	}

	bool TupleRGB::operator<(const TupleRGB &rhs) const {
		if (r < rhs.r)
			return true;
		if (rhs.r < r)
			return false;
		if (g < rhs.g)
			return true;
		if (rhs.g < g)
			return false;
		return b < rhs.b;
	}

	bool TupleRGB::operator>(const TupleRGB &rhs) const {
		return rhs < *this;
	}

	bool TupleRGB::operator<=(const TupleRGB &rhs) const {
		return !(rhs < *this);
	}

	bool TupleRGB::operator>=(const TupleRGB &rhs) const {
		return !(*this < rhs);
	}

	TupleRGB::operator Magick::ColorRGB() const {
		Magick::ColorRGB rgb;
		rgb.red(this->r);
		rgb.green(this->g);
		rgb.blue(this->b);
		rgb.alpha(1.0);
		return rgb;
	};
}
