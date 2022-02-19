//
// Created by cth451 on 2022/2/17.
//

#include <regex>
#include "Marker.h"

namespace Minemap {

	static const std::regex COLON(":");
	static const std::regex SPACE(" +");

	/**
	 * Create a marker position object based
	 * @param spec       "rel:<x>:<y>:<z>" or "abs:<x>:<y>:<z>"
	 * @param xOffset    xOffset from map
	 * @param zOffset    yOffset from map
	 */
	MarkerPosition::MarkerPosition(const std::string &spec, NBTP::IntTag::V xOffset, NBTP::IntTag::V zOffset) {

		std::sregex_token_iterator begin{spec.cbegin(), spec.cend(), COLON, -1}, last;
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

	std::shared_ptr<NBTP::CompoundTag> MarkerPosition::toCompound() const noexcept {
		auto tag = std::make_shared<NBTP::CompoundTag>();
		auto &payload = tag->getPayload();
		payload["X"] = std::make_shared<NBTP::IntTag>(this->x + this->xOffset);
		payload["Y"] = std::make_shared<NBTP::IntTag>(this->y);
		payload["Z"] = std::make_shared<NBTP::IntTag>(this->z + this->zOffset);
		return tag;
	}

	std::shared_ptr<NBTP::CompoundTag> Banner::toCompound() const noexcept {
		auto tag = std::make_shared<NBTP::CompoundTag>();
		auto &payload = tag->getPayload();
		if (this->name) {
			payload["Name"] = std::make_shared<NBTP::StringTag>(*this->name);
		}
		payload["Color"] = std::make_shared<NBTP::StringTag>(this->color);
		payload["Pos"] = this->position.toCompound();
		return tag;
	}

	Banner::Banner(const std::string &spec, NBTP::IntTag::V xOffset, NBTP::IntTag::V zOffset) {
		std::sregex_token_iterator begin{spec.cbegin(), spec.cend(), SPACE, -1}, last;
		std::vector<std::string> tokens{begin, last};
		if (tokens.size() != 2) {
			throw std::invalid_argument(INVALID_BANNER);
		}
		position = MarkerPosition(tokens[0], xOffset, zOffset);
		std::sregex_token_iterator b_begin{tokens[1].cbegin(), tokens[1].cend(), COLON, -1}, b_last;
		std::vector<std::string> b_tokens{b_begin, b_last};
		if (b_tokens.size() == 1) {
			this->color = b_tokens[0];
		} else if (b_tokens.size() == 2) {
			this->color = b_tokens[0];
			this->name = b_tokens[1];
		} else {
			throw std::invalid_argument(INVALID_BANNER);
		}
	}
}