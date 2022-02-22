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
		static const std::regex COLON(":");
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

	/**
	 * Create a marker position object from CompoundTag
	 * @param position      compound tag containing three int tags X, Y and Z
	 * @throw runtime_error if this tag does not represent a set of 3D coordinates
	 */
	MarkerPosition::MarkerPosition(const std::shared_ptr<NBTP::Tag>& _position) {
		if (!_position || _position->typeCode() != NBTP::COMPOUND) {
			throw std::runtime_error(POSITION_MALFORMED);
		}
		auto *position = (NBTP::CompoundTag *) _position.get();
		auto xt = position->lookup("X");
		auto yt = position->lookup("Y");
		auto zt = position->lookup("Z");
		if (xt == nullptr || yt == nullptr || zt == nullptr) {
			throw std::runtime_error(POSITION_MALFORMED);
		}
		if (xt->typeCode() != NBTP::INT || yt->typeCode() != NBTP::INT || zt->typeCode() != NBTP::INT) {
			throw std::runtime_error(POSITION_MALFORMED);
		}
		x = ((NBTP::IntTag*) xt.get())->getPayload();
		y = ((NBTP::IntTag*) yt.get())->getPayload();
		z = ((NBTP::IntTag*) zt.get())->getPayload();
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

    Banner::Banner(const std::shared_ptr<NBTP::Tag> _banner) {
		if (!_banner || _banner->typeCode() != NBTP::COMPOUND) {
			throw std::runtime_error(BANNERS_MALFORMED);
		}
		auto *banner = (NBTP::CompoundTag *) _banner.get();
		auto color_tag = banner->lookup("Color");
		auto name_tag = banner->lookup("Name");
		auto position_tag = banner->lookup("Pos");
		if (color_tag == nullptr || position_tag == nullptr) {
			throw std::runtime_error(BANNERS_MALFORMED);
		}
		if (color_tag->typeCode() != NBTP::STRING || position_tag->typeCode() != NBTP::COMPOUND) {
			throw std::runtime_error(BANNERS_MALFORMED);
		}
		if (name_tag && name_tag->typeCode() != NBTP::STRING) {
			throw std::runtime_error(BANNERS_MALFORMED);
		}
		color = ((NBTP::StringTag *) color_tag.get())->getPayload();
		position = MarkerPosition(position_tag);
		if (name_tag) {
			name = ((NBTP::StringTag *) name_tag.get())->getPayload();
		}
	}
}