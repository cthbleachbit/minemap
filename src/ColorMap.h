//
// Created by cth451 on 2020/05/15.
//

#ifndef COLORMAP_H
#define COLORMAP_H

#include <Magick++.h>
#include <memory>
#include <boost/bimap.hpp>

namespace Minemap {
	class TupleRGB {
	public:
		double r;
		double g;
		double b;
		TupleRGB();
		TupleRGB(const Magick::ColorRGB &color);
		bool operator==(const TupleRGB &rhs) const;
		bool operator!=(const TupleRGB &rhs) const;
		bool operator<(const TupleRGB &rhs) const;
		bool operator>(const TupleRGB &rhs) const;
		bool operator<=(const TupleRGB &rhs) const;
		bool operator>=(const TupleRGB &rhs) const;
	};

	typedef boost::bimap<TupleRGB, int8_t> ColorMap;
	typedef ColorMap::value_type ColorEntry;

	std::shared_ptr<ColorMap> loadColorMapFromPalette(const Magick::Image &img);
}

namespace std {
	template<>
	struct hash<Minemap::TupleRGB> {
		unsigned long operator()(const Minemap::TupleRGB &rgb) const {
			unsigned long hr = std::hash<double>()(rgb.r);
			unsigned long hg = std::hash<double>()(rgb.g);
			unsigned long rb = std::hash<double>()(rgb.b);
			return ((hr ^ (hg << 1)) >> 1) ^ (rb << 1);
		}
	};
}

#endif //COLORMAP_H
