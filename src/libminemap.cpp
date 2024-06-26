/**
 * @file libminemap entry point function implementations
 */

#include <fmt/format.h>
#include <iostream>
#include <memory>
#include "libminemap.h"
#include "Map.h"
#include "ColorMap.h"

namespace Minemap {
	void print_version() noexcept {
		std::cout << VERSION_MESSAGE << std::endl;
	}

	MINEMAP_CLONE
	void _clone_mapped_to_tag(const Magick::Image &input_img,
	                          const Magick::Image &mapped_img,
	                          const std::shared_ptr<ColorMap> &palette_lookup_table,
	                          NBTP::BytesTag *colors_tag) {
		size_t width = input_img.columns();
		size_t height = input_img.rows();

		// FIXME: GraphicsMagick Image::pixelColor() for some reason does not contain alpha channel
		bool has_alpha = input_img.matte();
		// FIXME: Have to map low level access
		const Magick::PixelPacket *input_img_cache = input_img.getConstPixels(0, 0, width, height);

		// Count partially transparent pixels
		int partially_transparent_count = 0;

		for (size_t i = 0; i < width * height; i++) {
			ssize_t col = i % width;
			ssize_t row = i / height;
			Magick::Color mapped_pix = mapped_img.pixelColor(col, row);

			const Magick::PixelPacket *input_pix = input_img_cache + i;
			// GraphicsMagick - alpha = 65535 under 16b <=> transparent
			//                  alpha = 0               <=> completely opaque
			double quantum_alpha = input_pix->opacity;
			if (quantum_alpha < TransparentOpacity && quantum_alpha != OpaqueOpacity) {
				partially_transparent_count++;
			}
			// Pixels that are not fully opaque are forced to be fully transparent
			bool is_transparent_input = quantum_alpha != 0;
			bool is_transparent = has_alpha && is_transparent_input;

			std::optional<MapColorCode> colorCode;
			if (is_transparent) {
				colorCode = 0;
			} else {
				colorCode = palette_lookup_table->lookup(TupleRGB(mapped_pix));
				if (!colorCode.has_value()) {
					std::string no_match = localizedFormat(COLOR_MISMATCH, col, row);
					throw std::runtime_error(no_match);
				}
			}
			colors_tag->insert(std::make_shared<NBTP::ByteTag>(colorCode.value()));
		}

		if (partially_transparent_count) {
			std::cerr << localizedFormat(COLOR_PARTIALLY_TRANSPARENT, partially_transparent_count) << std::endl;
		}
	}

	void mapped_to_tag(const Magick::Image &input_img,
	                   const Magick::Image &mapped_img,
	                   const std::shared_ptr<ColorMap> &palette_lookup_table,
	                   NBTP::BytesTag *colors_tag) {
		_clone_mapped_to_tag(input_img, mapped_img, palette_lookup_table, colors_tag);
	}

	MINEMAP_CLONE
	std::unique_ptr<std::array<double, 16384 * 4>>
	_clone_tag_to_pixelstore(const NBTP::ListTag::List &colors_list,
	                         const std::shared_ptr<ColorMap> &palette_lookup_table) {
		auto pixel_store = std::make_unique<std::array<double, 16384 * 4>>();
		auto pixel_array = pixel_store->data();
		for (int i = 0; i < 16384; i++) {
			MapColorCode colorIndex = (dynamic_cast<NBTP::ByteTag*>(colors_list[i].get())->getPayload());
			auto rgb = palette_lookup_table->lookup(colorIndex);

			if (!rgb.has_value()) {
				std::string error_string = localizedFormat(COLOR_OUT_OF_RANGE, colorIndex, i);
				throw std::runtime_error(error_string);
			}

			pixel_array[i * 4] = rgb->r;
			pixel_array[i * 4 + 1] = rgb->g;
			pixel_array[i * 4 + 2] = rgb->b;
			pixel_array[i * 4 + 3] = colorIndex < 4 ? 0 : MaxRGBDouble;
		}
		return pixel_store;
	}

	std::unique_ptr<std::array<double, 16384 * 4>>
	tag_to_pixelstore(const NBTP::ListTag::List &colors_tag,
	                  const std::shared_ptr<ColorMap> &palette_lookup_table) {
		return _clone_tag_to_pixelstore(colors_tag, palette_lookup_table);
	}
}
