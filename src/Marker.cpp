//
// Created by cth451 on 2022/2/17.
//

#include <regex>
#include "Marker.h"

namespace Minemap {
	/**
	 * Create a marker position object based
	 * @param spec       "rel:<x>:<y>:<z>" or "abs:<x>:<y>:<z>"
	 * @param xOffset    xOffset from map
	 * @param zOffset    yOffset from map
	 */
	MarkerPosition::MarkerPosition(const std::string &spec, NBTP::IntTag::V xOffset, NBTP::IntTag::V zOffset) {
		static const std::regex re(":");
		std::sregex_token_iterator begin{spec.cbegin(), spec.cend(), re, -1}, last;
		std::vector<std::string> tokens{begin, last};
		if (tokens.size() != 4) {
			throw std::invalid_argument(INVALID_POSITION);
		}
		if (tokens[0] == "rel") {
			this->xOffset = xOffset;
			this->zOffset = zOffset;
		} else if (tokens[0] != "abs") {
			throw std::invalid_argument(INVALID_POSITION);
		}
		try {
			this->x = std::stoi(tokens[1]);
			this->y = std::stoi(tokens[2]);
			this->z = std::stoi(tokens[3]);
		} catch (const std::invalid_argument &e) {
			throw std::invalid_argument(INVALID_POSITION);
		} catch (const std::out_of_range &e) {
			throw std::invalid_argument(POSITION_OUT_OF_RANGE);
		}
	}

	std::partial_ordering MarkerPosition::operator<=>(const MarkerPosition &pos) const noexcept {
		if (x + xOffset == pos.x + pos.xOffset && y == pos.y && z + zOffset == pos.z + pos.zOffset) {
			return std::partial_ordering::equivalent;
		}
		return std::partial_ordering::unordered;
	}
}