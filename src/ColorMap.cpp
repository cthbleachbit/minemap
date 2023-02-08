/**
 * @file Color map handles conversion between minecraft color codes and RGBA values
 */

#include "ColorMap.h"
#include "VersionSpec.h"

namespace Minemap {

	/**
	 * Constructs a bijective map between every color in the palette and corresponding color code in game. This data
	 * structure allows fast lookup into both representations.
	 *
	 * @param ver       Version
	 * @return          the said bijective map
	 */
	std::shared_ptr<ColorMap> loadColorMapFromVersion(const VersionSpec &ver) {
		MapColorCode maxCode = std::min((int) (4 * ver.paletteHeight), UINT8_MAX);

		auto color_map = std::make_unique<ColorMap>(maxCode);

		for (MapColorCode j = 0; j < maxCode; j++) {
			Minemap::TupleRGB pix((double) ver.paletteData[4 * j] / 255.0, (double) ver.paletteData[4 * j + 1] / 255.0, (double) ver.paletteData[4 * j + 2] / 255.0);
			color_map->insert(pix, static_cast<MapColorCode>(j));
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
