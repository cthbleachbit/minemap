//
// Created by cth451 on 2020/05/15.
//

#ifndef COLORMAP_H
#define COLORMAP_H

#include <Magick++.h>
#include <memory>
#include <map>

namespace Minemap {
	class TupleRGB {
	public:
		double r;
		double g;
		double b;
		TupleRGB();
		explicit TupleRGB(const Magick::ColorRGB &color);
		bool operator==(const TupleRGB &rhs) const;
		bool operator!=(const TupleRGB &rhs) const;
		bool operator<(const TupleRGB &rhs) const;
		bool operator>(const TupleRGB &rhs) const;
		bool operator<=(const TupleRGB &rhs) const;
		bool operator>=(const TupleRGB &rhs) const;
		explicit operator Magick::ColorRGB() const;
	};
}

namespace std {
	template<>
	struct hash<Minemap::TupleRGB> {
		/**
		 * Extended hash for the TupleRGB since Magick Quantum is very problematic.
		 * @param rgb
		 * @return
		 */
		unsigned long operator()(const Minemap::TupleRGB &rgb) const {
			unsigned long hr = std::hash<double>()(rgb.r);
			unsigned long hg = std::hash<double>()(rgb.g);
			unsigned long rb = std::hash<double>()(rgb.b);
			return ((hr ^ (hg << 1)) >> 1) ^ (rb << 1);
		}
	};
}

namespace Minemap {
	typedef uint8_t MapColorCode;
	/**
	 * Replaces boost::bimap. This maps minecraft map color code between RGB tuples.
	 */
	class ColorMap {
	public:
		typedef std::unordered_map<TupleRGB, MapColorCode> ReverseLookup;
		typedef TupleRGB* ForwardLookup;
	private:
		uint8_t _size;
		ReverseLookup reverseMap;
		ForwardLookup forwardMap;
	public:
		const ReverseLookup & reverse() const;
		void insert(const TupleRGB& rgb, MapColorCode code);
		TupleRGB lookup(MapColorCode code) const;

		explicit ColorMap(uint8_t size) noexcept;
		~ColorMap() noexcept;
	};

	/**
	 * Constructs a bijective map between every color in the palette and corresponding colro code in game. This data
	 * structure allows fast lookup into both representations.
	 *
	 * @param img       Loaded Magick image
	 * @return          the said bijective map
	 */
	std::shared_ptr<ColorMap> loadColorMapFromPalette(const Magick::Image &img);
}



#endif //COLORMAP_H
