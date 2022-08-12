//
// Created by cth451 on 2020/05/15.
//

#ifndef MINEMAP_COLORMAP_H
#define MINEMAP_COLORMAP_H

#include <Magick++.h>
#include <memory>
#include <map>
#include <optional>
#include <compare>
#include <unordered_map>

namespace Minemap {
	struct TupleRGB {
		double r = 0.0;
		double g = 0.0;
		double b = 0.0;

		constexpr TupleRGB() = default;

		TupleRGB(double r, double g, double b) noexcept: r(r), g(g), b(b) {};

		// Comparison operators
		bool operator==(const TupleRGB &rhs) const = default;

		bool operator!=(const TupleRGB &rhs) const = default;

		friend std::strong_ordering operator<=>(const TupleRGB &lhs, const TupleRGB &rhs) = default;

		// Construct from Magick ColorRGB
		explicit TupleRGB(const Magick::ColorRGB &color) noexcept;

		// Cast to Magick ColorRGB
		explicit operator Magick::ColorRGB() const noexcept;
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
			auto hr = static_cast<unsigned long>(rgb.r * 256);
			auto hg = static_cast<unsigned long>(rgb.g * 256);
			auto hb = static_cast<unsigned long>(rgb.b * 256);
			return (hr << 16) + (hg << 8) + hb;
		}
	};
}

namespace Minemap {
	typedef uint8_t MapColorCode;

	struct VersionSpec;

	/**
	 * Replaces boost::bimap. This maps minecraft map color code between RGB tuples.
	 */
	class ColorMap {
	private:
		typedef std::unordered_map<TupleRGB, MapColorCode> ReverseLookup;
		typedef TupleRGB *ForwardLookup;
		uint8_t _size;
		ReverseLookup reverseMap;
		ForwardLookup forwardMap; // Note that this is a pointer to an array on the heap
	public:
		/**
		 * Insert mapping between RGB <-> Color code
		 * @param rgb    RGB
		 * @param code   Map color code
		 */
		void insert(const TupleRGB &rgb, MapColorCode code);

		/**
		 * @return size of this mapping
		 */
		uint8_t size() const noexcept { return _size; };

		/**
		 * Lookup RGB from Map color code
		 * @param code   color code
		 * @return       RGB or nullopt if code not found
		 */
		std::optional<TupleRGB> lookup(MapColorCode code) const noexcept;

		/**
		 * Lookup Map color code from RGB
		 * @param rgb     RGB tuple
		 * @return        Map color code or nullopt if this RGB is not mapped
		 */
		std::optional<MapColorCode> lookup(const TupleRGB &rgb) const noexcept;

		/**
		 * Default constructor must not be used as the array size must be determined at construction
		 */
		ColorMap() = delete;

		/**
		 * construct a color map with this much size
		 * @param size capacity / the number of color codes this color map can hold
		 */
		explicit ColorMap(uint8_t size) noexcept;

		/**
		 * Destructor
		 */
		~ColorMap() noexcept;
	};

	/**
	 * Constructs a bijective map between every color in the palette and corresponding color code in game. This data
	 * structure allows fast lookup into both representations.
	 *
	 * @param ver       Version
	 * @return          the said bijective map
	 */
	std::shared_ptr<ColorMap> loadColorMapFromVersion(const VersionSpec &ver);
}


#endif //COLORMAP_H
