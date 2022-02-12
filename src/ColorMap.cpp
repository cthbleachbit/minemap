//
// Created by cth451 on 2020/05/15.
//

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
		ssize_t palette_width = palette_img.size().width();
		ssize_t palette_height = palette_img.size().height();
		ssize_t palette_pixels = palette_width * palette_height;

		MapColorCode maxCode = std::min(palette_pixels, (ssize_t) UINT8_MAX);

		auto color_map = std::make_shared<ColorMap>(maxCode);
		for (MapColorCode j = 0; j < maxCode; j++) {
			ssize_t palette_col = j % palette_width;
			ssize_t palette_row = j / palette_width;
			// This should have been palette_img.pixelColor, but visual studio refuses to link to it
#ifdef USE_GM
			auto p_inf = palette_img.pixelColor(palette_col, palette_row);
#else
			auto q = palette_img.getConstPixels(palette_col, palette_row, 1, 1);
			Magick::PixelInfo p_inf;
			MagickCore::GetPixelInfoPixel(palette_img.constImage(), q, &p_inf);
#endif
			Magick::ColorRGB palette_pix = Magick::Color(p_inf);

			// Insert the pixel into the color map
			color_map->insert(Minemap::TupleRGB(palette_pix), static_cast<MapColorCode>(j));
		}
		return color_map;
	}

	TupleRGB::TupleRGB(const Magick::ColorRGB &color) noexcept
		: r(color.red()), g(color.green()), b(color.blue()) {}

	TupleRGB::operator Magick::ColorRGB() const noexcept {
		Magick::ColorRGB rgb;
		rgb.red(this->r);
		rgb.green(this->g);
		rgb.blue(this->b);
		rgb.alpha(1.0);
		return rgb;
	};

	void ColorMap::insert(const TupleRGB &rgb, MapColorCode code) {
		if (code >= _size) {
			return;
		}
		this->reverseMap.insert(std::make_pair(rgb, code));
		this->forwardMap[code] = rgb;
	}

	ColorMap::ColorMap(uint8_t size) noexcept {
		this->_size = size;
		this->forwardMap = new TupleRGB[size];
	}

	ColorMap::~ColorMap() noexcept {
		delete this->forwardMap;
	}

	std::optional<TupleRGB> ColorMap::lookup(MapColorCode code) const noexcept {
		if (code > _size) {
			return std::nullopt;
		}
		return this->forwardMap[code];
	}

	std::optional<MapColorCode> ColorMap::lookup(const TupleRGB &rgb) const noexcept {
		auto itr = this->reverseMap.find(rgb);
		return itr == this->reverseMap.end() ? std::nullopt : std::optional<MapColorCode>(itr->second);
	}
}
